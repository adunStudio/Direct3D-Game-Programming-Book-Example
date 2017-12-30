#include "stdafx.h"
#include "GameFramework.h"


CGameFramework::CGameFramework()
{
	/*
	Direct3D 디바이스, 스왑 체인, 디바이스 컨텍스트, 그리고 렌더 타깃 뷰 인터페이스 포인터를 nullptr로 초기화한다.
	*/

	// 디바이스 인터페이스에 대한 포인터다.  주로 리소스를 생성하는 데 필요하다.
	m_pd3dDevice = nullptr;

	// 스왑 체인 인터페이스에 대한 포인터다. 주로 디스플레이를 제어하는 데 필요하다.
	m_pDXGISwapChain = nullptr;

	// 디바이스 컨텍스트에 대한 포인터다.    주로 파이프라인을 설정하는 데 필요하다.
	m_pd3dDeviceContext = nullptr;

	// 렌더 타깃 뷰 인터페이스에 대한 포인터이다.
	m_pd3dRenderTargetView = nullptr;

	m_nWndClientWidth  = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;
}

CGameFramework::~CGameFramework()
{
}

// 주 윈도우가 생성될 떄 호출되는 함수, 이 함수에서 Direct3D와 관련한 초기화를 한다.
// Direct3D 디스플레이를 생성하는 함수 createDirect3DDisplay()와 게임 객체를 생성하는 함수 buildObjects()를 호출한다.
bool CGameFramework::onCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	/*
	Direct3D를 초기화하는 과정은 크게 두 가지 단계로 이루어진다.

	1. Direct3D 디바이스, 컨텍스, 스왑 체인 생성하기

	2. 렌더 타겟 뷰 생성하기
	*/

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	// Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인등을 생성하는 함수를 호출한다.
	if (!createDirect3DDisplay())
		return false;

	// 렌더링할 객체를 생성한다.
	buildObjects();

	return true;
}

// 주 윈도우가 파괴될 때 호출되는 함수, 이 함수에서 Direct3D와 관련한 해제를 한다.
void CGameFramework::onDestroy()
{
	// 게임 객체를 소멸한다.
	releaseObjects();

	if (m_pd3dDeviceContext)    m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pDXGISwapChain)       m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext)    m_pd3dDeviceContext->Release();
	if (m_pd3dDevice)			m_pd3dDevice->Release();
}

// 렌더 타겟뷰를 생성하는 함수
// 이 함수는 스왑 체인의 첫 번째 후면 버퍼에 대한 렌더 타겟 뷰를 생성하고 디바이스 컨텍스트에 연결한다.
bool CGameFramework::createRenderTargetView()
{
	HRESULT hResult = S_OK;

	// 스왑 체인의 후면 버퍼에 대한 렌더 타겟 뷰를 생성한다.
	ID3D11Texture2D* pd3dBackBuffer;

	// 스왑 체인의 첫 번째 후면 버퍼 인터페이스를 가져온다.
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pd3dBackBuffer)))
		return false;

	// 스왑 체인의 첫 번째 후면 버퍼에 대한 렌더 타겟 뷰를 생성한다.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, nullptr, &m_pd3dRenderTargetView)))
		return false;

	if (pd3dBackBuffer)
		pd3dBackBuffer->Release();

	// 렌더 타겟 뷰를 생성하고 디바이스 컨텍스트의 출력 병합 단계에 연결한다.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, nullptr);

	return true;
}

// Direct3D와 관련한 초기화(디바이스 생성 등)를 하는 함수, onCreate() 함수에서 호출된다.
// Direct3D 디바이스를 생성하면서 스왑 체인, 디바이스 컨텍스트, 렌더 타겟 뷰를 생성하고 설정한다.
bool CGameFramework::createDirect3DDisplay()
{
	/* 먼저, 출력 윈도우의 클라이언트 영역의 크기를 멤버 변수에 저장한다. */
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
		// 디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
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

	// 생성할 스왑 체인을 서술하는 구조체다.
	/*
	스왑 체인을 생성하기 위한 스왑 체인 구조체 DXGI_SWAP_CHAIN_DESC를 설정한다.
	스왑 체인의 버퍼 크기는 주 윈도우 클라이언트 영역의 크기로 설정하고 출력 윈도우는 주 윈도우로 설정한다.
	스왑 체인의 출력 모드는 윈도우 모드로 설정한다.
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
	원하는 디바이스 드라이버 유형 순서로 스왑 체인과 디바이스, 디바이스 컨텍스트를 생성하기 위해 드라이버 유형 배열(d3dDriverTypes[])의
	각 원소에 대하여 디바이스와 스왑 체인 생성을 D3D11CreateDeviceAndSwapChain() 함수 호출을 통해 시도한다.
	*/
	D3D_DRIVER_TYPE   nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;

	for (UINT i = 0; i < nDriverTypes; ++i)
	{
		nd3dDriverType = d3dDriverTypes[i];

		/* 
		D3D11CreateDeviceAndSwapChain() 함수를 통하여 Direct3D 디바이스, 스왑 체인, 디바이스 컨텍스트를 동시에 생성한다. 
		먼저, 위에서 디바이스 생성에 필요한 디바이스 생성 플래그, 드라이버 유형, 특성 레벨(Feature Level)를 설정한다.
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

// 윈도우 메시지를 처리한다.
LRESULT CALLBACK CGameFramework::onProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		// 클라이언트 영역의 크기를 저장한다.
		m_nWndClientWidth  = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
		
		// 스왑 체인의 후면 버퍼를 사용하는 모든 인터페이스를 소멸한다.
		m_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		
		if (m_pd3dRenderTargetView)
			m_pd3dRenderTargetView->Release();

		m_pDXGISwapChain->ResizeBuffers(2, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		// 현재 클라이언트 영역의 크기에 맞는 새로운 렌더 타겟 뷰를 생성한다.
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

// 윈도우 마우스, 키보드 입력 메시지를 처리한다.
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

// 게임 프로그램에서 사용하는 객체들을 생성한다.
void CGameFramework::buildObjects()
{

}

// 게임 프로그램에서 사용한 객체들을 반환한다.
void CGameFramework::releaseObjects()
{

}

// 사용자 입력(키보드 입력과 마우스 입력)을 처리한다.
void CGameFramework::processInput()
{

}

// 게임 객체를 이동하거나 애니메이션을 처리한다.
void CGameFramework::animateObjects()
{

}

// Direct3D 그래픽 디바이스에 대한 그래픽 출력을 처리한다.
// 사용자 입력 처리를 위한 processInput() 함수, 게임 객체 애니메이션 처리를 위한 animateObjects()를 호출한다.
// 렌더링을 위한 로직을 구현한다.
void CGameFramework::frameAdvacne()
{
	processInput();
	animateObjects();

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };

	// 렌더 타겟 뷰를 색상(RGB: 0.0f, 0.125f, 0.3f)으로 지운다.
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	// 후면 버퍼를 전면 버퍼로 프리젠트한다.
	m_pDXGISwapChain->Present(0, 0);
}