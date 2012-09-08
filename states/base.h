#ifndef _BASE_H_
#define _BASE_H_

#include <state.h>
#include <geom.h>
#include <spline.h>

namespace cls
{
	class base : public state
	{
	private:
		typedef spline<vector3> vector_spline;

		image        m_Green;
		image        m_EnvMap;
		image        m_LitCorners;
		image        m_RefractMap;
		geom*        m_Geometry;
		vertex_geom* m_RefSphere;
		vertex_geom* m_DynamicBlob;
		vertex_geom* m_StaticBlob;
		vertex_geom* m_Sphere;
		vertex_geom* m_DynamicBlob2;
		camera       m_Camera;

		vector_spline* m_EyeSplines[100];
		vector_spline* m_TargetSplines[100];

		void effect0(size_t p_Time);
		void effect1(size_t p_Time);
		void effect2(size_t p_Time);
		void effect3(size_t p_Time);
		void effect4(size_t p_Time);
		void effect5(size_t p_Time);
		void effect6(size_t p_Time);
		void effect7(size_t p_Time);
		void post_uv(float p_X, float p_Y, float p_Z, const vector3& p_Normal) const;

	public:
		base(const char* p_Name, geom* p_Geometry);

		void on_enter(void* p_Parameter);
		void on_leave();
		void on_display();
	};
}

#endif

