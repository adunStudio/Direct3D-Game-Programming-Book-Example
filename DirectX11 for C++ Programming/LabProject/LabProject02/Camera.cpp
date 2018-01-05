#include "stdafx.h"
#include "Camera.h"
#include "Object.h"
#include "Player.h"

CCamera::CCamera(CCamera* pCamera)
{
	m_player = nullptr;
	D3DXMatrixIdentity(&m_d3dxmtxView);
	D3DXMatrixIdentity(&m_d3dxmtxProjection);

	// ī�޶� �̹� ������ ���� ī�޶��� ������ ���ο� ī�޶� �����Ѵ�.
	if (pCamera)
	{
		m_d3dxvPosition = pCamera->getPosition();
		m_d3dxvRight = pCamera->getRightVector();
		m_d3dxvLook = pCamera->getLookVector();
		m_d3dxvUp = pCamera->getUpVector();
		m_fPitch = pCamera->getPitch();
		m_fRoll = pCamera->getRoll();
		m_fYaw = pCamera->getYaw();
		m_d3dxmtxView = pCamera->getViewMatrix();
		m_d3dxmtxProjection = pCamera->getProjectionMatrix();
		m_d3dViewport = pCamera->getViewport();
		m_d3dxvLookAtWorld = pCamera->getLookAtPosition();
		m_d3dxvOffset = pCamera->getOffset();
		m_fTimeLag = pCamera->getTimeLag();
		m_player = pCamera->getPlayer();
		m_pd3dcbCamera = pCamera->getCameraConstantBuffer();
		if (m_pd3dcbCamera) m_pd3dcbCamera->AddRef();
	}
	else
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = 0.0f;

		m_fTimeLag = 0.0f;

		m_d3dxvLookAtWorld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_nMode = 0x00;

		m_player = nullptr;

		D3DXMatrixIdentity(&m_d3dxmtxView);
		D3DXMatrixIdentity(&m_d3dxmtxProjection);

		m_pd3dcbCamera = nullptr;
	}
}


CCamera::~CCamera()
{
	if (m_pd3dcbCamera)
		m_pd3dcbCamera->Release();
}

// ����Ʈ�� �����ϴ� �Լ�
void CCamera::setViewport(ID3D11DeviceContext* pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xStart);
	m_d3dViewport.TopLeftY = float(yStart);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;

	pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����ϴ� �Լ�

// ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up����(���� y�� ����)
void CCamera::generateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp)
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &d3dxvEyePosition, &d3dxvLookAt, &d3dxvUp);
}

void CCamera::generateViewMatrix()
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &m_d3dxvPosition, &m_d3dxvLookAtWorld, &m_d3dxvUp);
}

void CCamera::regenerateViewMatrix()
{
	// ī�޶��� z �� ���͸� ����ȭ�Ѵ�.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);

	// ī�޶��� z ��� y �࿡ ������ ���͸� x ������ �����Ѵ�.
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);

	// ī�޶��� x �� ���͸� ����ȭ�Ѵ�.
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);

	// ī�޶��� z ��� x �࿡ ������ ���͸� y ������ �����Ѵ�.
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);

	// ī�޶��� y �� ���͸� ����ȭ�Ѵ�.
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	m_d3dxmtxView._11 = m_d3dxvRight.x; m_d3dxmtxView._12 = m_d3dxvUp.x; m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y; m_d3dxmtxView._22 = m_d3dxvUp.y; m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z; m_d3dxmtxView._32 = m_d3dxvUp.z; m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvRight);
	m_d3dxmtxView._42 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvUp);
	m_d3dxmtxView._43 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvLook);
}


// ī�޶� ��ǥ���� �� ��� �Ÿ�, �� ���Ÿ�, ȭ���� ��Ⱦ��, ī�޶� FOV ���� ��ġ����
void CCamera::generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFrarPlaneDistance);
}

// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���̴��� �����ϱ� ���� ����ü�� ���Ͽ� ��� ���۸� �����ϴ� �Լ�
void CCamera::createShaderVariables(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �Է�, �Է�, ���
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbCamera);
}

