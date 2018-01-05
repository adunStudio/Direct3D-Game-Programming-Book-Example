#include "stdafx.h"
#include "Camera.h"
#include "Object.h"
#include "Player.h"

CCamera::CCamera(CCamera* pCamera)
{
	m_player = nullptr;
	D3DXMatrixIdentity(&m_d3dxmtxView);
	D3DXMatrixIdentity(&m_d3dxmtxProjection);

	// 카메라가 이미 있으면 기존 카메라의 정보를 새로운 카메라에 복사한다.
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

// 뷰포트를 설정하는 함수
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

// 카메라 변환 행렬과 투영 변환 행렬을 생성하는 함수

// 카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up벡터(로컬 y축 벡터)
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
	// 카메라의 z 축 벡터를 정규화한다.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);

	// 카메라의 z 축과 y 축에 수직인 벡터를 x 축으로 설정한다.
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);

	// 카메라의 x 축 벡터를 정규화한다.
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);

	// 카메라의 z 축과 x 축에 수직인 벡터를 y 축으로 설정한다.
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);

	// 카메라의 y 축 벡터를 정규화한다.
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	m_d3dxmtxView._11 = m_d3dxvRight.x; m_d3dxmtxView._12 = m_d3dxvUp.x; m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y; m_d3dxmtxView._22 = m_d3dxvUp.y; m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z; m_d3dxmtxView._32 = m_d3dxvUp.z; m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvRight);
	m_d3dxmtxView._42 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvUp);
	m_d3dxmtxView._43 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvLook);
}


// 카메라 좌표계의 근 평면 거리, 원 평면거리, 화면의 종횡비, 카메라 FOV 각도 위치벡터
void CCamera::generateProjectionMatrix(float fNearPlaneDistance, float fFrarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFrarPlaneDistance);
}

// 카메라 변환 행렬과 투영 변환 행렬을 셰이더로 전달하기 위한 구조체에 대하여 상수 버퍼를 생성하는 함수
void CCamera::createShaderVariables(ID3D11Device* pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 입력, 입력, 출력
	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbCamera);
}

// 카메라 변환 행렬과 투영변환 행렬을 셰이더로 전달하기 위한 구조체의 상수 버퍼를 갱신하고 디바이스 컨텍스트 슬롯에 연결하는 함수
// 상수 버퍼의 메모리 주소를 가져와서 카메라 변환 행렬과 투영 변환 행렬을 복사한다.
void CCamera::updateSHaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{

// 서브 리소스 데이터에 접근하기 위한 구조체
// pData 데이터에 대한 포인터
D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

// 서브 리소스에 포함된 데이터에 대한 포인터를 얻기위한 함수 (이 함수를 호출하면 GPU는 서브 리소스에 접근할 수 없게 된다.)
// ID3D11Resource 인터페이스 포인터, 서브 리소스 인덱스, 리소스에 대한 CPU의 권한, GPU가 리소스에 접근하고 잇을 때 CPU가 어덯게 할 것인가를 나타내는 상수, 맵핑된 서브 리소스에 대한 포인터
pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

VS_CB_VIEWPROJECTION_MATRIX* pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX*)d3dMappedResource.pData;

D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);

// 메모리 주소 사용을 반환한다.
pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

// 상수 버퍼 슬롯(VS_SLOT_VIEWPROJECTION_MATRIX)에 설정한다.
pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);
}


