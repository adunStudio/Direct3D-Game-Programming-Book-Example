// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <mmsystem.h>
#include <math.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <D3dx10math.h>
#include <D3DCompiler.h>

#define FRAME_BUFFER_WIDTH  640
#define FRAME_BUFFER_HEIGHT 480

// 카메라 변환 행렬과 투영 변환 행렬 그리고 월드 변환 행렬을 위한 상수 버퍼를 연결한 슬롯 번호
#define VS_SLOT_VIEWPROJECTION_MATRIX 0x00
#define VS_SLOT_WORDLD_MATRIX         0x01
