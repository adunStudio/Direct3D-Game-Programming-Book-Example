#pragma once

#include "Camera.h"
#include "Object.h"

class CPlayer : CGameObject
{
protected:
	CCamera* m_camera;


public:
	CPlayer();
	~CPlayer();

	inline void     setCamera(CCamera* pCamera) { m_camera = pCamera; };
	inline CCamera* getCamera()                 { return m_camera; };

	void createShaderVariables(ID3D11Device* pd3dDevice);
	void updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext);
};

