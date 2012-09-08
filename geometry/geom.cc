#include <stdexcept>

#include <geom.h>
#include <vector.h>
#include <raytracer.h>
#include <color.h>

namespace cls
{
	geom::~geom()
	{
		throw std::runtime_error("Default geom::~geom() called");
	}

	color geom::trace_ray(const ray p_Ray)
	{
		throw std::runtime_error("Default geom::trace_ray() called");
	}

	void geom::set_state(const bool p_Activate)
	{
		throw std::runtime_error("Default geom::set_state() called");
	}

	void geom::render(const camera p_Camera)
	{
		throw std::runtime_error("Default geom::render() called");
	}
};

cls::vector2 operator*(const float p_Value, const cls::vector2& p_Vector)
{
	return p_Vector * p_Value;
}

cls::vector3 operator*(const float p_Value, const cls::vector3& p_Vector)
{
	return p_Vector * p_Value;
}

