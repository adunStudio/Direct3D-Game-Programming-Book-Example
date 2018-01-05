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
	// ī�޶��� ��ġ(���� ��ǥ��) ����
	D3DXVECTOR3 m_d3dxvPosition;

	// ī�޶��� ���� x��(Right), y��(Up), z��(Look)�� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;

	// ī�޶� x ��, z ��, y ������ �󸶸�ŭ ȸ���ߴ����� ��Ÿ���� ����
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;

	// ī�޶��� ����
	DWORD m_nMode;

	// ī�޶� �ٶ󺸴� ��(���� ��ǥ��)�� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvLookAtWorld;

	// �÷��̾�� ī�޶��� �������� ��Ÿ���� ����. (�ַ� 3��Ī ī�޶󿡼� ���)
	D3DXVECTOR3 m_d3dxvOffset;

	// �÷��̾ ȸ���� �� �󸶸�ŭ�� �ð��� ������Ű�� ī�޶� ȸ����ų ���ΰ��� ��Ÿ����.
	float m_fTimeLag;


	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	// ����Ʈ�� ��Ÿ���� ��� ����
	D3D11_VIEWPORT m_d3dViewport;

	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ��� ���� �������̽� ������
	ID3D11Buffer* m_pd3dcbCamera;

	// �÷��̾� ��ü�� ���� ������
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


	// ����Ʈ�� �����ϴ� �Լ�
	void setViewport(ID3D11DeviceContext* pd3dDeviceConetxt, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	inline D3D11_VIEWPORT getViewport() { return m_d3dViewport; };

	inline D3DXMATRIX    getViewMatrix() { return m_d3dxmtxView; };
	inline D3DXMATRIX    getProjectionMatrix() { return m_d3dxmtxProjection; };
	inline ID3D11Buffer* getCameraConstantBuffer() { return m_pd3dcbCamera; };

	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����ϴ� �Լ�
	void generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void generateViewMatrix();
	void regenerateViewMatrix();
	void generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// ��� ���۸� �����ϰ� ������ �����ϴ� �Լ�
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