class Scene
{
public:
	ICamera *camera;
	vector<const IObject const *> objects;
	vector<const ILight const *> lights;
	vector<const IShader const *> shaders;
	
	void Render(string filename);
	
	bool LoadXmlScene(string filename);
};


class IObject
{
	/**
	 * Sees if the given ray intersects with the object.
	 * @param ray The ray to test for intersection.
	 * @param result If true is returned, this will contain the intersection data.
	 * @return True if there was an intersection, false if there was not.
	 */
	virtual bool Intersect(const Ray &ray, Intersection &result) = 0;


	/**
	 * Gets the shader associated with this object.
	 */
	virtual IShader *GetShader() = 0;
};


class IShader
{
	/**
	 * Calculates the color of a ray intersection based on intersection data.
	 * @param intersection The intersection data to shade with.
	 */
	virtual Color Shade(Intersection &intersection) = 0;
};


class ICamera
{
public:
	Ray positionAndDirection;
	
	/**
	 * Calculates the viewing ray for a given image coordinate.
	 */
	virtual Ray CalculateViewingRay(double imageX, double imageY) = 0;
};


class ILight
{
public:
	/**
	 * Gets the radiance that should be cast by the light at the given position, assuming no other objects are in the way.
	 */
	virtual Color GetRadiance(const Vector &position) = 0;
};


/**
 * This class represents a ray in 3D space.
 */
class Ray
{
public:
	/**
	 * Constructs a ray with a position vector of (0, 0, 0), and a direction vector of (1,0,0).
	 */
	Ray();


	/**
	 * Constructs a ray based on a given position and direction.
	 * @param position The starting position of the ray.
	 * @param direction The direction of the ray.  This will be normalized.
	 */
	Ray(const Vector &position, const Vector &direction);


	/**
	 * Gets a reference to the starting position of the ray.
	 */
	Vector &GetPosition();


	/**
	 * Gets a constant reference to the normalized direction of the ray.
	 */
	const Vector &GetDirection();


	/**
	 * Sets the direction of the ray.
	 * @param direction The new direction.  Will be normalized.
	 */
	void SetDirection(const Vector &direction);

private:
	/**
	 * The starting position of the ray.
	 */
	Vector m_position;

	/**
	 * The normalized direction of the ray.
	 */
	Vector m_direction;
};


/**
 * This structure represents an intersection.
 * It should contain enough information to shade the object that was collided with.
 */
struct Intersection
{
	/**
	 * The ray that collided with the object.
	 */
	Ray collidedWith;

	/**
	 * The normal of the surface where the ray hit.
	 */
	Vector surfaceNormal;

	/**
	 * A reference to the object that was intersected.
	 */
	IObject *object;
};

