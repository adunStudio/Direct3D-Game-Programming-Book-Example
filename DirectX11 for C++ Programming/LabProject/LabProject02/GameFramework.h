#pragma once

#include "Timer.h"
#include "Scene.h"

class CGameFramework
{
private:
	/*
	멤버 변수 선언

	응용 프로그램의 인스턴스 핸들과 주 윈도우 핸들을 저장하기 위한 멤버 변수를 선언한다.
	*/

	// 현재 실행중인 윈도우 프로그램의 인스턴스 핸들
	HINSTANCE m_hInstance;

	// 주 윈도우의 핸들
	HWND m_hWnd;

	// 주 윈도우의 메뉴 핸들
	HMENU m_hMenu;

	/*
	응용 프로그램의 주 윈도우의 클라이언트 영역의 크기를 저장하기 위한 멤버 변수를 선언한다.
	*/

	// 주 윈도우의 클라이언트 영역의 가로와 세로 크기
	int m_nWndClientWidth;
	int m_nWndClientHeight;

	/*

	DirectX 인터페이스 객체의 멤버 변수 선언

	DirectX 그래픽 프로그램은 기본적으로 다음과 같은 인터페이스 객체가 필요하다.

	- Direct3D 디바이스(Device)         인터페이스
	- 스왑 체인(Swap Chain)             인터페이스
	- 디바이스 컨텍스트(Device Context) 인터페이스
	- 렌더 타겟 뷰(Render Target View)  인터페이스

	기본적으로 Direct3D 디바이스 인터페이스는 버퍼(정점 버퍼, 인덱스 버퍼, 상수 버퍼 등)의 생성, 리소스(텍스처 등)의 생성, 셰이더 생성, 래스터라이저 상태 생성, 리소스 뷰의 생성 등을 필요하다.

	스왑 체인 인터페이스는 후면 버퍼의 내용을 출력하기 위해, 즉 프레젠테이션을 하는데 필요하다.

	디바이스 컨텍스트 인터페이스는 렌더링과 관련된 작업을 위해 필요하다.

	*/

	// 디바이스 인터페이스에 대한 포인터다. 주로 리소스를 생성하는 데 필요하다.
	ID3D11Device* m_pd3dDevice;

	// 스왑 체인 인터페이스에 대한 포인터다. 주로 디스플레이를 제어하는 데 필요하다.
	IDXGISwapChain* m_pDXGISwapChain;

	// 디바이스 컨텍스트에 대한 포인터다. 주로 파이프라인을 설정하는 데 필요하다.
	ID3D11DeviceContext* m_pd3dDeviceContext;

	// 렌더 타깃 뷰 인터페이스에 대한 포인터이다.
	ID3D11RenderTargetView* m_pd3dRenderTargetView;

	// 게임 프레임워크에서 사용할 타이머다.
	CGameTimer m_gameTimer;

	// 게임의 장면(Scene)을 관리하는 객체에 대한 포인터다.
	CScene* m_scene;

	// 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다.
	_TCHAR  m_pszBuffer[50];

public:
	CGameFramework();
	~CGameFramework();

	// 주 윈도우가 생성될 떄 호출되는 함수, 이 함수에서 Direct3D와 관련한 초기화를 한다.
	// Direct3D 디스플레이를 새엇ㅇ하는 함수 createDirect3DDisplay()와 게임 객체를 생성하는 함수 buildObjects()를 호출한다.
	bool onCreate(HINSTANCE hInstance, HWND hMainWnd);
	
	// 주 윈도우가 파괴될 때 호출되는 함수, 이 함수에서 Direct3D와 관련한 해제를 한다.
	void onDestroy();


	// 렌더 타겟뷰를 생성하는 함수
	// 이 함수는 스왑 체인의 첫 번째 후면 버퍼에 대한 렌더 타겟 뷰를 생성하고 디바이스 컨텍스트에 연결한다.
	bool createRenderTargetView();
	
	// Direct3D와 관련한 초기화(디바이스 생성 등)를 하는 함수, onCreate() 함수에서 호출된다.
	// Direct3D 디바이스를 생성하면서 스왑 체인, 디바이스 컨텍스트, 렌더 타겟 뷰를 생성하고 설정한다.
	bool createDirect3DDisplay();


	// 게임 프로그램에서 사용하는 객체들을 생성한다.
	void buildObjects();

	// 게임 프로그램에서 사용한 객체들을 반환한다.
	void releaseObjects();

	
	// 사용자 입력(키보드 입력과 마우스 입력)을 처리한다.
	void processInput();

	// 게임 객체를 이동하거나 애니메이션을 처리한다.
	void animateObjects();

	// Direct3D 그래픽 디바이스에 대한 그래픽 출력을 처리한다.
	// 사용자 입력 처리를 위한 processInput() 함수, 게임 객체 애니메이션 처리를 위한 animateObjects()를 호출한다.
	// 렌더링을 위한 로직을 구현한다.
	void frameAdvacne();

	// 윈도우 마우스, 키보드 입력 메시지를 처리한다.
	void onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	// 윈도우 메시지를 처리한다.
	LRESULT CALLBACK onProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

