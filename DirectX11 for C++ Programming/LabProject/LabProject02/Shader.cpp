#include "stdafx.h"
#include "Shader.h"

CShader::CShader()
{
	m_pd3dVertexShader = nullptr;
	m_pd3dPixelShader  = nullptr;
	m_pd3dVertexLayout = nullptr;
}


CShader::~CShader()
{
	if (m_pd3dVertexShader)
		m_pd3dVertexShader->Release();

	if (m_pd3dPixelShader)
		m_pd3dPixelShader->Release();

	if (m_pd3dVertexLayout)
		m_pd3dVertexLayout->Release();
}

// ���Ͽ��� ���� ���̴��� �����ϴ� �Լ�
void CShader::createVertexShaderFromFile
(
	ID3D11Device* pd3dDevice,
	WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
	ID3D11VertexShader** ppd3dVertexShader,
	D3D11_INPUT_ELEMENT_DESC* pd3dInputLayout, UINT nElements,
	ID3D11InputLayout** ppd3dVertexLayout
)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pd3dShaderBlob = nullptr;
	ID3DBlob* pd3dErrorBlob = nullptr;

	
	if 
	(
		// D3DX11CompileFromFile() �Լ��� ����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ�
		// �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dSahderBlob)�� ��ȯ�Ѵ�.
		SUCCEEDED
		(
			hResult =
				D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName,
				pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dShaderBlob,
				&pd3dErrorBlob, nullptr)
		)
	)
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ���� ���̴��� �����Ѵ�.
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), nullptr, ppd3dVertexShader);

		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�.
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);

		pd3dShaderBlob->Release();
		//MessageBox(NULL, L"Shader OK", L"Hi!", MB_OK);
	}
	else
		MessageBox(NULL, L"Shader Not OK", L"Hi!", MB_OK);


}

// ���Ͽ��� �ȼ� ���̴��� �����ϴ� �Լ�
void CShader::createPixelShaderFromFile
(
	ID3D11Device* pd3dDevice,
	WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
	ID3D11PixelShader** ppd3dPixelShader
)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pd3dShaderBlob = nullptr;
	ID3DBlob* pd3dErrorBlob = nullptr;

	if
		(
			// D3DX11CompileFromFile() �Լ��� ����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ�
			// �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dSahderBlob)�� ��ȯ�Ѵ�.
			SUCCEEDED
			(
				hResult =
				D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName,
					pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dShaderBlob,
					&pd3dErrorBlob, nullptr)
			)
			)
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ���� ���̴��� �����Ѵ�.
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), nullptr, ppd3dPixelShader);

		pd3dShaderBlob->Release();

		//MessageBox(NULL, L"Pixel OK", L"Hi!", MB_OK);

	}
	else
		MessageBox(NULL, L"Pixel Not OK", L"Hi!", MB_OK);

}

// CShader �Ļ� Ŭ�������� ���̴��� �����ϱ� ���� ȣ���ϴ� ���� �Լ�
void CShader::createShader(ID3D11Device* pd3dDevice, int nObjects)
{

}

// CShader �Ļ� Ŭ�������� �������ϱ� ���� ȣ���ϴ� ���� �Լ�
void CShader::render(ID3D11DeviceContext* pd3dDeviceContext)
{

	if (m_pd3dVertexLayout) 
		pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);


	if (m_pd3dVertexShader)
		pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);



	if (m_pd3dPixelShader) 
		pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

}

// ���̴� Ŭ������ ��� ���۸� �����ϰ� ��ȯ�ϴ� �Լ�
void CShader::createShaderVariables(ID3D11Device* pd3dDevice)
{

}

void CShader::releaseShaderVariables()
{

}

// ���̴� Ŭ������ ��� ���۸� �����ϴ� ��� �Լ�
void CShader::updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{

}

/* CObjectShader */

CObjectShader::CObjectShader()
{
	m_pd3dcbWorldMatrix = nullptr;
	m_ppObjects = nullptr;
	m_nObjects = 0;
	m_nIndexToAdd = 0;
}

CObjectShader::~CObjectShader()
{
	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i])
			m_ppObjects[i]->release();

	if (m_ppObjects)
		delete[] m_ppObjects;

	releaseShaderVariables();
}

