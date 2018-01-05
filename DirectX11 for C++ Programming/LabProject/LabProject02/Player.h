#pragma once

#include "Camera.h"
#include "Object.h"
#include "Timer.h"
#include "Shader.h"

class CPlayer : public CGameObject
{
protected:
	CCamera* m_camera;

	// 3��Ī ī�޶��� �� �÷��̾ �׸����� ����ϴ� ���̴�
	CPlayerShader* m_pShader;

	D3DXVECTOR3 m_d3dxvPosition; // �÷��̾��� ��ġ ����
	D3DXVECTOR3 m_d3dxvRight;    // x ��(Right)
	D3DXVECTOR3 m_d3dxvUp;       // y ��(Up)
	D3DXVECTOR3 m_d3dxvLook;     // z ��(Look)

	// �÷��̾ ���� x, y, z ������ �󸶸�ŭ ȸ���޴°��� ��Ÿ����.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	// �÷��̾��� �̵� �ӵ��� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvVelocity;

	// �÷��̾ �ۿ��ϴ� �߷��� ��Ÿ���� ����
	D3DXVECTOR3 m_d3dxvGravity;

	// xz ��鿡�� (�� ������ ����) �÷��̾��� �̵� �ӷ��� �ִ�
	float m_fMaxVelocityXZ;

	// y �� �������� (�� ������ ����) �÷��̾��� �̵� �ӷ� �ִ�
	float m_fMaxVelocityY;

	// �÷��̾ �ۿ��ϴ� ������
	float m_fFriction;

	// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� onPlayerUpdate() �Լ����� ����ϴ� ������
	LPVOID m_pPlayerUpdatedContext;

	// ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� onCameraUpdated() �Լ����� ����ϴ� ������
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

	// �÷��̾ �̵��ϴ� �Լ�
	inline void setPosition(const D3DXVECTOR3& d3dxvPosition) { move((d3dxvPosition - m_d3dxvPosition), false); };
	void move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void move(const D3DXVECTOR3& d3dxvShift, bool bVelocity = false);
	void move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);

	// �÷��̾ ȸ���ϴ� �Լ�
	void rotate(float x, float y, float z);

	// �÷��̾��� ��ġ�� ȸ�� ������ ��� �ð��� ���� �����ϴ� �Լ�
	void update(float fTimeElapsed);

	// �÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ�
	void regenerateWorldMatrix();

	// ī�޶� ������ �� ȣ��Ǵ� �Լ�
	CCamera* onChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);
	
	virtual void changeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);

	// �÷��̾��� ī�޶� 3��Ī ī�޶��� �� �÷��̾� �޽ø� �������Ѵ�.
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ�
	virtual void onPlayerUpdated(float fTimeElapsed);

	// ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ�
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