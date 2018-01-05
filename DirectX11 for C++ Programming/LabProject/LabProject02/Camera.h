#pragma once

struct VS_CB_VIEWPROJECTION_MATRIX
{
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;
};

class CPlayer;

#define FIRST_PERSON_CAMERA 0x01
#define SPACESHIP_CAMERA    0x02
#define THIRD_PERSON_CAMERA 0x03

class CCamera
{
protected:
	// 카메라의 위치(월드 좌표계) 벡터
	D3DXVECTOR3 m_d3dxvPosition;

	// 카메라의 로컬 x축(Right), y축(Up), z축(Look)을 나타내는 벡터
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;

	// 카메라가 x 축, z 축, y 축으로 얼마만큼 회전했는지를 나타내는 각도
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;

	// 카메라의 종류
	DWORD m_nMode;

	// 카메라가 바라보는 점(월드 좌표계)을 나타내는 벡터
	D3DXVECTOR3 m_d3dxvLookAtWorld;

	// 플레이어와 카메라의 오프셋을 나타내는 벡터. (주로 3인칭 카메라에서 사용)
	D3DXVECTOR3 m_d3dxvOffset;

	// 플레이어가 회전할 때 얼마만큼의 시간을 지연시키고 카메라를 회전시킬 것인가를 나타낸다.
	float m_fTimeLag;


	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	// 뷰포트를 나타내는 멤버 변수
	D3D11_VIEWPORT m_d3dViewport;

	// 카메라 변환 행렬과 투영 변환 행렬을 위한 상수 버퍼 인터페이스 포인터
	ID3D11Buffer* m_pd3dcbCamera;

	// 플레이어 객체에 대한 포인터
	CPlayer* m_player;


public:
	CCamera(CCamera* pCamera);
	~CCamera();

	inline void setPlayer(CPlayer *pPlayer) { m_player = pPlayer; };
	inline CPlayer *getPlayer() { return m_player; };
	
	inline void setMode(DWORD nMode) { m_nMode = nMode; };
	inline DWORD getMode() { return m_nMode ; };

	inline void setPosition(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; };
	inline D3DXVECTOR3& getPosition() { return m_d3dxvPosition; };

	inline void setLookAtPosition(D3DXVECTOR3 d3dxvLookAtWorld) { m_d3dxvLookAtWorld = d3dxvLookAtWorld; };
	inline D3DXVECTOR3& getLookAtPosition() { return m_d3dxvLookAtWorld; };

	inline D3DXVECTOR3& getRightVector() { return m_d3dxvRight; };
	inline D3DXVECTOR3& getUpVector() { return m_d3dxvUp; };
	inline D3DXVECTOR3& getLookVector() { return m_d3dxvLook; };

	inline float& getPitch() { return m_fPitch; };
	inline float& getRoll() { return m_fRoll; };
	inline float& getYaw() { return m_fYaw; };

	inline void setOffset(D3DXVECTOR3 d3dxvOffset) { m_d3dxvOffset = d3dxvOffset; m_d3dxvPosition += d3dxvOffset; };
	inline D3DXVECTOR3& getOffset() { return m_d3dxvOffset; };

	inline void setTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; };
	inline float getTimeLag() { return m_fTimeLag; };


	// 뷰포트를 설정하는 함수
	void setViewport(ID3D11DeviceContext* pd3dDeviceConetxt, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	inline D3D11_VIEWPORT getViewport() { return m_d3dViewport; };

	inline D3DXMATRIX    getViewMatrix() { return m_d3dxmtxView; };
	inline D3DXMATRIX    getProjectionMatrix() { return m_d3dxmtxProjection; };
	inline ID3D11Buffer* getCameraConstantBuffer() { return m_pd3dcbCamera; };

	// 카메라 변환 행렬과 투영 변환 행렬을 생성하는 함수
	void generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void generateViewMatrix();
	void regenerateViewMatrix();
	void generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// 상수 버퍼를 생성하고 내용을 갱신하는 함수
	void createShaderVariables(ID3D11Device* pd3dDevice);
	void updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext);


	virtual void move(const D3DXVECTOR3& d3dxvShift) 
	{
		m_d3dxvPosition += d3dxvShift;
	};
	virtual void rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void update(float fTimeElapsed) { }
	virtual void setLookAt(D3DXVECTOR3& vLookAt) { }
};


class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(CCamera* pCamera);

	virtual void rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};


class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera* pCamera);

	virtual void rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};


class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera* pCamera);

	virtual void update(float fTimeElapsed);
	virtual void setLookAt(D3DXVECTOR3& vLookAt);
};