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

// 파일에서 정점 셰이더를 생성하는 함수
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
		// D3DX11CompileFromFile() 함수는 파일(pszFileName)에서 셰이더 함수(pszShaderName)를 컴파일하여
		// 컴파일된 셰이더 코드의 메모리 주소(pd3dSahderBlob)를 반환한다.
		SUCCEEDED
		(
			hResult =
				D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName,
				pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dShaderBlob,
				&pd3dErrorBlob, nullptr)
		)
	)
	{
		// 컴파일된 셰이더 코드의 메모리 주소에서 정점 셰이더를 생성한다.
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), nullptr, ppd3dVertexShader);

		// 컴파일된 셰이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다.
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);

		pd3dShaderBlob->Release();
		//MessageBox(NULL, L"Shader OK", L"Hi!", MB_OK);
	}
	else
		MessageBox(NULL, L"Shader Not OK", L"Hi!", MB_OK);


}

// 파일에서 픽셀 셰이더를 생성하는 함수
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
			// D3DX11CompileFromFile() 함수는 파일(pszFileName)에서 셰이더 함수(pszShaderName)를 컴파일하여
			// 컴파일된 셰이더 코드의 메모리 주소(pd3dSahderBlob)를 반환한다.
			SUCCEEDED
			(
				hResult =
				D3DX11CompileFromFile(pszFileName, nullptr, nullptr, pszShaderName,
					pszShaderModel, dwShaderFlags, 0, nullptr, &pd3dShaderBlob,
					&pd3dErrorBlob, nullptr)
			)
			)
	{
		// 컴파일된 셰이더 코드의 메모리 주소에서 정점 셰이더를 생성한다.
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), nullptr, ppd3dPixelShader);

		pd3dShaderBlob->Release();

		//MessageBox(NULL, L"Pixel OK", L"Hi!", MB_OK);

	}
	else
		MessageBox(NULL, L"Pixel Not OK", L"Hi!", MB_OK);

}

// CShader 파생 클래스에서 셰이더를 생성하기 위해 호출하는 가상 함수
void CShader::createShader(ID3D11Device* pd3dDevice)
{

}

// CShader 파생 클래스에서 렌더링하기 위해 호출하는 가상 함수
void CShader::render(ID3D11DeviceContext* pd3dDeviceContext)
{

	if (m_pd3dVertexLayout) 
		pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);


	if (m_pd3dVertexShader)
		pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);



	if (m_pd3dPixelShader) 
		pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

}


/* CObjectShader */

CObjectShader::CObjectShader()
{
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
}

// 셰이더 생성 함수 선언, nObjects는 이 셰이더를 통해 렌더링되는 객체의 개수
// 정점 셰이더와 입력 레이아웃 객체, 그리고 픽셀 셰이더를 생성
void CObjectShader::createShader(ID3D11Device* pd3dDevice, int nObjects)
{




	// 1. 입력 조립 단계에 설정할 입력 레이아웃 객체를 정의한다.
	//    정점 버퍼의 한 원소가 CVertex 클래스의 멤버 변수(D3DXVECTOR3 즉, 실수 세 개) 이므로 입력 레이아웃 객체는 실수(32비트) 3개로 구성되며
	//    시맨틱이 POSITION이고 정점 데이터임을 표현한다.
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] = 
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// DXGI_FORMAT_R32G32B32A32_FLOAT : A four-component, 128-bit floating-point format that supports 32 bits per channel including alpha
	// DXGI_FORMAT_R32G32B32_FLOAT : A three - component, 96 - bit floating - point format that supports 32 bits per color channel.
	UINT nElements = ARRAYSIZE(d3dInputLayout);

	// Effect.fx 파일에서 정점 셰이더의 시작함수가 VS인 정점 셰이더를 생성한다.
	createVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VS", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);

	// Effect.fx 파일에서 픽셀 셰이더의 시작 함수가 PS인 픽셀 셰이더를 생성한다.
	createPixelShaderFromFile(pd3dDevice, L"Effect.fx", "PS", "ps_4_0", &m_pd3dPixelShader);

	m_nObjects = nObjects;

	if (m_nObjects > 0)
	{
		// 렌더링할 객체의 개수가 nObjects이므로 객체에 대한 포인터들의 배열을 정의한다.
		m_ppObjects = new CGameObject*[m_nObjects];

		for (int i = 0; i < m_nObjects; ++i)
			m_ppObjects[i] = nullptr;
	}
}

// 렌더링 함수 선언
void CObjectShader::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CShader::render(pd3dDeviceContext);

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i])
			m_ppObjects[i]->render(pd3dDeviceContext);
		

	}
}

// 셰이더에서 렌더링할 게임 객체를 추가하는 함수
void CObjectShader::addObject(CGameObject* object)
{
	// 셰이더에 객체를 연결하고 참조 카운터를 1 증가시킨다.
	if (m_ppObjects)
		m_ppObjects[m_nIndexToAdd++] = object;

	if (object)
		object->addRef();
}