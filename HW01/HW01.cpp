/*
	2019.06.27
	Created by Weslse
*/

#include <iostream>
#include <xmmintrin.h>

#define MAX_SIZE 4096


void FillTable(float* table, float a, float b)
{
	__m128 vZero = _mm_set_ps(0, 0, 0, 0);
	__m128 vOne = _mm_set_ps(1, 1, 1, 1);

	float t = 1.0f / (b - a); // ����
	__m128 vValue = _mm_set_ps(3 * t, 2 * t, 1 * t, 0 * t);    // �׷��� ��
	__m128 vInterval = _mm_set_ps(t * 4, t * 4, t * 4, t * 4); // �׷��� ���� ��·�

	//_mm_max_ps();
	//_mm_min_ps();
	int term = MAX_SIZE - ((MAX_SIZE - static_cast<int>(b)) % 4); // 4ĭ�� ���̳��� ��ŭ

	for (int i = 0; i < a; i += 4)
	{
		_mm_store_ps(table + i, vZero);
	}

	for (int i = a; i < b; i += 4)
	{
		_mm_store_ps(table + i, vValue);
		vValue = _mm_add_ps(vValue, vInterval);
	}

	for (int i = b; i < term; i += 4)
	{
		_mm_store_ps(table + i, vOne);
	}

	for (int i = term; i < MAX_SIZE; ++i)
	{
		table[i] = 1;
	}
}

int main(void)
{
	float table[MAX_SIZE] = { 0 };
	FillTable(table, 123, 291);

	return 0;
}

// _mm_set_ps()�� ���� set ������ ������.
// 1. �޸𸮿� �����ؾ��ϱ� ����
// 2. ���� ��������(AX,BX,..)���� 
//    XMM0~XMM7�� �������Ϳ� ���������� ���� �Ұ���

// Load ����(�迭 ��� �ٷ� ���� ������)�� ���� ����
// ���� ���� ���� �ӵ��� ����
// �����ؾ��� ���۷��� : SIMD Intrinsic (Intel)