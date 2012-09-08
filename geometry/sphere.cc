#include <vector>
#include <algorithm>

#include <vector.h>
#include <geom.h>
#include <raytracer.h>
#include <color.h>

namespace cls
{
	sphere::sphere(size_t p_Quality, size_t p_TextureLevels)
	{
		std::vector<vector3> l_Vertices;
		std::vector<size_t> l_Indices;

		// *** Generate octahedron

		l_Vertices.push_back(vector3(0, 0, 1));
		l_Vertices.push_back(vector3(0, 1, 0));
		l_Vertices.push_back(vector3(-1, 0, 0));
		l_Vertices.push_back(vector3(0, -1, 0));
		l_Vertices.push_back(vector3(1, 0, 0));
		l_Vertices.push_back(vector3(0, 0, -1));
					
		l_Indices.push_back(0); l_Indices.push_back(1); l_Indices.push_back(2);
		l_Indices.push_back(0); l_Indices.push_back(2); l_Indices.push_back(3);
		l_Indices.push_back(0); l_Indices.push_back(3); l_Indices.push_back(4);
		l_Indices.push_back(0); l_Indices.push_back(4); l_Indices.push_back(1);
		l_Indices.push_back(1); l_Indices.push_back(5); l_Indices.push_back(2);
		l_Indices.push_back(2); l_Indices.push_back(5); l_Indices.push_back(3);
		l_Indices.push_back(3); l_Indices.push_back(5); l_Indices.push_back(4);
		l_Indices.push_back(4); l_Indices.push_back(5); l_Indices.push_back(1);

		// *** Divide all faces into 4 new ones p_Quality times

		while(p_Quality--)
		{
			std::vector<size_t> l_NewIndices;

			while(l_Indices.size())
			{
				size_t i_Points[3];

				i_Points[2] = l_Indices.back(); l_Indices.pop_back();
				i_Points[1] = l_Indices.back(); l_Indices.pop_back();
				i_Points[0] = l_Indices.back(); l_Indices.pop_back();

				vector3 l_MidPoints[3];
				
				l_MidPoints[0] = 
					((l_Vertices[i_Points[0]] 
					+ l_Vertices[i_Points[1]]) / 2).normalize();
				l_MidPoints[1] = 
					((l_Vertices[i_Points[0]] 
					+ l_Vertices[i_Points[2]]) / 2).normalize();
				l_MidPoints[2] = 
					((l_Vertices[i_Points[1]] 
					+ l_Vertices[i_Points[2]]) / 2).normalize();

				size_t i_MidPoints[3];

				for(size_t i_MidPoint = 0; i_MidPoint < 3; i_MidPoint++)
				{
					size_t i_Vertex;

					for(i_Vertex = 0; i_Vertex < l_Vertices.size(); i_Vertex++)
					{
						if(l_Vertices[i_Vertex] == l_MidPoints[i_MidPoint])
						{
							i_MidPoints[i_MidPoint] = i_Vertex;

							break;
						}
					}

					if(i_Vertex == l_Vertices.size())
					{
						i_MidPoints[i_MidPoint] = l_Vertices.size();
						l_Vertices.push_back(l_MidPoints[i_MidPoint]);
					}
				}

				l_NewIndices.push_back(i_Points[0]);
				l_NewIndices.push_back(i_MidPoints[0]);
				l_NewIndices.push_back(i_MidPoints[1]);

				l_NewIndices.push_back(i_MidPoints[0]);
				l_NewIndices.push_back(i_MidPoints[2]);
				l_NewIndices.push_back(i_MidPoints[1]);

				l_NewIndices.push_back(i_MidPoints[0]);
				l_NewIndices.push_back(i_Points[1]);
				l_NewIndices.push_back(i_MidPoints[2]);

				l_NewIndices.push_back(i_MidPoints[1]);
				l_NewIndices.push_back(i_MidPoints[2]);
				l_NewIndices.push_back(i_Points[2]);
			}

			l_Indices = l_NewIndices;
		}

		m_FaceType = INDEXED_TRIANGLES;
		m_VertexCount = l_Vertices.size();
		m_IndexCount = l_Indices.size();
		m_TexLevelCount = p_TextureLevels;
		m_Vertices = new vector3[m_VertexCount];
		m_Indices = new size_t[m_IndexCount];
		m_TexCoords = new vector2*[m_TexLevelCount];
		m_Normals = new vector3[m_VertexCount];
		m_FaceNormals = new vector3[m_IndexCount / 3];

		std::copy(l_Vertices.begin(), l_Vertices.end(), m_Vertices);
		std::copy(l_Vertices.begin(), l_Vertices.end(), m_Normals);
		std::copy(l_Indices.begin(), l_Indices.end(), m_Indices);

		for(size_t i_TexLevel = 0; i_TexLevel < m_TexLevelCount; i_TexLevel++)
		{
			m_TexCoords[i_TexLevel] = new vector2[m_VertexCount];

			if(i_TexLevel == 0)
			{
				for(size_t i_Vertex = 0; i_Vertex < m_VertexCount; i_Vertex++)
				{
					m_TexCoords[i_TexLevel][i_Vertex] = vector2(
						m_Vertices[i_Vertex](0) * 0.5 + 0.5, 
						m_Vertices[i_Vertex](2) * 0.5 + 0.5);
				}
			}
			else
			{
				std::copy(
					m_TexCoords[0], m_TexCoords[0] + m_VertexCount, 
					m_TexCoords[i_TexLevel]);
			}
		}

		for(size_t i_Face = 0; i_Face < m_IndexCount / 3; i_Face++)
		{
			m_FaceNormals[i_Face] = 
				(m_Vertices[m_Indices[i_Face * 3 + 1]] - m_Vertices[m_Indices[i_Face * 3]]).cross
				(m_Vertices[m_Indices[i_Face * 3 + 2]] - m_Vertices[m_Indices[i_Face * 3]]);
		}
	}

	color sphere::trace_ray(const ray p_Ray)
	{
		// XXX: implementation missing

		return color(0, 0, 0, 0);
	}
};

