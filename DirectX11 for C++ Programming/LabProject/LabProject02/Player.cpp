#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
	m_camera = nullptr;
	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight    = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook     = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvGravity  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fFriction = 0.0f;
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY  = 0.0f;
	m_fFriction = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = nullptr;
	m_pCameraUpdatedContext = nullptr;

	//m_pShader = nullptr;
}

CPlayer::~CPlayer()
{
	if (m_camera)
		delete m_camera;
}

void CPlayer::createShaderVariables(ID3D11Device* pd3dDevice)
{

}

void CPlayer::updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_camera)
		m_camera->updateSHaderVariables(pd3dDeviceContext);
}

// �÷��̾��� ��ġ�� �����ϴ� �Լ�
void CPlayer::move(ULONG nDirection, float fDistance, bool bVelocity)
{
	// �÷��̾��� ��ġ�� �⺻������ ����ڰ� �÷��̾ �̵��Ϸ��� Ű���带 ���� �� ����ȴ�.
	// �̵� ����(dwDirection)�� ���� �÷��̾ fDistance��ŭ �̵���Ų��.
	if (nDirection)
	{
		D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);

		// [��] Ű�� ������ ���� z �� �������� �̵�
		if (nDirection & DIR_FORWARD)
			d3dxvShift += m_d3dxvLook * fDistance;
		// [��] Ű�� ������ ���� z �� �ݴ� �������� �̵�
		if(nDirection & DIR_BACKWARD)
			d3dxvShift -= m_d3dxvLook * fDistance;

		// [��] Ű�� ������ ���� x �� �������� �̵�
		if(nDirection * DIR_RIGHT)
			d3dxvShift += m_d3dxvRight * fDistance;
		// [��] Ű�� ������ ���� x �� �ݴ� �������� �̵�
		if (nDirection * DIR_LEFT)
			d3dxvShift -= m_d3dxvRight * fDistance;

		// [Page Up] Ű�� ������ ���� y �� �������� �̵�
		if (nDirection * DIR_UP)
			d3dxvShift += m_d3dxvUp * fDistance;
		// [Page Down] Ű�� ������ ���� y �� �ݴ� �������� �̵�
		if (nDirection * DIR_DOWN)
			d3dxvShift -= m_d3dxvUp * fDistance;


		// �÷��̾ ���� ��ġ ���Ϳ��� d3dxvShift ���� ��ŭ �̵�
		move(d3dxvShift, bVelocity);
	}

}
void CPlayer::move(const D3DXVECTOR3& d3dxvShift, bool bVelocity)
{
	// bVelocity�� ture�� �÷��̾ �̵����� �ʰ� �ӵ� ���͸� �����Ѵ�.
	if (bVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		// �÷��̾ ���� ��ġ ���Ϳ��� d3dxvShift ���͸�ŭ �̵��Ѵ�.
		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
		m_d3dxvPosition = d3dxvPosition;
		regenerateWorldMatrix();

		// �÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ��ġ�� d3dxvShift ���͸�ŭ �̵��Ѵ�.
		m_camera->move(d3dxvShift);
	}
}
void CPlayer::move(float fxOffset, float fyOffset, float fzOffset)
{

}

// �÷��̾ ȸ���ϴ� �Լ�
void CPlayer::rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_camera->getMode();

	// 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ �ٸ���.
	if (nCurrentCameraMode == FIRST_PERSON_CAMERA || nCurrentCameraMode == THIRD_PERSON_CAMERA)
	{
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw > 0.0f)   m_fYaw += 360.0f;
		}

		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { x -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { x -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}

		// ī�޶� x, y, z ��ŭ ȸ���Ѵ�. �÷��̾ ȸ���ϸ� ī�޶� ȸ���ϰ� �ȴ�.
		m_camera->rotate(x, y, z);

		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		// �����̽� �� ī�޶󿡼� �÷��̾��� ȸ���� ȸ�� ������ ������ ����.
		// �׸��� ��� ���� �߽����� ȸ���� �� �ִ�.
		m_camera->rotate(x, y, z);

		if (x != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		}

		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}

		if (z != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvLook, (float)D3DXToRadian(z));
			D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}

	// ȸ������ ���� �÷��̾��� ���� x, y, z ���� ���� �������� ���� �� �����Ƿ� z��(LookAt ����)�� �������� �Ͽ� ���� �����ϰ� ���� ���Ͱ� �ǵ��� �Ѵ�.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	regenerateWorldMatrix();
}

