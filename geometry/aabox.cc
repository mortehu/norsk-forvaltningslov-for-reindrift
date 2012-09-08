#include <stdlib.h>

#include <geom.h>
#include <vector.h>

namespace cls
{
	aabox::aabox(
		const vector3& p_Mins, 
		const vector3& p_Maxs, 
		size_t p_TextureLevels)
	{
		m_FaceType = QUADS;

		m_VertexCount = 6 * 4;
		m_FaceNormals = new vector3[6];
		m_Vertices = new vector3[m_VertexCount];
		m_Normals = new vector3[m_VertexCount];

		m_FaceNormals[0] = vector3(0, 1, 0);
		m_Vertices[0] = vector3(p_Mins(0), p_Maxs(1), p_Mins(2));
		m_Vertices[1] = vector3(p_Mins(0), p_Maxs(1), p_Maxs(2));
		m_Vertices[2] = vector3(p_Maxs(0), p_Maxs(1), p_Maxs(2));
		m_Vertices[3] = vector3(p_Maxs(0), p_Maxs(1), p_Mins(2));

		m_FaceNormals[1] = vector3(0, -1, 0);
		m_Vertices[4] = vector3(p_Mins(0), p_Mins(1), p_Mins(2));
		m_Vertices[5] = vector3(p_Maxs(0), p_Mins(1), p_Mins(2));
		m_Vertices[6] = vector3(p_Maxs(0), p_Mins(1), p_Maxs(2));
		m_Vertices[7] = vector3(p_Mins(0), p_Mins(1), p_Maxs(2));

		m_FaceNormals[2] = vector3(0, 0, 1);
		m_Vertices[8] = vector3(p_Mins(0), p_Mins(1), p_Maxs(2));
		m_Vertices[9] = vector3(p_Maxs(0), p_Mins(1), p_Maxs(2));
		m_Vertices[10] = vector3(p_Maxs(0), p_Maxs(1), p_Maxs(2));
		m_Vertices[11] = vector3(p_Mins(0), p_Maxs(1), p_Maxs(2));

		m_FaceNormals[3] = vector3(0, 0, -1);
		m_Vertices[12] = vector3(p_Mins(0), p_Mins(1), p_Mins(2));
		m_Vertices[13] = vector3(p_Mins(0), p_Maxs(1), p_Mins(2));
		m_Vertices[14] = vector3(p_Maxs(0), p_Maxs(1), p_Mins(2));
		m_Vertices[15] = vector3(p_Maxs(0), p_Mins(1), p_Mins(2));
		
		m_FaceNormals[4] = vector3(1, 0, 0);
		m_Vertices[16] = vector3(p_Maxs(0), p_Mins(1), p_Mins(2));
		m_Vertices[17] = vector3(p_Maxs(0), p_Maxs(1), p_Mins(2));
		m_Vertices[18] = vector3(p_Maxs(0), p_Maxs(1), p_Maxs(2));
		m_Vertices[19] = vector3(p_Maxs(0), p_Mins(1), p_Maxs(2));
		
		m_FaceNormals[5] = vector3(-1, 0, 0);
		m_Vertices[20] = vector3(p_Mins(0), p_Mins(1), p_Mins(2));
		m_Vertices[21] = vector3(p_Mins(0), p_Mins(1), p_Maxs(2));
		m_Vertices[22] = vector3(p_Mins(0), p_Maxs(1), p_Maxs(2));
		m_Vertices[23] = vector3(p_Mins(0), p_Maxs(1), p_Mins(2));

		for(size_t i_Vertex = 0; i_Vertex < m_VertexCount; i_Vertex++)
			m_Normals[i_Vertex] = m_FaceNormals[i_Vertex / 4];

		if(m_TexLevelCount > 0)
		{
			m_TexCoords = new vector2*[m_TexLevelCount];

			for(size_t i_Level = 0; i_Level < m_TexLevelCount; i_Level++)
			{
				m_TexCoords[i_Level] = new vector2[m_VertexCount];

				for(size_t i_Face = 0; i_Face < 6; i_Face++)
				{
					m_TexCoords[i_Level][i_Face * 4]     = vector2(0, 0);
					m_TexCoords[i_Level][i_Face * 4 + 1] = vector2(1, 0);
					m_TexCoords[i_Level][i_Face * 4 + 2] = vector2(1, 1);
					m_TexCoords[i_Level][i_Face * 4 + 3] = vector2(0, 1);
				}
			}
		}
	}
};

