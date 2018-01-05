#include "stdafx.h"
#include "Object.h"


CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_mesh = nullptr;
	m_references = 1;
}

CGameObject::~CGameObject()
{
	if (m_mesh)
		m_mesh->release();
}

void CGameObject::addRef()
{
	m_references++;
}

void CGameObject::release()
{
	if (m_references > 0)
		m_references--;

	if (m_references <= 0)
		delete this;
}

// 객체에 메시를 연결하기 위한 함수이다.
// 이미 연결된 메시가 있으면 연결된 메시의 참조 카운터를 감소시킨다.
// 새로운 메시를 연결하고 새로운 메시의 참조 카운터를 증가시킨다.
void CGameObject::setMesh(CMesh* mesh)
{
	if (m_mesh)
		m_mesh->release();

	m_mesh = mesh;

	if (m_mesh)
		m_mesh->addRef();
}

void CGameObject::animate(float fTimeElapsed)
{

}

// 객체를 렌더링하는 것은 객체에 연결된 메시를 렌더링하는 것이다.
void CGameObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_mesh)
	{		
		m_mesh->render(pd3dDeviceContext);
	}
}

// 객체의 위치를 설정한다.
void CGameObject::setPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}

void CGameObject::setPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxWorld._41 = d3dxvPosition.x;
	m_d3dxmtxWorld._42 = d3dxvPosition.y;
	m_d3dxmtxWorld._43 = d3dxvPosition.z;
}

// 로컬 x 축 방향으로 이동한다.
void CGameObject::moveStrafe(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvRight = getRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::setPosition(d3dxvPosition);
}
// 로컬 y 축 방향으로 이동한다.
void CGameObject::moveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvUp = getUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::setPosition(d3dxvPosition);
}
// 로컬 z 축 방향으로 이동한다.
void CGameObject::moveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvLookAt = getLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::setPosition(d3dxvPosition);
}

// 로컬 x, y, z 축으로 회전한다.
void CGameObject::rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}
void CGameObject::rotate(D3DXVECTOR3* pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}



/* CTriangleObject */

CTriangleObject::CTriangleObject()
{
	m_fRotationSpeed = 15.0f;
}

CTriangleObject::~CTriangleObject()
{
}

void CTriangleObject::animate(float fTimeElapsed)
{
	CGameObject::rotate(&m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CTriangleObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CGameObject::render(pd3dDeviceContext);
}