// ī�޶� ��ȯ ��İ� ������ȯ ����� ���̴��� �����ϱ� ���� ����ü�� ��� ���۸� �����ϰ� ����̽� ���ؽ�Ʈ ���Կ� �����ϴ� �Լ�
// ��� ������ �޸� �ּҸ� �����ͼ� ī�޶� ��ȯ ��İ� ���� ��ȯ ����� �����Ѵ�.
void CCamera::updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{

// ���� ���ҽ� �����Ϳ� �����ϱ� ���� ����ü
// pData �����Ϳ� ���� ������
D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

// ���� ���ҽ��� ���Ե� �����Ϳ� ���� �����͸� ������� �Լ� (�� �Լ��� ȣ���ϸ� GPU�� ���� ���ҽ��� ������ �� ���� �ȴ�.)
// ID3D11Resource �������̽� ������, ���� ���ҽ� �ε���, ���ҽ��� ���� CPU�� ����, GPU�� ���ҽ��� �����ϰ� ���� �� CPU�� ��F�� �� ���ΰ��� ��Ÿ���� ���, ���ε� ���� ���ҽ��� ���� ������
pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

VS_CB_VIEWPROJECTION_MATRIX* pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX*)d3dMappedResource.pData;

D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);

// �޸� �ּ� ����� ��ȯ�Ѵ�.
pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

// ��� ���� ����(VS_SLOT_VIEWPROJECTION_MATRIX)�� �����Ѵ�.
pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);
}


