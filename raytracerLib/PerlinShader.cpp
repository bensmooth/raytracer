#include "PerlinShader.h"
#include "Perlin.h"
#include "Intersection.h"
#include "BlinnPhongShader.h"
#include "ColorGradient.h"


using namespace sivelab;


PerlinShader::PerlinShader(Scene* scene)
{
	m_scene = scene;
}


PerlinShader::~PerlinShader()
{
}


Color PerlinShader::DirtyMirror(Intersection& intersection)
{
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);
	ColorGradient gradient(Color(0.75, 0.8, 1.0), Color(0.1, 0.1, 0.1));

	// Calculate and normalize noise to the range [0,1].
	double noise = PerlinNoise3D(intersectPoint[0], intersectPoint[1], intersectPoint[2], 2.0, 2.0, 8);
	noise /= 0.7;
	//noise = sin(noise + intersectPoint[0]);
	noise = (noise + 1.0) / 2.0;

	// Calculate the reflective part of the scene.
	BlinnPhongShader blinnPhong(m_scene, gradient.GetStart(), Color(1.0, 1.0, 1.0), 320.0, noise, 0.0);
	Color diffuse = blinnPhong.Shade(intersection);

	return (diffuse);
}


Color PerlinShader::BrushedMetal(Intersection& intersection)
{
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);
	ColorGradient gradient(Color(0.6, 0.6, 0.7), Color(0.1, 0.1, 0.1));

	// Calculate and normalize noise to the range [0,1].
	double noise = PerlinNoise3D(intersectPoint[0] / 100000.0, intersectPoint[1], intersectPoint[2] / 100000.0, 2.0, 2.0, 8);
	noise /= 0.7;
	noise = (noise + 1.0) / 2.0;

	// Calculate the reflective part of the scene.
	BlinnPhongShader blinnPhong(m_scene, gradient.GetStart(), Color(1.0, 1.0, 1.0), 320.0, 0.0, 0.0);
	Color diffuse = blinnPhong.Shade(intersection);

	// Combine the blinnPhong color and the noise color.
	diffuse.AddColors(gradient.Sample(noise));
	diffuse.LinearMult(0.5);
	return (diffuse);
}


Color PerlinShader::Marble(Intersection& intersection)
{
	Vector3D intersectPoint = intersection.collidedRay.GetPositionAtTime(intersection.t);
	ColorGradient gradient(Color((uint8_t)17, 8, 1), Color((uint8_t)180, 154, 141));

	// Calculate and normalize noise.
	double noise = PerlinNoise3D(intersectPoint[0], intersectPoint[1], intersectPoint[2], 2.0, 3.0, 5);
	noise /= 0.7;

	// Take sine of noise and a coordinate to get some periodicity.
	noise = sin(noise + intersectPoint[0]);

	// Normalize to [0,1].
	noise = (noise + 1.0) / 2.0;

	// Calculate the reflective part of the scene.
	BlinnPhongShader blinnPhong(m_scene, gradient.GetStart(), Color(1.0, 1.0, 1.0), 320.0, 0.05, 0.0);
	Color diffuse = blinnPhong.Shade(intersection);

	// Combine the blinnPhong color and the noise color.
	diffuse.AddColors(gradient.Sample(noise));
	return (diffuse);
}


Color PerlinShader::Shade(Intersection& intersection)
{
//	return (BrushedMetal(intersection));
	return (DirtyMirror(intersection));
//	return (Marble(intersection));
}

