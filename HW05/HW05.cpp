#include <iostream>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <pmmintrin.h>
#include <immintrin.h>

using namespace std;

void DotProduct(short* a, short* b, int* c, int size)
{
	for (int i = 0; i < size; i += 2)
	{
		__m128i vA1 = _mm_loadu_si128((__m128i*)(a + i * 4));
		__m128i vB1 = _mm_loadu_si128((__m128i*)(b + i * 4));
		__m128i vC1 = _mm_madd_epi16(vA1, vB1); // °ö¼À ÈÄ µ¡¼À 
		// a b c d
		// e f g h
		// ae + bf , cg+dh
		// ºñÆ® ¼ö Áõ°¡

		__m128i vX = _mm_hadd_epi32(vC1, vC1); // °¡·Î µ¡¼À a,b,c,d -> a+b, c+d (ºñÆ®¼ö Áõ°¡)

		_mm_storel_epi64((__m128i*)(c + i), vX);
	}
}

// back to front
__m128 alphablend(float r, float g, float b, float a,
	float tr, float tg, float tb)
{
	__m128 vColor = _mm_set_ps(r, g, b, 0.0f);
	__m128 vTotalColor = _mm_set_ps(tr, tg, tb, 0.0f);
	__m128 vAlpha = _mm_set1_ps(a);
	__m128 vOneMinusAlpha = _mm_set1_ps(1 - a);
	vColor = _mm_mul_ps(vColor, vAlpha);
	vTotalColor = _mm_mul_ps(vTotalColor, vOneMinusAlpha);

	//	tr = a * r + tr * (1 - a);
	//	tg = a * g + tg * (1 - a);
	//	tb = a * b + tb * (1 - a);

	return _mm_add_ps(vColor, vTotalColor);
}


int main()
{
	//short a[16] = { 1,2,3,4, 5,6,7,8, 1,0,3,4, 5,6,0,8 };
	//short b[16] = { 1,2,3,4, 5,6,7,8, 1,2,0,4, 5,0,7,8 };
	//int c[4];
	//DotProduct(a, b, c, 4);
	//for (int i = 0; i < 4; ++i)
	//{
	//	printf("%d ", c[i]);
	//}
	//printf("\n");

	//__m128 vA = _mm_set_ps(1.7, 3.1, -2.9, -2.1);
	//__m128i vIA = _mm_cvttps_epi32(vA);


	__m128 rgba = alphablend(0.2, 0.2, 0.2, 0.7, 0.4, 0.4, 0.4);
	for (int i = 0; i < 4; ++i)
	{
		printf("%f ", rgba.m128_f32[3 - i]);
	}
	printf("\n");

	
	// newton rhapson method code

	float t = 0.4512f;
	__m128 x = _mm_set1_ps(t);
	__m128 vThreeHalf = _mm_set1_ps(1.5f);
	__m128 vHalf = _mm_set1_ps(0.5f);
	__m128 y = _mm_rsqrt_ps(x); // yn
	__m128 y2 = _mm_mul_ps(_mm_mul_ps(x, y), y); // x * yn * yn
	y2 = _mm_sub_ps(vThreeHalf, _mm_mul_ps(y2, vHalf)); // 3/2 - x*yn*yn/2
	y2 = _mm_mul_ps(y, y2); // yn * (3/2 - x*yn*yn/2)

	printf("rsqrt(%f) = %f\n", t, y.m128_f32[0]);
	printf("NR method -> rsqrt(%f) = %f\n", t, y2.m128_f32[0]);
	printf("1/sqrt(%f) = %f\n", t, 1.f / sqrtf(t));
}