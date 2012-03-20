#include <stdio.h>
#include "Perlin.h"


double Norm(double x, double y, double z, double alpha, double beta, int n)
{
	double perlin = PerlinNoise3D(x, y, z, alpha, beta, n);
	return (perlin);
}


void SamplePerlin(double a, double b, int n)
{
	double max = 0.0;
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			for (int z = 0; z < 100; z++)
			{
				double perlin = Norm(x/10.0, y/10.0, z/10.0, a, b, n);
				if (max < perlin)
				{
					max = perlin;
				}
			}
		}
	}
	printf(" max = %.4f\n", max);
}


int main()
{
	double alpha = 4.0;
	double beta = 0.25;
	int n = 1;
	for (int i = 0; i < 12; i++)
	{
		printf("alpha = %.2f, beta = %.2f, n = %i:", alpha, beta, n);
		SamplePerlin(alpha, beta, n);
		n += 1;
	}
}

