#include "stdafx.h"
#include "Timer.h"


// ��ǻ�Ͱ� ���� ī����(Performance Counter) �ϵ��� ������ ������ ���� ī���Ϳ� ���� ���ļ�(Performance Frequency)�� ����Ͽ� �ð������� �����Ѵ�.
// ���� ���ļ��� ����� �� ������ ��Ƽ�̵�� Ÿ�̸Ӹ� ����Ѵ�. �� ��� �ð� ������ 0.001�ʴ�.
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

// Ÿ�̸� �ð��� �����Ѵ�.
void CGameTimer::tick(float fLockFPS)
{
	if (m_hardwareHasPerfrormanceCounter)
		QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

	else
		m_currentTime = timeGetTime();

	// ���������� �� ���߸� ȣ���� ���� ����� �ð��� ����Ѵ�.
	float timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;

	if (fLockFPS > 0.0f)
	{
		// �� �Լ��� �Ű� ����(fLockFPS)�� 0���� ũ�� �� �ð���ŭ
		// ȣ���� �Լ��� ��ٸ��� �Ѵ�.
		while (timeElapsed < (1.0f / fLockFPS))
		{
			if (m_hardwareHasPerfrormanceCounter)
				QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTime);

			else
				m_currentTime = timeGetTime();

			// ���������� �� �Լ��� ȣ���� ���� ����� �ð��� ����Ѵ�.
			timeElapsed = (m_currentTime - m_lastTime) * m_timeScale;
		}
	}

	// ���� �ð��� m_lastTime�� �����Ѵ�.
	m_lastTime = m_currentTime;

	// ������ ������ ó�� �ð��� ���� ������ ó�� �ð��� ���̰� 1�ʺ��� ������
	// ���� ������ ó�� �ð��� m_frameTime[0]�� �����Ѵ�.
	if (fabsf(timeElapsed - m_timeElapsed) < 1.0f)
	{
		memmove(&m_frameTime[1], m_frameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));

		m_frameTime[0] = timeElapsed;

		if (m_sampleCount < MAX_SAMPLE_COUNT)
			++m_sampleCount;
	}

	// �ʴ� ������ ���� 1 ������Ų��.
	++m_framePerSecond;

	// ���� ������ ó�� �ð��� �����Ͽ� �����Ѵ�.
	m_FPSTimeElapsed += timeElapsed;
	
	if (m_FPSTimeElapsed > 1.0f)
	{
		m_currentFrameRate = m_framePerSecond;
		m_framePerSecond = 0;
		m_FPSTimeElapsed = 0.0f;
	}


	// ������ ������ ó�� �ð��� ����� ���Ͽ� ������ ó�� �ð��� ���Ѵ�.
	m_timeElapsed = 0.0f;

	for (ULONG i = 0; i < m_sampleCount; ++i)
		m_timeElapsed += m_frameTime[i];

	if (m_sampleCount > 0)
		m_timeElapsed /= m_sampleCount;
}

// ������ ����Ʈ�� ��ȯ�Ѵ�.
unsigned long CGameTimer::getFrameRate(LPTSTR lpszString, int nCharacters)
{
	if (lpszString)
	{
		// ���� ������ ����Ʈ�� ���ڿ��� ��ȯ�Ͽ� lpszString ���ۿ� ���� "FPS"�� �����Ѵ�.
		_itow_s(m_currentFrameRate, lpszString, nCharacters, 10);
		wcscat_s(lpszString, nCharacters, _T(" FPS)"));
	}

	return m_currentFrameRate;
}

// �������� ��� ��� �ð��� ��ȯ�Ѵ�.
float CGameTimer::getTimeElapsed()
{
	return m_timeElapsed;
}
