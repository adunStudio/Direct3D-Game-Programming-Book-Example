#include "stdafx.h"
#include "GameFramework.h"


CGameFramework::CGameFramework()
{
	/*
	Direct3D ����̽�, ���� ü��, ����̽� ���ؽ�Ʈ, �׸��� ���� Ÿ�� �� �������̽� �����͸� nullptr�� �ʱ�ȭ�Ѵ�.
	*/

	// ����̽� �������̽��� ���� �����ʹ�.  �ַ� ���ҽ��� �����ϴ� �� �ʿ��ϴ�.
	m_pd3dDevice = nullptr;

	// ���� ü�� �������̽��� ���� �����ʹ�. �ַ� ���÷��̸� �����ϴ� �� �ʿ��ϴ�.
	m_pDXGISwapChain = nullptr;

	// ����̽� ���ؽ�Ʈ�� ���� �����ʹ�.    �ַ� ������������ �����ϴ� �� �ʿ��ϴ�.
	m_pd3dDeviceContext = nullptr;

	// ���� Ÿ�� �� �������̽��� ���� �������̴�.
	m_pd3dRenderTargetView = nullptr;
	m_pd3dDepthStencilView = nullptr;
	m_pd3dDepthStencilBuffer = nullptr;

	m_nWndClientWidth  = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_scene = nullptr;

	_tcscpy_s(m_pszBuffer, _T("LapProject ("));

	m_nPlayers = 0;
	m_ppPlayers = nullptr;
}

CGameFramework::~CGameFramework()
{
}

// �� �����찡 ������ �� ȣ��Ǵ� �Լ�, �� �Լ����� Direct3D�� ������ �ʱ�ȭ�� �Ѵ�.
// Direct3D ���÷��̸� �����ϴ� �Լ� createDirect3DDisplay()�� ���� ��ü�� �����ϴ� �Լ� buildObjects()�� ȣ���Ѵ�.
bool CGameFramework::onCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	/*
	Direct3D�� �ʱ�ȭ�ϴ� ������ ũ�� �� ���� �ܰ�� �̷������.

	1. Direct3D ����̽�, ���ؽ�, ���� ü�� �����ϱ�

	2. ���� Ÿ�� �� �����ϱ�
	*/

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	// Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�ε��� �����ϴ� �Լ��� ȣ���Ѵ�.
	if (!createDirect3DDisplay())
		return false;

	// �������� ��ü�� �����Ѵ�.
	buildObjects();

	return true;
}

// �� �����찡 �ı��� �� ȣ��Ǵ� �Լ�, �� �Լ����� Direct3D�� ������ ������ �Ѵ�.
void CGameFramework::onDestroy()
{
	// ���� ��ü�� �Ҹ��Ѵ�.
	releaseObjects();

	if (m_pd3dDeviceContext)    m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain)       m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext)    m_pd3dDeviceContext->Release();
	if (m_pd3dDevice)			m_pd3dDevice->Release();
}


// ���� Ÿ�ٺ並 �����ϴ� �Լ�
// �� �Լ��� ���� ü���� ù ��° �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����ϰ� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
bool CGameFramework::createRenderTargetView()
{
	HRESULT hResult = S_OK;

	// ���� ü���� �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
	ID3D11Texture2D* pd3dBackBuffer;

	// ���� ü���� ù ��° �ĸ� ���� �������̽��� �����´�.
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pd3dBackBuffer)))
		return false;

	// ���� ü���� ù ��° �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, nullptr, &m_pd3dRenderTargetView)))
		return false;

	if (pd3dBackBuffer)
		pd3dBackBuffer->Release();

	// ���� Ÿ�� �並 �����ϰ� ����̽� ���ؽ�Ʈ�� ��� ���� �ܰ迡 �����Ѵ�.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, nullptr);

	return true;
}

bool CGameFramework::createRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return true;
}


