#pragma once

#include "Object.h"
#include "Shader.h"
#include "Camera.h"

class CScene
{
private:
	CObjectShader** m_ppShaders;
	int m_nShaders;

	CGameObject** m_ppObjects;
	int m_nObjects;

public:
	CScene();
	~CScene();

	// ������ ���콺, Ű���� �Է� �޽����� ó���Ѵ�.
	bool onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void buildObjects(ID3D11Device* pd3dDevice);
	void releaseObjects();
	bool processInput();
	void animateObjects(float timeElapsed);
	void render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* camera = nullptr);
};

