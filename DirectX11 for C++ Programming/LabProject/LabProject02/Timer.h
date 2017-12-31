#pragma once

// 50ȸ�� 
const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
private:
	// ��ǻ�Ͱ� Performance Counter�� �����°�?
	bool m_hardwareHasPerfrormanceCounter;

	float m_timeScale;             // Scale Counter�� ��
	float m_timeElapsed;           // ������ ������ ���� ������ �ð�

	__int64 m_currentTime;         // ���� �ð�
	__int64 m_lastTime;            // ������ ������ �ð�

	__int64 m_performaceFrequency; // ��ǻ���� Performance Frequency

	float m_frameTime[MAX_SAMPLE_COUNT]; // ������ �ð��� �����ϱ� ���� �迭
	ULONG m_sampleCount;                 // ������ ������ Ƚ��

	unsigned long m_currentFrameRate;    // ������ ������ ����Ʈ
	unsigned long m_framePerSecond;      // �ʴ� ������ ��

	float m_FPSTimeElapsed;              // ������ ����Ʈ ��� �ҿ� �ð�

public:
	CGameTimer();
	virtual ~CGameTimer();

	// Ÿ�̸� �ð��� �����Ѵ�.
	void tick(float fLockFPS = 0.0f);

	// ������ ����Ʈ�� ��ȯ�Ѵ�.
	unsigned long getFrameRate(LPTSTR lpszString = nullptr, int nCharacters = 0);

	// �������� ��� ��� �ð��� ��ȯ�Ѵ�.
	float getTimeElapsed();
};

