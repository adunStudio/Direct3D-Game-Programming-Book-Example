#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

bool CScene::onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::buildObjects(ID3D11Device* pd3dDevice)
{
	return true;
}

void CScene::releaseObjects()
{

}

bool CScene::processInput()
{
	return false;
}

void CScene::animateObjects(float timeElapsed)
{

}

void CScene::render(ID3D11DeviceContext* pd3dDeviceContext)
{

}
