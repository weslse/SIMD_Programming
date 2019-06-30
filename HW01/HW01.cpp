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

	float t = 1.0f / (b - a); // 기울기
	__m128 vValue = _mm_set_ps(3 * t, 2 * t, 1 * t, 0 * t);    // 그래프 값
	__m128 vInterval = _mm_set_ps(t * 4, t * 4, t * 4, t * 4); // 그래프 값의 상승률

	//_mm_max_ps();
	//_mm_min_ps();
	int term = MAX_SIZE - ((MAX_SIZE - static_cast<int>(b)) % 4); // 4칸씩 차이나는 만큼

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

// _mm_set_ps()와 같은 set 연산은 느리다.
// 1. 메모리에 접근해야하기 때문
// 2. 기존 레지스터(AX,BX,..)에서 
//    XMM0~XMM7의 레지스터에 직접적으로 접근 불가능

// Load 연산(배열 등에서 바로 값을 가져옴)은 보다 빠름
// 덧셈 등의 연산 속도도 빠름
// 참고해야할 레퍼런스 : SIMD Intrinsic (Intel)