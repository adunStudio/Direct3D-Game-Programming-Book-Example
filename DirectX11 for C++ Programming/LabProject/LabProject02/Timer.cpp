#include "stdafx.h"
#include "Timer.h"


// 컴퓨터가 성능 카운터(Performance Counter) 하드웨어를 가지고 잇으면 성능 카운터와 성능 주파수(Performance Frequency)를 사용하여 시간단위를 설정한다.
// 성능 주파수를 사용할 수 없으면 멀티미디어 타이머를 사용한다. 이 경우 시간 단위는 0.001초다.
CGameTimer::CGameTimer()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_performaceFrequency))
	{
		m_hardwareHasPerfrormanceCounter = true;
		
		QueryPerformanceCounter((LARGE_INTEGER*)&m_lastTime);

		m_timeScale = 1.0f / m_performaceFrequency;
	}
	else
	{
		m_hardwareHasPerfrormanceCounter = false;

		m_lastTime = timeGetTime();

		m_timeScale = 0.001f;
	}

	m_sampleCount = 0;
	m_currentFrameRate = 0;
	m_framePerSecond = 0;
	m_FPSTimeElapsed = 0;
}


CGameTimer::~CGameTimer()
{
}

// 타이머 시간을 갱신한다.
void CGameTimer::tick(float fLockFPS)
{
	if (m_hardwareHasPerfrormanceCounter)
		QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

	else
		m_currentTime = timeGetTime();

	// 마지막으로 이 함추를 호출한 이후 경과한 시간을 계산한다.
	float timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;

	if (fLockFPS > 0.0f)
	{
		// 이 함수의 매개 변수(fLockFPS)가 0보다 크면 이 시간만큼
		// 호출한 함수를 기다리게 한다.
		while (timeElapsed < (1.0f / fLockFPS))
		{
			if (m_hardwareHasPerfrormanceCounter)
				QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

			else
				m_currentTime = timeGetTime();

			// 마지막으로 이 함수를 호출한 이후 경과한 시간을 계산한다.
			timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;
		}
	}

	// 현재 시각을 m_lastTime에 저장한다.
	m_lastTime = m_currentTime;

	// 마지막 프레임 처리 시간과 현재 프레임 처리 시간의 차이가 1초보다 작으면
	// 현재 프레임 처리 시간을 m_frameTime[0]에 저장한다.
	if (fabsf(timeElapsed - m_timeElapsed) < 1.0f)
	{
		memmove(&m_frameTime[1], m_frameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));

		m_frameTime[0] = timeElapsed;

		if (m_sampleCount < MAX_SAMPLE_COUNT)
			++m_sampleCount;
	}

	// 초당 프레임 수를 1 증가시킨다.
	++m_framePerSecond;

	// 현재 프레임 처리 시간을 누적하여 저장한다.
	m_FPSTimeElapsed += timeElapsed;
	
	if (m_FPSTimeElapsed > 1.0f)
	{
		m_currentFrameRate = m_framePerSecond;
		m_framePerSecond = 0;
		m_FPSTimeElapsed = 0.0f;
	}


	// 누적된 프레임 처리 시간의 평균을 구하여 프레임 처리 시간을 구한다.
	m_timeElapsed = 0.0f;

	for (ULONG i = 0; i < m_sampleCount; ++i)
		m_timeElapsed += m_frameTime[i];

	if (m_sampleCount > 0)
		m_timeElapsed /= m_sampleCount;
}

// 프레임 레이트를 반환한다.
unsigned long CGameTimer::getFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		// 현재 프레임 레이트를 문자열로 변환하여 lpszString 버퍼에 쓰고 "FPS"와 결합한다.
		_itow_s(m_currentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return m_currentFrameRate;
}

// 프레임의 평균 경과 시간을 반환한다.
float CGameTimer::getTimeElapsed()
{
	return m_timeElapsed;
}