// �÷��̾��� ��ġ�� ȸ�� ������ ��� �ð��� ���� �����ϴ� �Լ�
void CPlayer::update(float fTimeElapsed)
{
	m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	// XZ ����� �ִ� �ӷº��� ũ�� �ӵ� ������ x�� z���� ������ �����Ѵ�.
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);

	if (fLength > m_fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	}

	// Y ������ ũ�⸦ ���Ѵ�. �̰��� y �� ������ �ִ� �ӷº��� ũ�� �ӵ� ������ y���� ������ �����Ѵ�.
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);

	if (fLength > m_fMaxVelocityY)
		m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	// �÷��̾ �ӵ� ���͸�ŭ �̵��Ѵ�.
	move(m_d3dxvVelocity * fTimeElapsed, false);

	// �÷��̾��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�.
	if (m_pPlayerUpdatedContext)
		onPlayerUpdated(fTimeElapsed);

	DWORD nCurrentCameraMode = m_camera->getMode();

	// �÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ���¸� �����Ѵ�.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		m_camera->update(fTimeElapsed);

	// ī�޶��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�.
	if (m_pCameraUpdatedContext)
		onCameraUpdated(fTimeElapsed);

	// ī�޶� 3��Ī ī�޶��̸� ī�޶� ����� �÷��̾� ��ġ�� �ٶ󺸵��� �Ѵ�.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		m_camera->setLookAt(m_d3dxvPosition);

	// ī�޶��� ī�޶� ��ȯ ����� �ٽ� �����Ѵ�.
	m_camera->regenerateViewMatrix();


	// �÷��̾��� �ӵ� ���Ͱ� ������ ������ ������ �Ǿ�� �Ѵٸ� ���� ���͸� �����Ѵ�.
	D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	fLength = D3DXVec3Length(&m_d3dxvVelocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength)
		fDeceleration = fLength;
	m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

// �÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ�
void CPlayer::regenerateWorldMatrix()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x; m_d3dxmtxWorld._12 = m_d3dxvRight.y; m_d3dxmtxWorld._13 = m_d3dxvRight.z;
	m_d3dxmtxWorld._21 = m_d3dxvUp.x; m_d3dxmtxWorld._22 = m_d3dxvUp.y; m_d3dxmtxWorld._23 = m_d3dxvUp.z;
	m_d3dxmtxWorld._31 = m_d3dxvLook.x; m_d3dxmtxWorld._32 = m_d3dxvLook.y; m_d3dxmtxWorld._33 = m_d3dxvLook.z;
	m_d3dxmtxWorld._41 = m_d3dxvPosition.x; m_d3dxmtxWorld._42 = m_d3dxvPosition.y; m_d3dxmtxWorld._43 = m_d3dxvPosition.z;
}

// ī�޶� ������ �� ȣ��Ǵ� �Լ�
CCamera* CPlayer::onChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	// ���ο� ī�޶��� ��忡 ���� ī�޶� ���� �����Ѵ�.
	CCamera* newCamera = nullptr;
	
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		newCamera = new CFirstPersonCamera(m_camera);
		break;

	case THIRD_PERSON_CAMERA:
		newCamera = new CThirdPersonCamera(m_camera);
		break;

	case SPACESHIP_CAMERA:
		newCamera = new CSpaceShipCamera(m_camera);
		break;
	}

	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
		D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		// Look ���Ϳ� ���� ��ǥ���� z��(0, 0, 1)�� �̷�� ����(����=cos)�� ����Ͽ� �÷��̾��� y ���� ȸ�� ���� m_fYaw�� ����Ѵ�.
		m_fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_d3dxvLook)));
		if (m_d3dxvLook.x < 0.0f)
			m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_camera)
	{
		m_d3dxvRight = m_camera->getRightVector();
		m_d3dxvUp = m_camera->getUpVector();
		m_d3dxvLook = m_camera->getLookVector();
	}

	if (newCamera)
	{
		// ���� ī�޶� ������ ���ο� ī�޶� ���� ���̴� ������ �����Ѵ�.
		if (!m_camera)
			newCamera->createShaderVariables(pd3dDevice);

		newCamera->setMode(nNewCameraMode);

		// ���� ī�޶� ����ϴ� �÷��̾� ��ü�� �����Ѵ�.
		newCamera->setPlayer(this);
	}

	if (m_camera)
		delete m_camera;

	return newCamera;
}

