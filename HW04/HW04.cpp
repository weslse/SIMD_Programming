#include <emmintrin.h>
#include <iostream>

using namespace std;

// 벡터 배열을 받아 단위벡터로 만들어주는 함수
void Normalize(float* vectors, float* normalized, int size)
{

	for (int i = 0; i < size; i++)
	{
		__m128 vVector = _mm_loadu_ps(vectors + (4 * i));
		__m128 vVectorSq = _mm_mul_ps(vVector, vVector);
		
		__m128 vHi = _mm_unpackhi_ps(vVectorSq, vVectorSq);
		__m128 vLo = _mm_unpacklo_ps(vVectorSq, vVectorSq);

		__m128 vHiHi = _mm_unpackhi_ps(vHi, vHi);
		__m128 vHiLo = _mm_unpacklo_ps(vHi, vHi);

		__m128 vLoHi = _mm_unpackhi_ps(vLo, vLo);
		__m128 vLoLo = _mm_unpacklo_ps(vLo, vLo);

		__m128 vLenSQ = _mm_add_ps(vHiHi, vHiLo);
		vLenSQ = _mm_add_ps(vLenSQ, vLoHi);


		// 벡터의 크기 = root(x*x+y*y+z*z)
		__m128 vInvLen = _mm_rsqrt_ps(vVectorSq);
		__m128 vNormalized = _mm_mul_ps(vVector, vInvLen);
		_mm_storeu_ps(normalized + (4 * i), vNormalized);
	}
}

int main()
{
	float vectors[2][4] = { {2, 3, 1, 0}, {3, 4, 5, 0} };
	float normalized[2][4];

	// 주어진 벡터를 단위벡터로 만들어 저장하라
	// 마지막 성분의 값은 0이다.
	Normalize((float*)vectors, (float*)normalized, 2);

	for (int i = 0; i < 2; i++)
	{
		printf("%f %f %f\n", normalized[i][0], normalized[i][1], normalized[i][2]);
	}

	return 0;
}

// 고정 소수점 연산
// int <-> float 연산의 비용은 크다 (저장 방식이 다르기 때문)
// _mm_mulhi_epi(u)16()
// : 곱셈(*)을 하고 하위 비트를 shift연산으로 날려줌
// -> 16 * 16 = 최대 32비트 --> Overflow!
// 약간의 오차를 감수하고 하위비트를 shift하여 제거

//void PrintM128i(__m128i v)
//{
//	for (int i = 0; i < 8; i++)
//		printf("%d ", v.m128i_i16[i]);
//	printf("\n");
//}
//
//int main()
//{
//	short img[9] = { 10,20,30,40,50,60,70,80,90 };
//	short out[8];
//
//	float w1 = 0.6, w2 = 0.4;
//	short shortW1 = w1 * 65536, shortW2 = w2 * 65536;
//
//	__m128i vW1 = _mm_set1_epi16(shortW1);
//	__m128i vW2 = _mm_set1_epi16(shortW2);
//	
//
//	// val = i[i] * w1 + i[i+1] *w2;
//	__m128i vBefore = _mm_loadu_si128((__m128i*)img);
//	__m128i vAfter = _mm_loadu_si128((__m128i*)(img + 1));
//	
//	__m128i vHalf = _mm_avg_epu16(vBefore, vAfter);
//
//	__m128i vT1 = _mm_mulhi_epu16(vBefore, vW1);
//	__m128i vT2 = _mm_mulhi_epu16(vAfter, vW2);
//
//	__m128i vOut = _mm_add_epi16(vT1, vT2);
//	
//	_mm_storeu_si128((__m128i*)out, vOut);
//	
//	__m128i vHalfTwice = _mm_slli_epi16(vHalf, 1);
//
//	PrintM128i(vHalfTwice);
//}

// 3:1 or 1:1 같은 경우 avg 함수를 이용하여 계산하면 편함(빠름)

// shift : _mm_slli_epi() 왼쪽 shift
//       : _mm_srli_epi() 오른쪽 shift