#ifndef _GEOM_H_
#define _GEOM_H_

#include <common.h>
#include <vector.h>
#include <matrix.h>

namespace cls
{
	class ray;
	class color;

	class camera
	{
	private:
		// *** Clipping planes

		vector3 m_Normals[4];
		float   m_Distance[4];
		vector3 m_Up;
		vector3 m_Left;

	public:
		// *** Position and orientation

		vector3 m_Position;
		vector3 m_Direction;
		float   m_Roll;
		float   m_FOV;

		camera() { }
		
		camera(
			const vector3& p_Position,
			const vector3& p_Direction,
			const float p_Roll, 
			const float p_FOV = 80)
		{
			m_Position = p_Position;
			m_Direction = p_Direction;
			m_Roll = p_Roll;
			m_FOV = p_FOV;

			m_Direction.normalize();
		}


		// *** Initialize the GL matrices to match the camera

		void activate();


		// *** Determine whether the given bounding box is inside
		//     the frustum.

		bool inside(
			const vector3& p_Mins,
			const vector3& p_Maxs) const;

		const vector3& get_up() const
		{
			return m_Up;
		}

		const vector3& get_left() const
		{ 
			return m_Left;
		}
	};

	class geom
	{
	public:
		virtual ~geom();

		// *** Perform raytracing. Return 24 bit RGB value

		virtual color trace_ray(const ray p_Ray);

		// *** Set or unset the states needed to render

		virtual void set_state(bool p_Activate = true);

		// *** Render the scene with the given preactivated camera

		virtual void render(const camera p_Camera);
	};

	// *** Predefined objects

	class vertex_geom : public geom
	{
	public:
		enum face_type
		{
			TRIANGLES = 1, 
			TRIANGLE_STRIP = 2, 
			QUADS = 3, 
			QUAD_STRIP = 4, 
			INDEXED_TRIANGLES = 5, 
			INDEXED_QUADS = 6
		};

		face_type m_FaceType;
		size_t    m_VertexCount;
		vector3*  m_Vertices;
		size_t    m_TexLevelCount;
		vector2** m_TexCoords;
		size_t    m_IndexCount;
		size_t*   m_Indices;
		vector3*  m_Normals;
		vector3*  m_FaceNormals;

		virtual void  calc_normals();
		virtual color trace_ray(const ray p_Ray);
		virtual void  set_state(bool p_Activate = true);
		virtual void  render(const camera p_Camera);
	};

	class aabox : public vertex_geom
	{
	public:
		aabox(
			const vector3& p_Mins, 
			const vector3& p_Maxs, 
			size_t p_TextureLevels = 0);
	};

	class sphere : public vertex_geom
	{
	public:
		sphere(
			size_t p_Quality = 3, 
			size_t p_TextureLevels = 0);

		color trace_ray(const ray p_Ray);
	};
}

#endif // !_GEOM_H_