/* CSpaceShipCamera */
CSpaceShipCamera::CSpaceShipCamera(CCamera* pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CSpaceShipCamera::rotate(float x, float y, float z)
{
	// 스페이스 십 카메라를 플레이어의 로컬 x, y, z 축을 기준으로 회전한다.
	// 일반적인 스페이스 십 카메라의 회전은 플레이어를 회전하여 이루어진다.
	// 스페이스 십 카메라를 따로 회전하는 것을 허용하지 않는다.

	// 플레이어의 로컬 x 축을 기준으로 카메라를 회전하면 카메라의 로컬 x, y, z 축이 바뀐다.
	// 플레이어의 중심 위치와 카메라의 중심 위치가 다르면 회전에 의해 카메라의 위치도 바뀌게 된다.


	D3DXMATRIX mtxRotate;

	if (m_player && (x != 0.0f))
	{
		// 플레이어의 로컬 x 축에 대하여 x 각도의 회전 행렬을 계산한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getRightVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // 카메라의 로컬 x축 회전
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // 카메라의 로컬 y축 회전
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // 카메라의 로컬 z축 회전

		// 카메라의 위치 벡터에서 플레이어의 위치 벡터를 뺀다.
		// 결과는 플레이어 위치를 기준으로 한 카메라의 위치 벡터
		m_d3dxvPosition -= m_player->getPosition();

		// 플레이어의 위치를 중심으로 카메라의 위치 벡터(플레이어를 기준으로 한)를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}

	if (m_player && (y != 0.0f))
	{
		// 플레이어의 로컬 y 축에 대하여 y 각도의 회전 행렬을 계산한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getUpVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // 카메라의 로컬 x축 회전
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // 카메라의 로컬 y축 회전
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // 카메라의 로컬 z축 회전

																		   // 카메라의 위치 벡터에서 플레이어의 위치 벡터를 뺀다.
																		   // 결과는 플레이어 위치를 기준으로 한 카메라의 위치 벡터
		m_d3dxvPosition -= m_player->getPosition();

		// 플레이어의 위치를 중심으로 카메라의 위치 벡터(플레이어를 기준으로 한)를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}

	if (m_player && (z != 0.0f))
	{
		// 플레이어의 로컬 z 축에 대하여 z 각도의 회전 행렬을 계산한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getLookVector(), (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate); // 카메라의 로컬 x축 회전
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);       // 카메라의 로컬 y축 회전
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);   // 카메라의 로컬 z축 회전

																		   // 카메라의 위치 벡터에서 플레이어의 위치 벡터를 뺀다.
																		   // 결과는 플레이어 위치를 기준으로 한 카메라의 위치 벡터
		m_d3dxvPosition -= m_player->getPosition();

		// 플레이어의 위치를 중심으로 카메라의 위치 벡터(플레이어를 기준으로 한)를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
	}
}


/* CFirstPersonCamera */
CFirstPersonCamera::CFirstPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	// 일반적으로 1인칭 카메라는 사람의 눈을 의미한다.
	// 1인칭 카메라로 변경하기  이전의 카메라가 스페이스 십 카메라이면 카메라의 Up 벡터를 월드 좌표계의 y 축이 되도록 한다.
	// 이것은 스페이스 십 카메라의 로컬 y 축 벡터가 어떤 방향이든지 1인칭 카메라의 로컬 y 축 벡터가 월드 좌표계의 y 축이 되도록 즉, 똑바로 서있는 형태로 설정한다는 의미다.
	// 그리고 로컬 x 축 벡터와 로컬 z 축 벡터의 y좌표가 0.0f가 되도록한다.

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
		// 카메라의 로컬 x 축을 기준으로 회전하는 행렬 생성 (고개를 끄덕이는 동작)
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}

	if (m_player && (y != 0.0f))
	{
		//카메라의 로컬 y축을 기준으로 회전하는 행렬 생성
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(y));

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}

	if (m_player && (z != 0.0f))
	{
		// 플레이어의 로컬 z 축을 기준으로 회전하는 행렬 생성
		D3DXMatrixRotationAxis(&mtxRotate, &m_player->getLookVector(), (float)D3DXToRadian(y));

		// 카메라의 위치 벡터를 플레이어 좌표계로 표현(오프셋 벡터).
		m_d3dxvPosition -= m_player->getPosition();

		// 오프셋 벡터 벡터를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);

		// 회전한 카메라의 위치를 월드 좌표계로 표현한다.
		m_d3dxvPosition += m_player->getPosition();

		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}
}


