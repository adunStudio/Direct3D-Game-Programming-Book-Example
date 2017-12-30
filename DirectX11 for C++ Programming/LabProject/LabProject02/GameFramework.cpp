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

	m_nWndClientWidth  = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;
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

	if (!createRenderTargetView())
		return false;

	return true;
}

// ������ �޽����� ó���Ѵ�.
LRESULT CALLBACK CGameFramework::onProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		// Ŭ���̾�Ʈ ������ ũ�⸦ �����Ѵ�.
		m_nWndClientWidth  = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
		
		// ���� ü���� �ĸ� ���۸� ����ϴ� ��� �������̽��� �Ҹ��Ѵ�.
		m_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		
		if (m_pd3dRenderTargetView)
			m_pd3dRenderTargetView->Release();

		m_pDXGISwapChain->ResizeBuffers(2, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		// ���� Ŭ���̾�Ʈ ������ ũ�⿡ �´� ���ο� ���� Ÿ�� �並 �����Ѵ�.
		createRenderTargetView();
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
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
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

}

// ���� ���α׷����� ����� ��ü���� ��ȯ�Ѵ�.
void CGameFramework::releaseObjects()
{

}

// ����� �Է�(Ű���� �Է°� ���콺 �Է�)�� ó���Ѵ�.
void CGameFramework::processInput()
{

}

// ���� ��ü�� �̵��ϰų� �ִϸ��̼��� ó���Ѵ�.
void CGameFramework::animateObjects()
{

}

// Direct3D �׷��� ����̽��� ���� �׷��� ����� ó���Ѵ�.
// ����� �Է� ó���� ���� processInput() �Լ�, ���� ��ü �ִϸ��̼� ó���� ���� animateObjects()�� ȣ���Ѵ�.
// �������� ���� ������ �����Ѵ�.
void CGameFramework::frameAdvacne()
{
	processInput();
	animateObjects();

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	// ���� Ÿ�� �並 ����(RGB: 0.0f, 0.125f, 0.3f)���� �����.
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	// �ĸ� ���۸� ���� ���۷� ������Ʈ�Ѵ�.
	m_pDXGISwapChain->Present(0, 0);
}