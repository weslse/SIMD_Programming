#include <emmintrin.h>
#include <pmmintrin.h>
#include <iostream>

using namespace std;

// 벡터 배열을 받아 단위벡터로 만들어주는 함수
void Normalize(float* vectors, float* normalized, int size)
{
	// * vector 값을 나열할 때는 0 1 2 3 방향으로 나열하였음
	for (int i = 0; i < size; i++)
	{
		// vector의 값 a b c d
		__m128 vVector = _mm_loadu_ps(vectors + (4 * i));

		// 제곱계산 a^2 b^2 c^2 d^2
		__m128 vVectorSq = _mm_mul_ps(vVector, vVector);

		// 상위 비트만 남기기
		// 연산이 3 2 1 0 방향으로 진행
		//        d c b a
		//        d c b a
		//      1.  |		(아래방향으로 읽기)
		//      2.|
		// c^2 c^2 d^2 d^2
		__m128 vVectorC2D2 = _mm_unpackhi_ps(vVectorSq, vVectorSq);

		// 하위 비트만 남기기
		// 연산이 3 2 1 0 행 순으로 진행
		//        d c b a
		//        d c b a
		//      1.      |   (아래방향으로 읽기)
		//      2.    |
		// a^2 a^2 b^2 b^2
		__m128 vVectorA2B2 = _mm_unpacklo_ps(vVectorSq, vVectorSq);

		// 각 원소 값만 추출
		// D가 문제에서는 0이므로 구할 필요 없음
		// d^2 d^2 d^2 d^2
		//__m128 vVectorD2 = _mm_unpackhi_ps(vVectorC2D2, vVectorC2D2);

		// c^2 c^2 c^2 c^2
		__m128 vVectorC2 = _mm_unpacklo_ps(vVectorC2D2, vVectorC2D2);

		// b^2 b^2 b^2 b^2
		__m128 vVectorB2 = _mm_unpackhi_ps(vVectorA2B2, vVectorA2B2);

		// a^2 a^2 a^2 a^2
		__m128 vVectorA2 = _mm_unpacklo_ps(vVectorA2B2, vVectorA2B2);

		// 각 제곱값을 모두 더한다.
		__m128 vLenSq = _mm_add_ps(vVectorA2, vVectorB2);
		vLenSq = _mm_add_ps(vLenSq, vVectorC2);
		
		// hadd
		__m128 vLenSq2 = _mm_hadd_ps(vVectorSq, vVectorSq);
		vLenSq2 = _mm_hadd_ps(vLenSq2, vLenSq2);
		//vLenSq = _mm_add_ps(vLenSq, vVectorD2);

		// _mm_rsqrt_ps()는 값마다 APROXIMATE(1/sqrt()) 계산을 한다.
		// Approximate 했기 때문에 약간의 오차가 발생
		// but, 속도가 빠름(?)
		//__m128 vInvLen = _mm_rsqrt_ps(vLenSq);
		__m128 vInvLen = _mm_rsqrt_ps(vLenSq2);

		// sqrt, div 등을 이용하여 속도가 느림
		// 정밀한 계산은 아래와 같다. 
		//__m128 vInvLen = _mm_sqrt_ps(vLenSq);
		//__m128 vOne = _mm_set1_ps(1);
		//vInvLen = _mm_div_ps(vOne, vInvLen);

		// 곱셈
		__m128 vNormalized = _mm_mul_ps(vVector, vInvLen);

		// 저장
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

		//printf("%f %f %f\n", 
		//	vectors[i][0] / sqrtf(vectors[i][0] * vectors[i][0] + vectors[i][1] * vectors[i][1] + vectors[i][2] * vectors[i][2]), 
		//	vectors[i][1] / sqrtf(vectors[i][0] * vectors[i][0] + vectors[i][1] * vectors[i][1] + vectors[i][2] * vectors[i][2]),
		//	vectors[i][2] / sqrtf(vectors[i][0] * vectors[i][0] + vectors[i][1] * vectors[i][1] + vectors[i][2] * vectors[i][2]));
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