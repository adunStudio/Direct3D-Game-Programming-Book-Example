#pragma once

#include "Camera.h"
#include "Object.h"
#include "Timer.h"
#include "Shader.h"

class CPlayer : public CGameObject
{
protected:
	CCamera* m_camera;

	// 3인칭 카메라일 때 플레이어를 그리려고 사용하는 셰이더
	CPlayerShader* m_pShader;

	D3DXVECTOR3 m_d3dxvPosition; // 플레이어의 위치 벡터
	D3DXVECTOR3 m_d3dxvRight;    // x 축(Right)
	D3DXVECTOR3 m_d3dxvUp;       // y 축(Up)
	D3DXVECTOR3 m_d3dxvLook;     // z 축(Look)

	// 플레이어가 로컬 x, y, z 축으로 얼마만큼 회전햇는가를 나타낸다.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	// 플레이어의 이동 속도를 나타내는 벡터
	D3DXVECTOR3 m_d3dxvVelocity;

	// 플레이어에 작용하는 중력을 나타내는 벡터
	D3DXVECTOR3 m_d3dxvGravity;

	// xz 평면에서 (한 프레임 동안) 플레이어의 이동 속력의 최댓값
	float m_fMaxVelocityXZ;

	// y 축 방향으로 (한 프레임 동안) 플레이어의 이동 속력 최댓값
	float m_fMaxVelocityY;

	// 플레이어에 작용하는 마찰력
	float m_fFriction;

	// 플레이어의 위치가 바뀔 때마다 호출되는 onPlayerUpdate() 함수에서 사용하는 데이터
	LPVOID m_pPlayerUpdatedContext;

	// 카메라의 위치가 바뀔 때마다 호출되는 onCameraUpdated() 함수에서 사용하는 데이터
	LPVOID m_pCameraUpdatedContext;

public:
	CPlayer();
	~CPlayer();

	inline void     setCamera(CCamera* pCamera) { m_camera = pCamera; };
	inline CCamera* getCamera()                 { return m_camera; };

	void createShaderVariables(ID3D11Device* pd3dDevice);
	void updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext);

	inline D3DXVECTOR3 getPosition() { return m_d3dxvPosition; };
	inline D3DXVECTOR3 getLookVector() { return m_d3dxvLook; };
	inline D3DXVECTOR3 getUpVector() { return m_d3dxvUp; };
	inline D3DXVECTOR3 getRightVector() { return m_d3dxvRight; };

	inline void setFriction(float fFriction) { m_fFriction = fFriction; };
	inline void setGravity(const D3DXVECTOR3& d3dxvGravity) { m_d3dxvGravity = d3dxvGravity; };
	inline void setMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; };
	inline void setMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; };
	inline void setVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; };
	inline const D3DXVECTOR3& getVelocity() const { return m_d3dxvVelocity; };

	inline float getYaw()   const { return m_fYaw; };
	inline float getPitch() const { return m_fPitch; };
	inline float getRoll()  const { return m_fRoll; };

	inline void setPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; };
	inline void setCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; };

	// 플레이어를 이동하는 함수
	inline void setPosition(const D3DXVECTOR3& d3dxvPosition) { move((d3dxvPosition - m_d3dxvPosition), false); };
	void move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void move(const D3DXVECTOR3& d3dxvShift, bool bVelocity = false);
	void move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	// 플레이어를 회전하는 함수
	void rotate(float x, float y, float z);

	// 플레이어의 위치와 회전 정보를 경과 시간에 따라 갱신하는 함수
	void update(float fTimeElapsed);

	// 플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수
	void regenerateWorldMatrix();

	// 카메라를 변경할 때 호출되는 함수
	CCamera* onChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	
	virtual void changeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);

	// 플레이어의 카메라가 3인칭 카메라일 때 플레이어 메시를 렌더링한다.
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// 플레이어의 위치가 바뀔 때마다 호출되는 함수
	virtual void onPlayerUpdated(float fTimeElapsed);

	// 카메라의 위치가 바뀔 때마다 호출되는 함수
	virtual void onCameraUpdated(float fTimeElapsed);
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer(ID3D11Device *pd3dDevice);
	~CAirplanePlayer();

	virtual void changeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void render(ID3D11DeviceContext *pd3dDeviceContext);
};