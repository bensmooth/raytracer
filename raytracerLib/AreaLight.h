#include "ILight.h"
#include "JitteredSampler.h"


/**
 * Represents a light that has rectangular dimensions.
 */
class AreaLight : public ILight
{
public:
	AreaLight(const sivelab::Vector3D &position, const sivelab::Vector3D &normal, const Color &intensity, double width, double height);
	virtual ~AreaLight();

	virtual sivelab::Vector3D GetPosition() const;
	virtual Color GetRadiance(const sivelab::Vector3D& position) const;

	/**
	 * Gets the position on the light (in world coordinates) using the given sample.
	 */
	sivelab::Vector3D GetPosition(const Sample &sample) const;
private:
	sivelab::Vector3D m_position;
	Color m_intensity;
	sivelab::Vector3D m_normal;
	double m_width;
	double m_height;
};
