#pragma once

#include "Mesh.h"

#define DIR_FORWARD  0x01
#define DIR_BACKWARD 0x02
#define DIR_LEFT     0x04
#define DIR_RIGHT    0x08
#define DIR_UP       0x10
#define DIR_DOWN     0x20

// 게임의 객체를 표현하기 위한 클래스
class CGameObject
{
private:
	int m_references;

public:
	D3DXMATRIX m_d3dxmtxWorld; // 월드 변환 행렬
	CMesh* m_mesh;             // 같은 종류의 객체들이 하나의 메시를 공유할 수 있도록 메시(Mesh)에 대한 포인터로 선언

	// 자전 속도와 회전축 벡터를 나타내는 멤버 변수
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

	// 객체의 위치를 설정한다.
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(D3DXVECTOR3 d3dxvPosition);

	// 로컬 x 축 방향으로 이동한다.
	virtual void moveStrafe(float fDistance = 1.0f);
	// 로컬 y 축 방향으로 이동한다.
	virtual void moveUp(float fDistance = 1.0f);
	// 로컬 z 축 방향으로 이동한다.
	virtual void moveForward(float fDistance = 1.0f);

	// 로컬 x, y, z 축으로 회전한다.
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

	// 자전 속도와 회전축 벡터를 설정하는 함수
	inline void setRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	inline void setRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }
};