// Direct3D�� ������ �ʱ�ȭ(����̽� ���� ��)�� �ϴ� �Լ�, onCreate() �Լ����� ȣ��ȴ�.
// Direct3D ����̽��� �����ϸ鼭 ���� ü��, ����̽� ���ؽ�Ʈ, ���� Ÿ�� �並 �����ϰ� �����Ѵ�.
bool CGameFramework::createDirect3DDisplay()
{
	/* ����, ��� �������� Ŭ���̾�Ʈ ������ ũ�⸦ ��� ������ �����Ѵ�. */
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;


	UINT dwCreateDeviceFlags = 0;

#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		// ����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D_DRIVER_TYPE);
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	// ������ ���� ü���� �����ϴ� ����ü��.
	/*
	���� ü���� �����ϱ� ���� ���� ü�� ����ü DXGI_SWAP_CHAIN_DESC�� �����Ѵ�.
	���� ü���� ���� ũ��� �� ������ Ŭ���̾�Ʈ ������ ũ��� �����ϰ� ��� ������� �� ������� �����Ѵ�.
	���� ü���� ��� ���� ������ ���� �����Ѵ�.
	*/
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));

	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = true;
	dxgiSwapChainDesc.Flags = 0;

	/*
	���ϴ� ����̽� ����̹� ���� ������ ���� ü�ΰ� ����̽�, ����̽� ���ؽ�Ʈ�� �����ϱ� ���� ����̹� ���� �迭(d3dDriverTypes[])��
	�� ���ҿ� ���Ͽ� ����̽��� ���� ü�� ������ D3D11CreateDeviceAndSwapChain() �Լ� ȣ���� ���� �õ��Ѵ�.
	*/
	D3D_DRIVER_TYPE   nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;

	for (UINT i = 0; i < nDriverTypes; ++i)
	{
		nd3dDriverType = d3dDriverTypes[i];

		/* 
		D3D11CreateDeviceAndSwapChain() �Լ��� ���Ͽ� Direct3D ����̽�, ���� ü��, ����̽� ���ؽ�Ʈ�� ���ÿ� �����Ѵ�. 
		����, ������ ����̽� ������ �ʿ��� ����̽� ���� �÷���, ����̹� ����, Ư�� ����(Feature Level)�� �����Ѵ�.
		*/ 
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(nullptr,
			nd3dDriverType, nullptr, dwCreateDeviceFlags, d3dFeatureLevels,
			nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc,
			&m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel,
			&m_pd3dDeviceContext
		)))
			break;
	}

	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext)
		return false;

	if (!createRenderTargetDepthStencilView())
		return false;


	return true;
}

// ������ �޽����� ó���Ѵ�.
LRESULT CALLBACK CGameFramework::onProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CCamera* pCamera = m_ppPlayers[0]->getCamera();


	switch (nMessageID)
	{
	case WM_SIZE:
		// Ŭ���̾�Ʈ ������ ũ�⸦ �����Ѵ�.
		m_nWndClientWidth  = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
		
		// ���� ü���� �ĸ� ���۸� ����ϴ� ��� �������̽��� �Ҹ��Ѵ�.
		m_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(2, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		// ���� Ŭ���̾�Ʈ ������ ũ�⿡ �´� ���ο� ���� Ÿ�� �並 �����Ѵ�.
		createRenderTargetDepthStencilView();

		if (pCamera)
			pCamera->setViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);

		
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		onProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		onProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}

	return 0;
}

// ������ ���콺, Ű���� �Է� �޽����� ó���Ѵ�.
void CGameFramework::onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		// ���콺 ĸó�� �ϰ� ���� ���콺 ��ġ�� �����´�.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		// ���콺 ĸó�� �����Ѵ�.
		ReleaseCapture();
		break;

	case WM_MOUSEMOVE:
		break;

	default:
		break;
	}
}

void CGameFramework::onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
			
		case VK_F1:
		case VK_F2:
		case VK_F3:
			m_ppPlayers[0]->changeCamera(m_pd3dDevice, (wParam - VK_F1 + 1), m_gameTimer.getTimeElapsed());
			break;
		default:
			break;
		}

		break;
	
	default:
		break;
	}

}

// ���� ���α׷����� ����ϴ� ��ü���� �����Ѵ�.
void CGameFramework::buildObjects()
{
	// �� ��ü ����
	m_scene = new CScene();

	// �÷��̾� ��ü ����
	m_nPlayers = 1;
	m_ppPlayers = new CPlayer*[m_nPlayers];
	
	CAirplanePlayer* pAirplanePlayer = new CAirplanePlayer(m_pd3dDevice);

	// �÷��̾��� ī�޶� �����̽� �� ī�޶�� �����Ѵ�.
	pAirplanePlayer->changeCamera(m_pd3dDevice, SPACESHIP_CAMERA, m_gameTimer.getTimeElapsed());


	// ī�޶� ��ü ����, ����Ʈ ����
	CCamera* camera = new CCamera(nullptr);
	camera->createShaderVariables(m_pd3dDevice);
	camera->setViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);

	// ���� ��ȯ ��� ����
	camera->generateProjectionMatrix(1.0f, 5000.0f, ASPECT_RATIO, 90.0f);

	// ī�޶� ��ȯ ��� ����
	//D3DXVECTOR3 d3dxvEyePosition = D3DXVECTOR3(0.0f, 0.0f, -2.0f);
	//D3DXVECTOR3 d3dxvLookAt      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 d3dxvUp          = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//camera->generateViewMatrix(d3dxvEyePosition, d3dxvLookAt, d3dxvUp);
	camera->generateViewMatrix();

	// �÷��̾��� ��ġ�� �����̽� �� ī�޶�� �����Ѵ�.
	pAirplanePlayer->setPosition(D3DXVECTOR3(0.0f, 10.0f, -50.0f));

	// ī�޶� ��ü�� �÷��̾� ��ü�� �����Ѵ�.
	//m_ppPlayers[0]->setCamera(camera);
	//m_ppPlayers[0]->createShaderVariables(m_pd3dDevice);

	m_ppPlayers[0] = pAirplanePlayer;

	if (m_scene)
		m_scene->buildObjects(m_pd3dDevice);
}

