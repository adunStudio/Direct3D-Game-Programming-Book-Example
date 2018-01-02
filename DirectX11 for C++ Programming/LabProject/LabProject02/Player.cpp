#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
	m_camera = nullptr;
}


CPlayer::~CPlayer()
{
	if (m_camera)
		delete m_camera;
}

void CPlayer::createShaderVariables(ID3D11Device* pd3dDevice)
{

}

void CPlayer::updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_camera)
		m_camera->updateSHaderVariables(pd3dDeviceContext);
}