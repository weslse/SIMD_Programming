#include <iostream>
#include <emmintrin.h>

using namespace std;

// HW 0301
void BGRA2RGBA(float *bgra, float *rgba, int size)
{
	for (int i = 0; i < size; i += 4)
	{
		// BGRA에서 load
		__m128 vBGRA = _mm_loadu_ps(bgra + i);
		
		// 0으로 set
		__m128 vRGBA = _mm_setzero_ps();
		
		// BGRA 0 1 2 3 이니깐
		// RGBA 2 1 0 3
		// 읽는 방향 <- 이므로
		// 3012 -> 11000110 = 0xc6
		vRGBA = _mm_shuffle_ps(vBGRA, vBGRA, 0xc6);

		// 구한 rgba 저장
		_mm_storeu_ps(rgba, vRGBA);
	}
}

// HW 0302
int CountNumber(float *colors, float value, int size)
{
	// 비교할 값에 대한 변수 생성
	__m128 vValue = _mm_set1_ps(value);
	int cnt = 0; // 개수
	for (int i = 0; i < size; i += 4)
	{
		// 행렬에서 값을 load
		__m128 vColors = _mm_loadu_ps(colors + i);
		
		// 원하는 값과 비교
		__m128 vCmp = _mm_cmpeq_ps(vColors, vValue);
		
		// movemask를 이용하여 
		// 어느 위치가 value값과 같은지 구함
		int maskNum = _mm_movemask_ps(vCmp);
		
		// 나오는 숫자 0 1 0 0 이므로
		// num을 계산하는 루프
		for (int i = 4; i > 0; --i)
		{
			// 해당 위치를 계산 
			// 순서가 3 2 1 0 이므로
			// i - 1만큼 shift 해준다.
			int divNum = (1 << (i - 1));
			
			// 해당 위치가 1이면 cnt 증가
			cnt += (maskNum / divNum);

			// 다음 자리 검사 위해 나누기
			maskNum %= divNum;
		}
	}

	return cnt;
}

int main()
{
	// HW 0301
	float bgr[4] = { 0, 0.3, 0.9, 1 };
	float rgb[4];
	BGRA2RGBA(bgr, rgb, 4);
	cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << " " << rgb[3] << endl;

	// HW 0302
	//__m128 a = _mm_set_ps(-1, 1, -2, -3);
	//cout << "mask = " << _mm_movemask_ps(a) << endl;

	float colors[4] = { 0, 0.3, 0.9, 1 };
	int num = CountNumber(colors, 0.3, 4);
	cout << "count = " << num << endl;
}

// __m128i : 16 Byte 의 구조체 해당 구조체에 알맞게 자료형을 넣을 수 있음
//           16 Byte = int(4 Byte) * 4 = short(2 Byte) * 8
// _mm_set_epi32()
// _mm_load_si128() : 캐스팅할 것
// _mm_setzero_si128() : xor로 처리되어 속도 빠름
// _mm_set1~ : 한 가지 값으로 구조체 셋

// _mm_shuffle_ps (__m128 a, __m128 b, int imm8)
// imm8은 이진수로 순서를 지정
// ex) 3210 -> 2130을 하고자 한다면
//     2(10) 1(01) 3(11) 0(00) = 10011100 = 0x9c

// unpackhi_epi32(a,b), unpacklo_epi32(a,b)
// ex) A = a b c d
//	   B = e f g h
// unpackhi_epi32(A, B) = b f a e
// unpacklo_epi32(A, B) = d h c g
