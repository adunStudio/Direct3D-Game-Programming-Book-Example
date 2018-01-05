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

// 플레이어의 위치를 변경하는 함수
void CPlayer::move(ULONG nDirection, float fDistance, bool bVelocity)
{
	// 플레이어의 위치는 기본적으로 사용자가 플레이어를 이동하려고 키보드를 누를 때 변경된다.
	// 이동 방향(dwDirection)에 따라 플레이어를 fDistance만큼 이동시킨다.
	if (nDirection)
	{
		D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);

		// [↑] 키를 누르면 로컬 z 축 방향으로 이동
		if (nDirection & DIR_FORWARD)
			d3dxvShift += m_d3dxvLook * fDistance;
		// [↓] 키를 누르면 로컬 z 축 반대 방향으로 이동
		if(nDirection & DIR_BACKWARD)
			d3dxvShift -= m_d3dxvLook * fDistance;

		// [→] 키를 누르면 로컬 x 축 방향으로 이동
		if(nDirection * DIR_RIGHT)
			d3dxvShift += m_d3dxvRight * fDistance;
		// [←] 키를 누르면 로컬 x 축 반대 방향으로 이동
		if (nDirection * DIR_LEFT)
			d3dxvShift -= m_d3dxvRight * fDistance;

		// [Page Up] 키를 누르면 로컬 y 축 방향으로 이동
		if (nDirection * DIR_UP)
			d3dxvShift += m_d3dxvUp * fDistance;
		// [Page Down] 키를 누르면 로컬 y 축 반대 방향으로 이동
		if (nDirection * DIR_DOWN)
			d3dxvShift -= m_d3dxvUp * fDistance;


		// 플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동
		move(d3dxvShift, bVelocity);
	}

}
void CPlayer::move(const D3DXVECTOR3& d3dxvShift, bool bVelocity)
{
	// bVelocity가 ture면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		// 플레이어를 현재 위치 벡터에서 d3dxvShift 벡터만큼 이동한다.
		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
		m_d3dxvPosition = d3dxvPosition;
		regenerateWorldMatrix();

		// 플레이어의 위치가 변경되었으므로 카메라의 위치도 d3dxvShift 벡터만큼 이동한다.
		m_camera->move(d3dxvShift);
	}
}
void CPlayer::move(float fxOffset, float fyOffset, float fzOffset)
{

}

// 플레이어를 회전하는 함수
void CPlayer::rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_camera->getMode();

	// 1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 다른다.
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

		// 카메라를 x, y, z 만큼 회전한다. 플레이어를 회전하면 카메라가 회전하게 된다.
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
		// 스페이스 십 카메라에서 플레이어의 회전은 회전 각도의 제한이 없다.
		// 그리고 모든 축을 중심으로 회전할 수 있다.
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

	// 회전으로 인해 플레이어의 로컬 x, y, z 축이 서로 직교하지 않을 수 있으므로 z축(LookAt 벡터)을 기준으로 하여 서로 직교하고 단위 벡터가 되도록 한다.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	regenerateWorldMatrix();
}

// 플레이어의 위치와 회전 정보를 경과 시간에 따라 갱신하는 함수
void CPlayer::update(float fTimeElapsed)
{
	m_d3dxvVelocity += m_d3dxvGravity * fTimeElapsed;

	// XZ 평면의 최대 속력보다 크면 속도 벡터의 x와 z방향 성분을 조정한다.
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);

	if (fLength > m_fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (m_fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (m_fMaxVelocityXZ / fLength);
	}

	// Y 성분의 크기를 구한다. 이것이 y 축 방향의 최대 속력보다 크면 속도 벡터의 y방향 성분을 조정한다.
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);

	if (fLength > m_fMaxVelocityY)
		m_d3dxvVelocity.y *= (m_fMaxVelocityY / fLength);

	// 플레이어를 속도 벡터만큼 이동한다.
	move(m_d3dxvVelocity * fTimeElapsed, false);

	// 플레이어의 위치가 변경될 때 추가로 수행할 작업을 수행한다.
	if (m_pPlayerUpdatedContext)
		onPlayerUpdated(fTimeElapsed);

	DWORD nCurrentCameraMode = m_camera->getMode();

	// 플레이어의 위치가 변경되었으므로 카메라의 상태를 갱신한다.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		m_camera->update(fTimeElapsed);

	// 카메라의 위치가 변경될 때 추가로 수행할 작업을 수행한다.
	if (m_pCameraUpdatedContext)
		onCameraUpdated(fTimeElapsed);

	// 카메라가 3인칭 카메라이면 카메라가 변경된 플레이어 위치를 바라보도록 한다.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA)
		m_camera->setLookAt(m_d3dxvPosition);

	// 카메라의 카메라 변환 행렬을 다시 생성한다.
	m_camera->regenerateViewMatrix();


	// 플레이어의 속도 벡터가 마찰력 때문에 감속이 되어야 한다면 감속 벡터를 생성한다.
	D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	fLength = D3DXVec3Length(&m_d3dxvVelocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength)
		fDeceleration = fLength;
	m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

// 플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수
void CPlayer::regenerateWorldMatrix()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x; m_d3dxmtxWorld._12 = m_d3dxvRight.y; m_d3dxmtxWorld._13 = m_d3dxvRight.z;
	m_d3dxmtxWorld._21 = m_d3dxvUp.x; m_d3dxmtxWorld._22 = m_d3dxvUp.y; m_d3dxmtxWorld._23 = m_d3dxvUp.z;
	m_d3dxmtxWorld._31 = m_d3dxvLook.x; m_d3dxmtxWorld._32 = m_d3dxvLook.y; m_d3dxmtxWorld._33 = m_d3dxvLook.z;
	m_d3dxmtxWorld._41 = m_d3dxvPosition.x; m_d3dxmtxWorld._42 = m_d3dxvPosition.y; m_d3dxmtxWorld._43 = m_d3dxvPosition.z;
}

// 카메라를 변경할 때 호출되는 함수
CCamera* CPlayer::onChangeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	// 새로운 카메라의 모드에 따라 카메라를 새로 생성한다.
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
		// Look 벡터와 월드 좌표계의 z축(0, 0, 1)이 이루는 각도(내적=cos)를 계산하여 플레이어의 y 축의 회전 각도 m_fYaw를 계산한다.
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
		// 기존 카메라가 없으면 새로운 카메라를 위한 셰이더 변수를 생성한다.
		if (!m_camera)
			newCamera->createShaderVariables(pd3dDevice);

		newCamera->setMode(nNewCameraMode);

		// 현재 카메라를 사용하는 플레이어 객체를 설정한다.
		newCamera->setPlayer(this);
	}

	if (m_camera)
		delete m_camera;

	return newCamera;
}

void CPlayer::changeCamera(ID3D11Device* pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{

}

// 플레이어의 카메라가 3인칭 카메라일 때 플레이어 메시를 렌더링한다.
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

// 플레이어의 위치가 바뀔 때마다 호출되는 함수
void CPlayer::onPlayerUpdated(float fTimeElapsed)
{

}

// 카메라의 위치가 바뀔 때마다 호출되는 함수
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

	// 플레이어를 위한 셰이더 변수를 생성한다.
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
	// 플레이어의 특성을 1인칭 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
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

	// 플레이어의 특성을 스페이스 쉽 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
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

	// 플레이어 특성을 3인칭 카메라 모드에 맞게 변경한다.
	// 지연 효과와 카메라 오프셋을 설정한다.
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

	// 카메라 정보를 시간에 따라 갱신한다.
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