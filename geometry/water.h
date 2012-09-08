#ifndef _WATER_H_
#define _WATER_H_

namespace cls
{
	class water : public geom
	{
	private:
		image*   m_RefractionMap;
		image*   m_ReflectionMap;
		float*   m_Elevations;
		vector3* m_Normals;

	public:
		water(
			const vector3& p_UpperLeft, 
			const vector3& p_XDirection, 
			const vector3& p_YDirection, 
			const geom& p_Containter);

		~water();

		// *** Advance timer

		void advance(double p_DeltaTime);

		// *** Set or unset the states needed to render

		void set_state(bool p_Activate = true);

		// *** Render the scene with the given preactivated camera

		void render(const camera p_Camera);
	};
};

#endif // !_WATER_H_

