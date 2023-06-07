#include <omp.h>
#include <iostream>
#include <vector>

using namespace std;


int main()
{
	omp_lock_t lock;
	const int NumPoints = 5;
	auto list = new float[2][NumPoints];
	list[0][0] = 1; list[1][0] = 1;
	list[0][1] = 0; list[1][1] = 0;
	list[0][2] = 2; list[1][2] = 2;
	list[0][3] = 1.5; list[1][3] = 1.5;
	list[0][4] = 1.5; list[1][4] = 1.5;

	float midX = 0;
	float midY = 0;
	int i = 0;
	omp_init_lock(&lock);
#pragma omp parallel for private(i) reduction(+:midX,midY)
	for (i = 0; i < NumPoints; i++)
	{
		midX += list[0][i];
		midY += list[1][i];
	}
	midX = midX / (NumPoints-1);
	midY = midY / (NumPoints -1);
	float longX = NULL;
	float radX = NULL;
	float longY = NULL;
	float radY = NULL;
#pragma omp parallel for shared(radX, radY) private(longX, longY, i)
	for (int i = 0; i < NumPoints; i++)
	{
		int tempX = midX - list[0][i];
		int tempY = midY - list[1][i];
		if ((longX < tempX) || (longX == NULL))
		{
			longX = tempX;
		}
		if ((longY < tempY) || (longY == NULL))
		{
			longY = tempY;
		}
		omp_set_lock(&lock);
		if ((radX < longX) || (radX == NULL))
		{
			radX = longX;
		}
		if ((radY < longY) || (radY == NULL))
		{
			radY = longY;
		}
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
	float rad = max(radX, radY);
	cout << "circle center coords is X: " << midX << " Y: " << midY << endl;
	cout << "radius is: " << rad << endl;
}