/* CSpaceShipCamera */
CSpaceShipCamera::CSpaceShipCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CSpaceShipCamera::rotate(float x, float y, float z)
{
	// �����̽� �� ī�޶� �÷��̾��� ���� x, y, z ���� �������� ȸ���Ѵ�.
	// �Ϲ����� �����̽� �� ī�޶��� ȸ���� �÷��̾ ȸ���Ͽ� �̷������.
	// �����̽� �� ī�޶� ���� ȸ���ϴ� ���� ������� �ʴ´�.

	// �÷��̾��� ���� x ���� �������� ī�޶� ȸ���ϸ� ī�޶��� ���� x, y, z ���� �ٲ��.
	// �÷��̾��� �߽� ��ġ�� ī�޶��� �߽� ��ġ�� �ٸ��� ȸ���� ���� ī�޶��� ��ġ�� �ٲ�� �ȴ�.


	D3DXMATRIX mtxRotate;

	if (m_player && (x != 0.0f))
	{
		// �÷��̾��� ���� x �࿡ ���Ͽ� x ������ ȸ�� ����� ����Ѵ�.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getRightVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // ī�޶��� ���� x�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // ī�޶��� ���� y�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // ī�޶��� ���� z�� ȸ��

		// ī�޶��� ��ġ ���Ϳ��� �÷��̾��� ��ġ ���͸� ����.
		// ����� �÷��̾� ��ġ�� �������� �� ī�޶��� ��ġ ����
		m_d3dxvPosition -= m_player->getPosition();

		// �÷��̾��� ��ġ�� �߽����� ī�޶��� ��ġ ����(�÷��̾ �������� ��)�� ȸ���Ѵ�.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}

	if (m_player && (y != 0.0f))
	{
		// �÷��̾��� ���� y �࿡ ���Ͽ� y ������ ȸ�� ����� ����Ѵ�.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getUpVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // ī�޶��� ���� x�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // ī�޶��� ���� y�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // ī�޶��� ���� z�� ȸ��

																		   // ī�޶��� ��ġ ���Ϳ��� �÷��̾��� ��ġ ���͸� ����.
																		   // ����� �÷��̾� ��ġ�� �������� �� ī�޶��� ��ġ ����
		m_d3dxvPosition -= m_player->getPosition();

		// �÷��̾��� ��ġ�� �߽����� ī�޶��� ��ġ ����(�÷��̾ �������� ��)�� ȸ���Ѵ�.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}

	if (m_player && (z != 0.0f))
	{
		// �÷��̾��� ���� z �࿡ ���Ͽ� z ������ ȸ�� ����� ����Ѵ�.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getLookVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // ī�޶��� ���� x�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // ī�޶��� ���� y�� ȸ��
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // ī�޶��� ���� z�� ȸ��

																		   // ī�޶��� ��ġ ���Ϳ��� �÷��̾��� ��ġ ���͸� ����.
																		   // ����� �÷��̾� ��ġ�� �������� �� ī�޶��� ��ġ ����
		m_d3dxvPosition -= m_player->getPosition();

		// �÷��̾��� ��ġ�� �߽����� ī�޶��� ��ġ ����(�÷��̾ �������� ��)�� ȸ���Ѵ�.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}
}


/* CFirstPersonCamera */
CFirstPersonCamera::CFirstPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	// �Ϲ������� 1��Ī ī�޶�� ����� ���� �ǹ��Ѵ�.
	// 1��Ī ī�޶�� �����ϱ�  ������ ī�޶� �����̽� �� ī�޶��̸� ī�޶��� Up ���͸� ���� ��ǥ���� y ���� �ǵ��� �Ѵ�.
	// �̰��� �����̽� �� ī�޶��� ���� y �� ���Ͱ� � �����̵��� 1��Ī ī�޶��� ���� y �� ���Ͱ� ���� ��ǥ���� y ���� �ǵ��� ��, �ȹٷ� ���ִ� ���·� �����Ѵٴ� �ǹ̴�.
	// �׸��� ���� x �� ���Ϳ� ���� z �� ������ y��ǥ�� 0.0f�� �ǵ����Ѵ�.

	m_nMode = FIRST_PERSON_CAMERA;

	if (pCamera)
	{
		if (pCamera->getMode() == SPACESHIP_CAMERA)
		{
			m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y  = 0.0f;
			D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
			D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		}
	}
}

void CFirstPersonCamera::rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;

	if (x != 0.0f)
	{
		// ī�޶��� ���� x ���� �������� ȸ���ϴ� ��� ���� (���� �����̴� ����)
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}

	if (m_player && (y != 0.0f))
	{
		//ī�޶��� ���� y���� �������� ȸ���ϴ� ��� ����
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(y));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}

	if (m_player && (z != 0.0f))
	{
		// �÷��̾��� ���� z ���� �������� ȸ���ϴ� ��� ����
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getLookVector(), (float)D3DXToRadian(y));

		// ī�޶��� ��ġ ���͸� �÷��̾� ��ǥ��� ǥ��(������ ����).
		m_d3dxvPosition -= m_player->getPosition();

		// ������ ���� ���͸� ȸ���Ѵ�.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);

		// ȸ���� ī�޶��� ��ġ�� ���� ��ǥ��� ǥ���Ѵ�.
		m_d3dxvPosition += m_player->getPosition();

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}
}


/* CThirdPersonCamera */
CThirdPersonCamera::CThirdPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	// �Ϲ������� 3��Ī ī�޶�� �� ������ ������ �÷��̷��� �ٶ󺸴� ���� �ǹ�
	// ���� �÷��̾�� ������ �Ÿ�(ī�޶� ������)�� �ΰ� �÷��̾��� ������ ����(LookAt ����)�� �ٶ󺸵����Ѵ�.
	// �÷��̾ �����̸� ī�޶�� �ڵ����� ���� �����̰� �ȴ�.
	// ī�޶� ������ ������ ���� ����.

	m_nMode = THIRD_PERSON_CAMERA;

	if (pCamera)
	{
		if (pCamera->getMode() == SPACESHIP_CAMERA)
		{
			m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;
			D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
			D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		}
	}
}

