#pragma once

#include "Timer.h"
#include "Scene.h"

class CGameFramework
{
private:
	/*
	��� ���� ����

	���� ���α׷��� �ν��Ͻ� �ڵ�� �� ������ �ڵ��� �����ϱ� ���� ��� ������ �����Ѵ�.
	*/

	// ���� �������� ������ ���α׷��� �ν��Ͻ� �ڵ�
	HINSTANCE m_hInstance;

	// �� �������� �ڵ�
	HWND m_hWnd;

	// �� �������� �޴� �ڵ�
	HMENU m_hMenu;

	/*
	���� ���α׷��� �� �������� Ŭ���̾�Ʈ ������ ũ�⸦ �����ϱ� ���� ��� ������ �����Ѵ�.
	*/

	// �� �������� Ŭ���̾�Ʈ ������ ���ο� ���� ũ��
	int m_nWndClientWidth;
	int m_nWndClientHeight;

	/*

	DirectX �������̽� ��ü�� ��� ���� ����

	DirectX �׷��� ���α׷��� �⺻������ ������ ���� �������̽� ��ü�� �ʿ��ϴ�.

	- Direct3D ����̽�(Device)         �������̽�
	- ���� ü��(Swap Chain)             �������̽�
	- ����̽� ���ؽ�Ʈ(Device Context) �������̽�
	- ���� Ÿ�� ��(Render Target View)  �������̽�

	�⺻������ Direct3D ����̽� �������̽��� ����(���� ����, �ε��� ����, ��� ���� ��)�� ����, ���ҽ�(�ؽ�ó ��)�� ����, ���̴� ����, �����Ͷ����� ���� ����, ���ҽ� ���� ���� ���� �ʿ��ϴ�.

	���� ü�� �������̽��� �ĸ� ������ ������ ����ϱ� ����, �� ���������̼��� �ϴµ� �ʿ��ϴ�.

	����̽� ���ؽ�Ʈ �������̽��� �������� ���õ� �۾��� ���� �ʿ��ϴ�.

	*/

	// ����̽� �������̽��� ���� �����ʹ�. �ַ� ���ҽ��� �����ϴ� �� �ʿ��ϴ�.
	ID3D11Device* m_pd3dDevice;

	// ���� ü�� �������̽��� ���� �����ʹ�. �ַ� ���÷��̸� �����ϴ� �� �ʿ��ϴ�.
	IDXGISwapChain* m_pDXGISwapChain;

	// ����̽� ���ؽ�Ʈ�� ���� �����ʹ�. �ַ� ������������ �����ϴ� �� �ʿ��ϴ�.
	ID3D11DeviceContext* m_pd3dDeviceContext;

	// ���� Ÿ�� �� �������̽��� ���� �������̴�.
	ID3D11RenderTargetView* m_pd3dRenderTargetView;

	// ���� �����ӿ�ũ���� ����� Ÿ�̸Ӵ�.
	CGameTimer m_gameTimer;

	// ������ ���(Scene)�� �����ϴ� ��ü�� ���� �����ʹ�.
	CScene* m_scene;

	// ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
	_TCHAR  m_pszBuffer[50];

public:
	CGameFramework();
	~CGameFramework();

	// �� �����찡 ������ �� ȣ��Ǵ� �Լ�, �� �Լ����� Direct3D�� ������ �ʱ�ȭ�� �Ѵ�.
	// Direct3D ���÷��̸� �������ϴ� �Լ� createDirect3DDisplay()�� ���� ��ü�� �����ϴ� �Լ� buildObjects()�� ȣ���Ѵ�.
	bool onCreate(HINSTANCE hInstance, HWND hMainWnd);
	
	// �� �����찡 �ı��� �� ȣ��Ǵ� �Լ�, �� �Լ����� Direct3D�� ������ ������ �Ѵ�.
	void onDestroy();


	// ���� Ÿ�ٺ並 �����ϴ� �Լ�
	// �� �Լ��� ���� ü���� ù ��° �ĸ� ���ۿ� ���� ���� Ÿ�� �並 �����ϰ� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	bool createRenderTargetView();
	
	// Direct3D�� ������ �ʱ�ȭ(����̽� ���� ��)�� �ϴ� �Լ�, onCreate() �Լ����� ȣ��ȴ�.
	// Direct3D ����̽��� �����ϸ鼭 ���� ü��, ����̽� ���ؽ�Ʈ, ���� Ÿ�� �並 �����ϰ� �����Ѵ�.
	bool createDirect3DDisplay();


	// ���� ���α׷����� ����ϴ� ��ü���� �����Ѵ�.
	void buildObjects();

	// ���� ���α׷����� ����� ��ü���� ��ȯ�Ѵ�.
	void releaseObjects();

	
	// ����� �Է�(Ű���� �Է°� ���콺 �Է�)�� ó���Ѵ�.
	void processInput();

	// ���� ��ü�� �̵��ϰų� �ִϸ��̼��� ó���Ѵ�.
	void animateObjects();

	// Direct3D �׷��� ����̽��� ���� �׷��� ����� ó���Ѵ�.
	// ����� �Է� ó���� ���� processInput() �Լ�, ���� ��ü �ִϸ��̼� ó���� ���� animateObjects()�� ȣ���Ѵ�.
	// �������� ���� ������ �����Ѵ�.
	void frameAdvacne();

	// ������ ���콺, Ű���� �Է� �޽����� ó���Ѵ�.
	void onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	// ������ �޽����� ó���Ѵ�.
	LRESULT CALLBACK onProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