// ���� ���α׷����� ����� ��ü���� ��ȯ�Ѵ�.
void CGameFramework::releaseObjects()
{
	if (m_scene)
		m_scene->releaseObjects();

	if (m_scene)
		delete m_scene;

	if (m_ppPlayers)
	{
		for (int i = 0; i < m_nPlayers; ++i)
			if (m_ppPlayers[i])
				delete m_ppPlayers[i];

		delete[] m_ppPlayers;
	}
}

// ����� �Է�(Ű���� �Է°� ���콺 �Է�)�� ó���Ѵ�.
void CGameFramework::processInput()
{
	bool bProcessedByScene = false;

	if (m_scene)
		bProcessedByScene = m_scene->processInput();
	
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDirection = 0;

		if (GetKeyboardState(pKeyBuffer))
		{
			if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
			if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
			if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
			if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
			if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
			if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
		}

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;

		if (GetCapture() == m_hWnd)
		{
			// ���콺 Ŀ���� ȭ�鿡�� ������ �ʰ� �Ѵ�.
			SetCursor(NULL);

			// ���� ���콺 Ŀ���� ��ġ�� �����´�.
			GetCursorPos(&ptCursorPos);

			// ���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;

			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}

		// �÷��̾ �̵��ϰų�(dwDirection) ȸ���Ѵ�(cxDelta, cyDelta)
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				if (pKeyBuffer[VK_RBUTTON])
					m_ppPlayers[0]->rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_ppPlayers[0]->rotate(cyDelta, cxDelta, 0.0f);
			}

			if (dwDirection)
				m_ppPlayers[0]->move(dwDirection, 500.0f * m_gameTimer.getTimeElapsed(), true);
		}
	}

	m_ppPlayers[0]->update(m_gameTimer.getTimeElapsed());
}

// ���� ��ü�� �̵��ϰų� �ִϸ��̼��� ó���Ѵ�.
void CGameFramework::animateObjects()
{
	if (m_scene)
		m_scene->animateObjects(m_gameTimer.getTimeElapsed());
}

// Direct3D �׷��� ����̽��� ���� �׷��� ����� ó���Ѵ�.
// ����� �Է� ó���� ���� processInput() �Լ�, ���� ��ü �ִϸ��̼� ó���� ���� animateObjects()�� ȣ���Ѵ�.
// �������� ���� ������ �����Ѵ�.
void CGameFramework::frameAdvacne()
{
	//if (!m_bActive)
		//return;

	// Ÿ�̸��� �ð��� ���ŵǵ��� �ϰ� ������ ����Ʈ�� �����Ѵ�.
	m_gameTimer.tick();

	processInput();
	animateObjects();

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };


	if (m_pd3dRenderTargetView)
		m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	if (m_pd3dDepthStencilView)
		m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// ���� Ÿ�� �並 ����(RGB: 0.0f, 0.125f, 0.3f)���� �����.
	//m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	CCamera* camera = nullptr;

	for (int i = 0; i < m_nPlayers; ++i)
	{

		if (m_ppPlayers[i])
		{
			m_ppPlayers[i]->updateShaderVariables(m_pd3dDeviceContext);
			camera = m_ppPlayers[i]->getCamera();
		}
		
		if (m_scene)
			m_scene->render(m_pd3dDeviceContext, camera);


		for (int j = 0; j < m_nPlayers; ++j)
		{
			if (m_ppPlayers[j])
				m_ppPlayers[j]->render(m_pd3dDeviceContext);
		}
	}
	
	// �ĸ� ���۸� ���� ���۷� ������Ʈ�Ѵ�.
	m_pDXGISwapChain->Present(0, 0);

	/*
	m_pszBuffer ���ڿ��� "LapProject ("���� �ʱ�ȭ�Ǿ����Ƿ� (m_pszBuffer+12)�������� ������ ����Ʈ�� ���ڿ��� ����Ͽ� " FPS)" ���ڿ��� ��ģ��.
	*/
	m_gameTimer.getFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer); // ������ ������ ����Ʈ�� ���ڿ��� �����ͼ� �� �������� Ÿ��Ʋ�� ����Ѵ�.
}