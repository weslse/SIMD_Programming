#include <iostream>
#include <emmintrin.h>

using namespace std;

// HW 0301
void BGRA2RGBA(float *bgra, float *rgba, int size)
{
	for (int i = 0; i < size; i += 4)
	{
		// BGRA���� load
		__m128 vBGRA = _mm_loadu_ps(bgra + i);
		
		// 0���� set
		__m128 vRGBA = _mm_setzero_ps();
		
		// BGRA 0 1 2 3 �̴ϱ�
		// RGBA 2 1 0 3
		// �д� ���� <- �̹Ƿ�
		// 3012 -> 11000110 = 0xc6
		vRGBA = _mm_shuffle_ps(vBGRA, vBGRA, 0xc6);

		// ���� rgba ����
		_mm_storeu_ps(rgba, vRGBA);
	}
}

// HW 0302
int CountNumber(float *colors, float value, int size)
{
	// ���� ���� ���� ���� ����
	__m128 vValue = _mm_set1_ps(value);
	int cnt = 0; // ����
	for (int i = 0; i < size; i += 4)
	{
		// ��Ŀ��� ���� load
		__m128 vColors = _mm_loadu_ps(colors + i);
		
		// ���ϴ� ���� ��
		__m128 vCmp = _mm_cmpeq_ps(vColors, vValue);
		
		// movemask�� �̿��Ͽ� 
		// ��� ��ġ�� value���� ������ ����
		int maskNum = _mm_movemask_ps(vCmp);
		
		// ������ ���� 0 1 0 0 �̹Ƿ�
		// num�� ����ϴ� ����
		for (int i = 4; i > 0; --i)
		{
			// �ش� ��ġ�� ��� 
			// ������ 3 2 1 0 �̹Ƿ�
			// i - 1��ŭ shift ���ش�.
			int divNum = (1 << (i - 1));
			
			// �ش� ��ġ�� 1�̸� cnt ����
			cnt += (maskNum / divNum);

			// ���� �ڸ� �˻� ���� ������
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

// __m128i : 16 Byte �� ����ü �ش� ����ü�� �˸°� �ڷ����� ���� �� ����
//           16 Byte = int(4 Byte) * 4 = short(2 Byte) * 8
// _mm_set_epi32()
// _mm_load_si128() : ĳ������ ��
// _mm_setzero_si128() : xor�� ó���Ǿ� �ӵ� ����
// _mm_set1~ : �� ���� ������ ����ü ��

// _mm_shuffle_ps (__m128 a, __m128 b, int imm8)
// imm8�� �������� ������ ����
// ex) 3210 -> 2130�� �ϰ��� �Ѵٸ�
//     2(10) 1(01) 3(11) 0(00) = 10011100 = 0x9c

// unpackhi_epi32(a,b), unpacklo_epi32(a,b)
// ex) A = a b c d
//	   B = e f g h
// unpackhi_epi32(A, B) = b f a e
// unpacklo_epi32(A, B) = d h c g
