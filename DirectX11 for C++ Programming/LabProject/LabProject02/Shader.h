#pragma once

#include "Object.h"

class CShader
{
public:
	// 정점 셰이더 인터페이스 포인터
	ID3D11VertexShader* m_pd3dVertexShader;
	
	// 입력 레이아웃 인터페이스 포인터
	ID3D11InputLayout*  m_pd3dVertexLayout;
	
	// 픽셀 셰이더 인터페이스 포인터
	ID3D11PixelShader*  m_pd3dPixelShader;

public:
	CShader();
	virtual ~CShader();

	// 파일에서 정점 셰이더를 생성하는 함수
	void createVertexShaderFromFile
	(
		ID3D11Device* pd3dDevice,
		WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11VertexShader** ppd3dVertexShader,
		D3D11_INPUT_ELEMENT_DESC* pd3dInputLayout, UINT nElements,
		ID3D11InputLayout** ppd3dVertexLayout
	);

	// 파일에서 픽셀 셰이더를 생성하는 함수
	void createPixelShaderFromFile
	(
		ID3D11Device* pd3dDevice,
		WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11PixelShader** ppd3dPixelShader
	);

	// CShader 파생 클래스에서 셰이더를 생성하기 위해 호출하는 가상 함수
	virtual void createShader(ID3D11Device* pd3dDevice);

	// CShader 파생 클래스에서 렌더링하기 위해 호출하는 가상 함수
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};

class CObjectShader : public CShader
{
private:
	// 이 셰이더에서 렌더링할 게임 객체에 대한 포인터들의 리스트
	CGameObject** m_ppObjects;
	int m_nObjects;

	// 이 셰이더에서 렌더링할 게임 객체를 추가할 위치를 나타내는 인덱스
	int m_nIndexToAdd;

public:
	CObjectShader();
	~CObjectShader();

	// 셰이더 생성 함수 선언, nObjects는 이 셰이더를 통해 렌더링되는 객체의 개수
	void createShader(ID3D11Device* pd3dDevice, int nObjects);

	// 렌더링 함수 선언
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// 셰이더에서 렌더링할 게임 객체를 추가하는 함수
	void addObject(CGameObject* object);
};