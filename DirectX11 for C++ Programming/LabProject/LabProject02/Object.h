#pragma once

#include "Mesh.h"

#define DIR_FORWARD  0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT     0x04
#define DIR_RIGHT    0x08
#define DIR_UP       0x10
#define DIR_DOWN     0x20

// ������ ��ü�� ǥ���ϱ� ���� Ŭ����
class CGameObject
{
private:
	int m_references;

public:
	D3DXMATRIX m_d3dxmtxWorld; // ���� ��ȯ ���
	CMesh* m_mesh;             // ���� ������ ��ü���� �ϳ��� �޽ø� ������ �� �ֵ��� �޽�(Mesh)�� ���� �����ͷ� ����

	// ���� �ӵ��� ȸ���� ���͸� ��Ÿ���� ��� ����
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;

public:
	CGameObject();
	virtual ~CGameObject();

	void addRef();
	void release();

	virtual void setMesh(CMesh* mesh);
	virtual void animate(float fTimeElapsed);
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// ��ü�� ��ġ�� �����Ѵ�.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(D3DXVECTOR3 d3dxvPosition);

	// ���� x �� �������� �̵��Ѵ�.
	virtual void moveStrafe(float fDistance = 1.0f);
	// ���� y �� �������� �̵��Ѵ�.
	virtual void moveUp(float fDistance = 1.0f);
	// ���� z �� �������� �̵��Ѵ�.
	virtual void moveForward(float fDistance = 1.0f);

	// ���� x, y, z ������ ȸ���Ѵ�.
	virtual void rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void rotate(D3DXVECTOR3* pd3dxvAxis, float fAngle);

	inline D3DXVECTOR3 getPosition() { return D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43); };
	inline D3DXVECTOR3 getLookAt() { D3DXVECTOR3 d3dxvLookAt = D3DXVECTOR3(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33); D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt); return d3dxvLookAt; };
	inline D3DXVECTOR3 getUp() { D3DXVECTOR3 d3dxvUp = D3DXVECTOR3(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23); D3DXVec3Normalize(&d3dxvUp, &d3dxvUp); return d3dxvUp; };
	inline D3DXVECTOR3 getRight() { D3DXVECTOR3 d3dxvRight = D3DXVECTOR3(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13); D3DXVec3Normalize(&d3dxvRight, &d3dxvRight); return d3dxvRight; };
};

class CTriangleObject : public CGameObject
{
public:
	CTriangleObject();
	virtual ~CTriangleObject();

	virtual void animate(float fTimeElapsed);
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// ���� �ӵ��� ȸ���� ���͸� �����ϴ� �Լ�
	inline void setRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	inline void setRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }
};