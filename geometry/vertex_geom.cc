#include <stdexcept>
#include <stdlib.h>
#include <GL/gl.h>

#include <vector.h>
#include <geom.h>
#include <raytracer.h>
#include <color.h>

namespace cls
{
	color vertex_geom::trace_ray(const ray p_Ray)
	{
		// XXX: fixme

		return color(0, 0, 0, 0);
	}

	void vertex_geom::calc_normals()
	{
		if(m_FaceType == INDEXED_TRIANGLES)
		{
			for(size_t i_Face = 0; i_Face < m_IndexCount / 3; i_Face++)
			{
				m_FaceNormals[i_Face] = 
					(m_Vertices[m_Indices[i_Face * 3 + 1]] - m_Vertices[m_Indices[i_Face * 3]]).cross
					(m_Vertices[m_Indices[i_Face * 3 + 2]] - m_Vertices[m_Indices[i_Face * 3]]).normalize();
			}

			for(size_t i_Vertex = 0; i_Vertex < m_VertexCount; i_Vertex++)
			{
				size_t l_Count = 0;
				m_Normals[i_Vertex] = vector3(0, 0, 0);

				for(size_t i_Face = 0; i_Face < m_IndexCount / 3; i_Face++)
				{
					if((m_Indices[i_Face * 3] == i_Vertex)
					|| (m_Indices[i_Face * 3 + 1] == i_Vertex)
					|| (m_Indices[i_Face * 3 + 2] == i_Vertex))
					{
						m_Normals[i_Vertex] += m_FaceNormals[i_Face];
						l_Count++;
					}
				}

				m_Normals[i_Vertex] /= (float) l_Count;
			}
		}
	}

	void vertex_geom::set_state(bool p_Activate)
	{
	}

	void vertex_geom::render(const camera p_Camera)
	{
		switch(m_FaceType)
		{
		case TRIANGLES:
		case INDEXED_TRIANGLES:

			glBegin(GL_TRIANGLES);

			break;

		case TRIANGLE_STRIP:

			glBegin(GL_TRIANGLE_STRIP);

			break;

		case QUADS:
		case INDEXED_QUADS:

			glBegin(GL_QUADS);

			break;

		case QUAD_STRIP:

			glBegin(GL_QUAD_STRIP);

			break;

		default:

			throw std::runtime_error("Invalid face type");
		}

		if(m_FaceType == INDEXED_TRIANGLES || m_FaceType == INDEXED_QUADS)
		{
			// XXX: do this the OpenGL way

			for(size_t i_Index = 0; i_Index < m_IndexCount; i_Index++)
			{
				for(size_t i_TexLevel = 0; 
				           i_TexLevel < m_TexLevelCount; i_TexLevel++)
				{
					glMultiTexCoord2fvARB(
						GL_TEXTURE0_ARB + i_TexLevel, 
						m_TexCoords[i_TexLevel][m_Indices[i_Index]].get_data());
				}

				glNormal3fv(m_Normals[m_Indices[i_Index]].get_data());
				glVertex3fv(m_Vertices[m_Indices[i_Index]].get_data());
			}
		}
		else
		{
			for(size_t i_Vertex = 0; i_Vertex < m_VertexCount; i_Vertex++)
			{
				for(size_t i_TexLevel = 0; 
				           i_TexLevel < m_TexLevelCount; i_TexLevel++)
				{
					glMultiTexCoord2fvARB(
						GL_TEXTURE0_ARB + i_TexLevel, 
						m_TexCoords[i_TexLevel][i_Vertex].get_data());
				}

				glNormal3fv(m_Normals[i_Vertex].get_data());
				glVertex3fv(m_Vertices[i_Vertex].get_data());
			}
		}

		glEnd();
	}
};

