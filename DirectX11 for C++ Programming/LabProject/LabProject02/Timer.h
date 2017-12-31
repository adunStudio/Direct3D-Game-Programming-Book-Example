#pragma once

// 50회의 
const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
private:
	// 컴퓨터가 Performance Counter를 가졌는가?
	bool m_hardwareHasPerfrormanceCounter;

	float m_timeScale;             // Scale Counter의 양
	float m_timeElapsed;           // 마지막 프레임 이후 지나간 시간

	__int64 m_currentTime;         // 현재 시간
	__int64 m_lastTime;            // 마지막 프레임 시간

	__int64 m_performaceFrequency; // 컴퓨터의 Performance Frequency

	float m_frameTime[MAX_SAMPLE_COUNT]; // 프레임 시간을 누적하기 위한 배열
	ULONG m_sampleCount;                 // 누적된 프레임 횟수

	unsigned long m_currentFrameRate;    // 현재의 프레임 레이트
	unsigned long m_framePerSecond;      // 초당 프레임 수

	float m_FPSTimeElapsed;              // 프레임 레이트 계산 소요 시간

public:
	CGameTimer();
	virtual ~CGameTimer();

	// 타이머 시간을 갱신한다.
	void tick(float fLockFPS = 0.0f);

	// 프레임 레이트를 반환한다.
	unsigned long getFrameRate(LPTSTR lpszString = nullptr, int nCharacters = 0);

	// 프레임의 평균 경과 시간을 반환한다.
	float getTimeElapsed();
};

