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

// ��ü�� �޽ø� �����ϱ� ���� �Լ��̴�.
// �̹� ����� �޽ð� ������ ����� �޽��� ���� ī���͸� ���ҽ�Ų��.
// ���ο� �޽ø� �����ϰ� ���ο� �޽��� ���� ī���͸� ������Ų��.
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

// ��ü�� �������ϴ� ���� ��ü�� ����� �޽ø� �������ϴ� ���̴�.
void CGameObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_mesh)
	{		
		m_mesh->render(pd3dDeviceContext);
	}
}

// ��ü�� ��ġ�� �����Ѵ�.
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

// ���� x �� �������� �̵��Ѵ�.
void CGameObject::moveStrafe(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvRight = getRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::setPosition(d3dxvPosition);
}
// ���� y �� �������� �̵��Ѵ�.
void CGameObject::moveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvUp = getUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::setPosition(d3dxvPosition);
}
// ���� z �� �������� �̵��Ѵ�.
void CGameObject::moveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = getPosition();
	D3DXVECTOR3 d3dxvLookAt = getLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::setPosition(d3dxvPosition);
}

// ���� x, y, z ������ ȸ���Ѵ�.
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