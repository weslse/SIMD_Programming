/*
	2019.06.29
	Created by Weslse
*/

#include <iostream>
#include <xmmintrin.h>



// HW 0201
void Change(float *p, float from, float to, int size)
{
	__m128 vFrom = _mm_set_ps(from, from, from, from);
	__m128 vTo = _mm_set_ps(to, to, to, to);

	for (int i = 0; i < size; ++i)
	{
		__m128 vData = _mm_loadu_ps(p + i); // 배열 로드
		
		// from과 같은 위치 찾기
		__m128 vCmpData = _mm_cmpeq_ps(vData, vFrom); 
		
		// from과 같은 값이 아닌 곳의 값 유지
		vData = _mm_andnot_ps(vCmpData, vData); 
		
		// to가 들어가야할 위치에 to를 넣기
		__m128 vToData = _mm_and_ps(vCmpData, vTo);
		
		// 위에서 만든 to만 있는 값과
		// from과 같지 않은 유지된 값을 더해서 
		// 최종값 생성
		vData = _mm_add_ps(vData, vToData);
		
		// 배열에 저장
		_mm_storeu_ps(p + i, vData);
	}
}


// HW 0202
void ChangeRange(float *p, float f1, float f2, float to, int size)
{
	__m128 vF1 = _mm_set_ps(f1, f1, f1, f1);
	__m128 vF2 = _mm_set_ps(f2, f2, f2, f2);
	__m128 vTo = _mm_set_ps(to, to, to, to);

	for (int i = 0; i < size; ++i)
	{
		// 배열에서 로드
		__m128 vData = _mm_loadu_ps(p + i);
		
		// f1보다 큰 값에 대한 마스크 생성
		__m128 vB1 = _mm_cmpgt_ps(vData, vF1);
		// f2보다 작은 값들에 대한 마스크 생성
		__m128 vB2 = _mm_cmplt_ps(vData, vF2);
		// 위 마스크를 합쳐서 구간에 알맞는 마스크 생성
		__m128 vBound = _mm_and_ps(vB1, vB2);
		
		// 구간에 맞지 않는 값들을 유지
		vData = _mm_andnot_ps(vBound, vData);

		// 구간에 to값을 넣은 변수 생성
		__m128 vToData = _mm_and_ps(vBound, vTo);

		// 위에서 만든 to만 있는 값과
		// 구간에 맞지않아 유지된 값을
		// 더하여 최종값 생성
		vData = _mm_add_ps(vData, vToData);

		// 배열에 저장
		_mm_storeu_ps(p + i, vData);
	}
}

int main(void)
{
	// HW 0201
	float buf1[4] = { 10, 20, 30, 40 };
	Change(buf1, 20, 50, 4);

	// HW 0202
	float buf2[4] = { 10, 20, 30, 40 };
	ChangeRange(buf2, 15, 35, 50, 4);

	return 0;
}

// _mm_and_ps()
// _mm_andnot_ps()
// _mm_cmpeq_ps()
