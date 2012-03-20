/* Perlin.h */
#ifndef PERLIN_H
#define PERLIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define setup(i,b0,b1,r0,r1)\
t = vec[i] + N;\
b0 = ((int)t) & BM;\
b1 = (b0+1) & BM;\
r0 = t - (int)t;\
r1 = r0 - 1.;
#define at2(rx,ry) ( rx * q[0] + ry * q[1] )
#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

double PerlinNoise1D(double x, double alpha, double beta, int n);
double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);


#ifdef __cplusplus
}
#endif

#endif