/* CThirdPersonCamera */
CThirdPersonCamera::CThirdPersonCamera(CCamera* pCamera) : CCamera(pCamera)
{
	// 일반적으로 3인칭 카메라는 제 삼자의 눈으로 플레이러를 바라보는 것을 의미
	// 보통 플레이어에서 일정한 거리(카메라 오프셋)를 두고 플레이어의 일정한 지점(LookAt 벡터)을 바라보도록한다.
	// 플레이어를 움직이면 카메라는 자동으로 따라서 움직이게 된다.
	// 카메라만 별도로 움직일 수는 없다.

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
	// 3인칭 카메라의 회전은 플레이어의 회전으로 발생한다.
	// 플레이어의 회전은 플레이어의 로컬 y축을 중심으로 하는 회전만 가능하다.
	// 플레이어가 회전하면 카메라는 플레이어의 y축을 중심으로 회전하고 난 후 플레이어를 바라보도록 해야 한다.

	if (m_player)
	{
		D3DXMATRIX mtxRotate;
		D3DXMatrixIdentity(&mtxRotate);
		D3DXVECTOR3 d3dxvRight = m_player->getRightVector();
		D3DXVECTOR3 d3dxvUp = m_player->getUpVector();
		D3DXVECTOR3 d3dxvLook = m_player->getLookVector();

		// 플레이어의 로컬 x, y, z 축 벡터로부터 회전 행렬을 생성한다.
		mtxRotate._11 = d3dxvRight.x; mtxRotate._21 = d3dxvUp.x; mtxRotate._31 = d3dxvLook.x;
		mtxRotate._12 = d3dxvRight.y; mtxRotate._22 = d3dxvUp.y; mtxRotate._32 = d3dxvLook.y;
		mtxRotate._13 = d3dxvRight.z; mtxRotate._23 = d3dxvUp.z; mtxRotate._33 = d3dxvLook.z;

		D3DXVECTOR3 d3dxvOffset;
		D3DXVec3TransformCoord(&d3dxvOffset, &m_d3dxvOffset, &mtxRotate);
		
		// 회전한 카메라의 위치는 플레이어의 위치에 회전한 카메라 오프셋 벡터를 더한 것이다.
		D3DXVECTOR3 d3dxvPosition = m_player->getPosition() + d3dxvOffset;

		// 현재 카메라 위치에서 회전한 카메라의 위치까지의 벡터
		D3DXVECTOR3 d3dxvDirection = d3dxvPosition - m_d3dxvPosition;
		float fLength = D3DXVec3Length(&d3dxvDirection);
		D3DXVec3Normalize(&d3dxvDirection, &d3dxvDirection);

		// 3인칭 카메라의 래그(Lag)는 플레이어가 회전하더라도 카메라가 동시에 따라서 회전하지 않고 약간의 시차를 두고 회전하는 효과를 구현하기 위한 것
		// m_fTimeLag가 1보다 크면 fTimeLagScale이 작아지고 실제 회전이 적게 일어난다.
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
	// 3인칭 카메라를 회전시키면 카메라의 위치와 바향이 바뀌게 된다.
	// 그리고 카메라가 플레이어를 바라보도록 해야 하므로 새로운 카메라의 위치 벡터를 사용하여 카메라의 방향 벡터(Right, Up, Look 벡터)를 다시 지정한다.
	// 이를 위해 카메라 변환 행렬을 사용한다.
	// 카메라 변환 행렬의 첫 번째 열 벡터가 Right 벡터, 두 번째 열 벡터가 Up 벡터, 세 번째 열 벡터가 Look 벡터가 된다.


	D3DXMATRIX mtxLookAt;

	// D3DXMatrixLookAtLH()  함수는 카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up 벡터를 지정하면 카메라 변환 행렬을 반환한다.
	D3DXMatrixLookAtLH(&mtxLookAt, &m_d3dxvPosition, &vLookAt, &m_player->getUpVector());

	m_d3dxvRight = D3DXVECTOR3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_d3dxvUp    = D3DXVECTOR3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_d3dxvLook  = D3DXVECTOR3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}
