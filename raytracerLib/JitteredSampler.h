#include <vector>


/**
 * Represents a single sample.
 */
typedef std::pair<double, double> Sample;

/**
 * A list of samples.
 */
typedef std::vector<Sample> SampleList;

/**
 * Generates jittered samples on the unit square.
 */
class JitteredSampler
{
public:
	/**
	 * Sets up the sampler.
	 */
	JitteredSampler();

	/**
	 * Actually generates the samples.
	 * The current sample is set to the first one generated.
	 * @param sampleCount The number of samples to generate.  Must be a perfect square, or an exception will be thrown.
	 */
	void Generate(int sampleCount);

	/**
	 * Gets a constant reference to the list of samples.
	 * @remarks Must be called after Generate().
	 */
	const SampleList &GetSampleList() const;

	/**
	 * Gets the current sample.
	 * @remarks Must be called after Generate().
	 */
	const Sample &GetCurrentSample() const;

	/**
	 * Steps the current sample to the next sample in the list.
	 * Calling this more than the number of samples generated will result in wrapping around to the beginning of the list.
	 * @remarks Must be called after Generate().
	 */
	void Next();

private:
	/**
	 * Throws an exception if no samples have been generated yet.
	 */
	void ThrowIfNoSamples() const;

	SampleList m_samples;

	int m_currentSample;
};
