#pragma once
class CScene
{
public:
	CScene();
	~CScene();

	// ������ ���콺, Ű���� �Է� �޽����� ó���Ѵ�.
	bool onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	bool buildObjects(ID3D11Device* pd3dDevice);
	void releaseObjects();
	bool processInput();
	void animateObjects(float timeElapsed);
	void render(ID3D11DeviceContext* pd3dDeviceContext);
};
