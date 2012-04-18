#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "JitteredSampler.h"
#include "RaytraceException.h"


JitteredSampler::JitteredSampler()
{
	m_currentSample = 0;
}


void JitteredSampler::Generate(int sampleCount)
{
	// Reset current samples.
	m_samples.clear();
	m_currentSample = 0;

	// Make sure that the number of samples is a perfect square.
	int sqrtOfSampleCount = (int)sqrt(sampleCount);
	if ((sqrtOfSampleCount * sqrtOfSampleCount) != sampleCount)
	{
		char buffer[128];
		sprintf(buffer, "Unable to construct jittered samples: %i is not a perfect square!", sampleCount);
		throw RaytraceException(buffer);
	}

	// Divide the area up into a grid of equally-sized squares.
	// Calculate the number of grid squares for both the width and the height of the area.
	int gridUnitsPerSide = sqrtOfSampleCount;

	// Calculate the side length of a single grid square.
	double sideLengthOfUnit = 1.0 / gridUnitsPerSide;

	// Generate a ray for each grid unit.
	for (int gridY = 0; gridY < gridUnitsPerSide; gridY++)
	{
		for (int gridX = 0; gridX < gridUnitsPerSide; gridX++)
		{
			// Generate a random position inside the current square, and map it into grid space.
			double sampleX = (drand48() + gridX) * sideLengthOfUnit;
			double sampleY = (drand48() + gridY) * sideLengthOfUnit;
			m_samples.push_back(std::make_pair(sampleX, sampleY));
		}
	}
}


const Sample &JitteredSampler::GetCurrentSample() const
{
	ThrowIfNoSamples();

	return (m_samples[m_currentSample % m_samples.size()]);
}


void JitteredSampler::Next()
{
	ThrowIfNoSamples();

	m_currentSample++;
}


const SampleList& JitteredSampler::GetSampleList() const
{
	return (m_samples);
}


void JitteredSampler::ThrowIfNoSamples() const
{
	if (m_samples.empty())
	{
		throw RaytraceException("Tried to do something before calling JitteredSampler::Generate()!");
	}
}

