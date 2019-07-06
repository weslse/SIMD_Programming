#include <iostream>
#include <emmintrin.h>

using namespace std;

// HW 0301
void BGRA2RGBA(float *bgra, float *rgba, int size)
{

}

// HW 0302
int CountNumber(float *colors, float value, int size)
{
	return 1;
}

int main()
{
	// HW 0301
	float bgr[4] = { 0,0.3,0.9,1 };
	float rgb[4];
	BGRA2RGBA(bgr, rgb, 4);
	cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << " " << rgb[3] << endl;

	// HW 0302
	__m128 a = _mm_set_ps(-1, 1, -2, -3);
	cout << "mask = " << _mm_movemask_ps(a) << endl;

	float colors[4] = { 0, 0.3, 0.9, 1 };
	int num = CountNumber(colors, 0.3, 4);
	cout << "count = " << num << endl;
}