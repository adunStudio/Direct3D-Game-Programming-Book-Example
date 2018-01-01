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

/* CTriangleObject */

CTriangleObject::CTriangleObject()
{

}

CTriangleObject::~CTriangleObject()
{

}

void CTriangleObject::animate(float fTimeElapsed)
{
	CGameObject::animate(fTimeElapsed);
}

void CTriangleObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CGameObject::render(pd3dDeviceContext);
}