void CPlayer::changeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{

}

// �÷��̾��� ī�޶� 3��Ī ī�޶��� �� �÷��̾� �޽ø� �������Ѵ�.
void CPlayer::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->updateShaderVariables(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->render(pd3dDeviceContext);
	}

	if (m_mesh)
		m_mesh->render(pd3dDeviceContext);
}

// �÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ�
void CPlayer::onPlayerUpdated(float fTimeElapsed)
{

}

// ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ�
void CPlayer::onCameraUpdated(float fTimeElapsed)
{

}


/* CAirplanePlayer */
CAirplanePlayer::CAirplanePlayer(ID3D11Device *pd3dDevice)
{
	CMesh* pAirPlaneMesh = new CAirPlaneMesh(pd3dDevice, 20.0f, 20.0f, 20.0f, 4.0f, D3DCOLOR_XRGB(0, 255, 0));

	setMesh(pAirPlaneMesh);

	m_pShader = new CPlayerShader();
	m_pShader->createShader(pd3dDevice);
	m_pShader->createShaderVariables(pd3dDevice);

	// �÷��̾ ���� ���̴� ������ �����Ѵ�.
	createShaderVariables(pd3dDevice);
}

CAirplanePlayer::~CAirplanePlayer()
{
	if (m_pShader)
		delete m_pShader;
}

void CAirplanePlayer::changeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = m_camera ? m_camera->getMode() : 0x00;

	if (nCurrentCameraMode == nNewCameraMode)
		return;

	switch (nNewCameraMode)
	{
	// �÷��̾��� Ư���� 1��Ī ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
	case FIRST_PERSON_CAMERA:
		setFriction(200.0f);
		setGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		setMaxVelocityXZ(125.0f);
		setMaxVelocityY(400.0f);
		m_camera = onChangeCamera(pd3dDevice, FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_camera->setTimeLag(0.0f);
		m_camera->setOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		m_camera->generateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;

	// �÷��̾��� Ư���� �����̽� �� ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
	case SPACESHIP_CAMERA:
		setFriction(125.0f);
		setGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		setMaxVelocityXZ(125.0f);
		setMaxVelocityY(400.0f);
		m_camera = onChangeCamera(pd3dDevice, SPACESHIP_CAMERA, nCurrentCameraMode);
		m_camera->setTimeLag(0.25f);
		m_camera->setOffset(D3DXVECTOR3(0.0f, 20.0f, -50.0f));
		m_camera->generateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;

	// �÷��̾� Ư���� 3��Ī ī�޶� ��忡 �°� �����Ѵ�.
	// ���� ȿ���� ī�޶� �������� �����Ѵ�.
	case THIRD_PERSON_CAMERA:
		setFriction(250.0f);
		setGravity(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		setMaxVelocityXZ(125.0f);
		setMaxVelocityY(400.0f);
		m_camera = onChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_camera->setTimeLag(0.25f);
		m_camera->setOffset(D3DXVECTOR3(0.0f, 20.0f, -50.0f));
		m_camera->generateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;

	default:
		break;
	}

	// ī�޶� ������ �ð��� ���� �����Ѵ�.
	update(fTimeElapsed);
}

void CAirplanePlayer::render(ID3D11DeviceContext *pd3dDeviceContext)
{
	DWORD nCurrentCameraMode = (m_camera) ? m_camera->getMode() : 0x00;

	if ((nCurrentCameraMode == THIRD_PERSON_CAMERA) && m_mesh)
	{
		D3DXMATRIX mtxRotate;

		D3DXMatrixRotationYawPitchRoll(&mtxRotate, 0.0f, (float)D3DXToRadian(90.0f), 0.0f);

		m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;

		CPlayer::render(pd3dDeviceContext);
	}
}