// ���̴� ���� �Լ� ����, nObjects�� �� ���̴��� ���� �������Ǵ� ��ü�� ����
// ���� ���̴��� �Է� ���̾ƿ� ��ü, �׸��� �ȼ� ���̴��� ����
void CObjectShader::createShader(ID3D11Device* pd3dDevice, int nObjects)
{

	// 1. �Է� ���� �ܰ迡 ������ �Է� ���̾ƿ� ��ü�� �����Ѵ�.
	//    ���� ������ �� ���Ұ� CVertex Ŭ������ ��� ����(D3DXVECTOR3 ��, �Ǽ� �� ��) �̹Ƿ� �Է� ���̾ƿ� ��ü�� �Ǽ�(32��Ʈ) 3���� �����Ǹ�
	//    �ø�ƽ�� POSITION�̰� ���� ���������� ǥ���Ѵ�.
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] = 
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// DXGI_FORMAT_R32G32B32A32_FLOAT : A four-component, 128-bit floating-point format that supports 32 bits per channel including alpha
	// DXGI_FORMAT_R32G32B32_FLOAT : A three - component, 96 - bit floating - point format that supports 32 bits per color channel.
	UINT nElements = ARRAYSIZE(d3dInputLayout);

	// Effect.fx ���Ͽ��� ���� ���̴��� �����Լ��� VS�� ���� ���̴��� �����Ѵ�.
	createVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VS", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);

	// Effect.fx ���Ͽ��� �ȼ� ���̴��� ���� �Լ��� PS�� �ȼ� ���̴��� �����Ѵ�.
	createPixelShaderFromFile(pd3dDevice, L"Effect.fx", "PS", "ps_4_0", &m_pd3dPixelShader);

	m_nObjects = nObjects;

	if (m_nObjects > 0)
	{
		// �������� ��ü�� ������ nObjects�̹Ƿ� ��ü�� ���� �����͵��� �迭�� �����Ѵ�.
		m_ppObjects = new CGameObject*[m_nObjects];

		for (int i = 0; i < m_nObjects; ++i)
			m_ppObjects[i] = nullptr;
	}

	createShaderVariables(pd3dDevice);
}


// ���̴� Ŭ������ ��� ���۸� �����ϰ� ��ȯ�ϴ� �Լ�
void CObjectShader::createShaderVariables(ID3D11Device* pd3dDevice)
{
	// ���� ��ȯ ����� ���� ��� ���۸� �����Ѵ�.
	CShader::createShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&bd, nullptr, &m_pd3dcbWorldMatrix);
}

void CObjectShader::releaseShaderVariables()
{
	if (m_pd3dcbWorldMatrix)
		m_pd3dcbWorldMatrix->Release();
}

// ���̴� Ŭ������ ��� ���۸� �����ϴ� ��� �Լ�
void CObjectShader::updateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld)
{
	// ���� ��ȯ ����� ��� ���ۿ� �����ϰ�, ��� ���۸� ����̽��� ����(VS_SLOT_WORLD_MATRIX)�� �����Ѵ�.
	CShader::updateShaderVariables(pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	VS_CB_WORLD_MATRIX* pcbWorldMatrix = (VS_CB_WORLD_MATRIX*)d3dMappedResource.pData;

	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);

	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORDLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

// ������ �Լ� ����
void CObjectShader::render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera)
{
	CShader::render(pd3dDeviceContext);

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i])
		{
			updateShaderVariables(pd3dDeviceContext, &m_ppObjects[i]->m_d3dxmtxWorld);
			m_ppObjects[i]->render(pd3dDeviceContext);
		}
	}
}

// ���̴����� �������� ���� ��ü�� �߰��ϴ� �Լ�
void CObjectShader::addObject(CGameObject* object)
{
	// ���̴��� ��ü�� �����ϰ� ���� ī���͸� 1 ������Ų��.
	if (m_ppObjects)
		m_ppObjects[m_nIndexToAdd++] = object;

	if (object)
		object->addRef();
}



void CDiffusedShader::createShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	createVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSDiffusedColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	createPixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSDiffusedColor", "ps_4_0", &m_pd3dPixelShader);
}

void CDiffusedShader::createShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::createShaderVariables(pd3dDevice);
}

void CDiffusedShader::updateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::updateShaderVariables(pd3dDeviceContext);
}

void CDiffusedShader::render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::render(pd3dDeviceContext);
}