void CThirdPersonCamera::update(float fTimeElapsed)
{
	// 3��Ī ī�޶��� ȸ���� �÷��̾��� ȸ������ �߻��Ѵ�.
	// �÷��̾��� ȸ���� �÷��̾��� ���� y���� �߽����� �ϴ� ȸ���� �����ϴ�.
	// �÷��̾ ȸ���ϸ� ī�޶�� �÷��̾��� y���� �߽����� ȸ���ϰ� �� �� �÷��̾ �ٶ󺸵��� �ؾ� �Ѵ�.

	if (m_player)
	{
		D3DXMATRIX mtxRotate;
		D3DXMatrixIdentity(&mtxRotate);
		D3DXVECTOR3 d3dxvRight = m_player->getRightVector();
		D3DXVECTOR3 d3dxvUp = m_player->getUpVector();
		D3DXVECTOR3 d3dxvLook = m_player->getLookVector();

		// �÷��̾��� ���� x, y, z �� ���ͷκ��� ȸ�� ����� �����Ѵ�.
		mtxRotate._11 = d3dxvRight.x; mtxRotate._21 = d3dxvUp.x; mtxRotate._31 = d3dxvLook.x;
		mtxRotate._12 = d3dxvRight.y; mtxRotate._22 = d3dxvUp.y; mtxRotate._32 = d3dxvLook.y;
		mtxRotate._13 = d3dxvRight.z; mtxRotate._23 = d3dxvUp.z; mtxRotate._33 = d3dxvLook.z;

		D3DXVECTOR3 d3dxvOffset;
		D3DXVec3TransformCoord(&d3dxvOffset, &m_d3dxvOffset, &mtxRotate);
		
		// ȸ���� ī�޶��� ��ġ�� �÷��̾��� ��ġ�� ȸ���� ī�޶� ������ ���͸� ���� ���̴�.
		D3DXVECTOR3 d3dxvPosition = m_player->getPosition() + d3dxvOffset;

		// ���� ī�޶� ��ġ���� ȸ���� ī�޶��� ��ġ������ ����
		D3DXVECTOR3 d3dxvDirection = d3dxvPosition - m_d3dxvPosition;
		float fLength = D3DXVec3Length(&d3dxvDirection);
		D3DXVec3Normalize(&d3dxvDirection, &d3dxvDirection);

		// 3��Ī ī�޶��� ����(Lag)�� �÷��̾ ȸ���ϴ��� ī�޶� ���ÿ� ���� ȸ������ �ʰ� �ణ�� ������ �ΰ� ȸ���ϴ� ȿ���� �����ϱ� ���� ��
		// m_fTimeLag�� 1���� ũ�� fTimeLagScale�� �۾����� ���� ȸ���� ���� �Ͼ��.
		float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;

		float fDistance = fLength * fTimeLagScale;

		if (fDistance > fLength)
			fDistance = fLength;

		if (fLength < 0.01f)
			fDistance = fLength;

		if (fDistance > 0)
		{
			m_d3dxvPosition += d3dxvDirection * fDistance;
			setLookAt(getLookAtPosition());
		}
	}

}

void CThirdPersonCamera::setLookAt(D3DXVECTOR3& vLookAt)
{
	// 3��Ī ī�޶� ȸ����Ű�� ī�޶��� ��ġ�� ������ �ٲ�� �ȴ�.
	// �׸��� ī�޶� �÷��̾ �ٶ󺸵��� �ؾ� �ϹǷ� ���ο� ī�޶��� ��ġ ���͸� ����Ͽ� ī�޶��� ���� ����(Right, Up, Look ����)�� �ٽ� �����Ѵ�.
	// �̸� ���� ī�޶� ��ȯ ����� ����Ѵ�.
	// ī�޶� ��ȯ ����� ù ��° �� ���Ͱ� Right ����, �� ��° �� ���Ͱ� Up ����, �� ��° �� ���Ͱ� Look ���Ͱ� �ȴ�.


	D3DXMATRIX mtxLookAt;

	// D3DXMatrixLookAtLH()  �Լ��� ī�޶��� ��ġ ����, ī�޶� �ٶ󺸴� ����, ī�޶��� Up ���͸� �����ϸ� ī�޶� ��ȯ ����� ��ȯ�Ѵ�.
	D3DXMatrixLookAtLH(&mtxLookAt, &m_d3dxvPosition, &vLookAt, &m_player->getUpVector());

	m_d3dxvRight = D3DXVECTOR3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_d3dxvUp    = D3DXVECTOR3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_d3dxvLook  = D3DXVECTOR3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}
