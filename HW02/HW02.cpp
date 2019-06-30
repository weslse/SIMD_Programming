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
		__m128 vData = _mm_loadu_ps(p + i); // �迭 �ε�
		
		// from�� ���� ��ġ ã��
		__m128 vCmpData = _mm_cmpeq_ps(vData, vFrom); 
		
		// from�� ���� ���� �ƴ� ���� �� ����
		vData = _mm_andnot_ps(vCmpData, vData); 
		
		// to�� ������ ��ġ�� to�� �ֱ�
		__m128 vToData = _mm_and_ps(vCmpData, vTo);
		
		// ������ ���� to�� �ִ� ����
		// from�� ���� ���� ������ ���� ���ؼ� 
		// ������ ����
		vData = _mm_add_ps(vData, vToData);
		
		// �迭�� ����
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
		// �迭���� �ε�
		__m128 vData = _mm_loadu_ps(p + i);
		
		// f1���� ū ���� ���� ����ũ ����
		__m128 vB1 = _mm_cmpgt_ps(vData, vF1);
		// f2���� ���� ���鿡 ���� ����ũ ����
		__m128 vB2 = _mm_cmplt_ps(vData, vF2);
		// �� ����ũ�� ���ļ� ������ �˸´� ����ũ ����
		__m128 vBound = _mm_and_ps(vB1, vB2);
		
		// ������ ���� �ʴ� ������ ����
		vData = _mm_andnot_ps(vBound, vData);

		// ������ to���� ���� ���� ����
		__m128 vToData = _mm_and_ps(vBound, vTo);

		// ������ ���� to�� �ִ� ����
		// ������ �����ʾ� ������ ����
		// ���Ͽ� ������ ����
		vData = _mm_add_ps(vData, vToData);

		// �迭�� ����
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
