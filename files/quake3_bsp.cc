// Quake 3 BSP routines by Morten Hustveit
//
// Based on a description by Kekoa Proudfoot <kekoa@grahpics.stanford.edu>

#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

#include <bezier.h>
#include <profiler.h>
#include <raytracer.h>
#include <color.h>
#include <files/quake3_bsp.h>
#include <files/swap.h>

	template<class type>
	static void CopyLump(FILE* p_File, const header p_Header, int32_t pi_Lump,
			type*& r_Dest, size_t& r_Count)
	{
		r_Count = p_Header.m_Lumps[pi_Lump].m_FileLength / sizeof(type);

		if(p_Header.m_Lumps[pi_Lump].m_FileLength % sizeof(type))
			throw std::runtime_error("Invalid BSP file: Bad lump size");

		r_Dest = new type[r_Count];
		fseek(p_File, p_Header.m_Lumps[pi_Lump].m_FileOffset, SEEK_SET);
		fread(r_Dest, sizeof(type), r_Count, p_File);

		for(size_t i = 0; i < r_Count; i++)
			byte_swap(r_Dest[i]);
	}

	template<>
	void CopyLump <visibility> (FILE* p_File, const header p_Header,
			int32_t pi_Lump, visibility*& r_Dest, size_t& r_Count)
	{
		r_Count = p_Header.m_Lumps[pi_Lump].m_FileLength - 2 * sizeof(int32_t);

		fseek(p_File, p_Header.m_Lumps[pi_Lump].m_FileOffset, SEEK_SET);
		r_Dest = new visibility();
		fread(&r_Dest->m_VectorCount, 1, sizeof(int32_t), p_File);
		fread(&r_Dest->m_VectorSize, 1, sizeof(int32_t), p_File);
		byte_swap(r_Dest->m_VectorCount);
		byte_swap(r_Dest->m_VectorSize);
		r_Dest->m_Data = new uint8_t[r_Dest->m_VectorCount * r_Dest->m_VectorSize];
		fread(r_Dest->m_Data, r_Dest->m_VectorSize, r_Dest->m_VectorCount, p_File);
	}

	template<>
	void byte_swap <header> (header& p_Val)
	{
		byte_swap(p_Val.m_Version);

		for(size_t i = 0; i < c_LumpCount; i++)
		{
			byte_swap(p_Val.m_Lumps[i].m_FileOffset);
			byte_swap(p_Val.m_Lumps[i].m_FileLength);
		}
	}

	template<>
	void byte_swap <vector3> (vector3& p_Val)
	{
		byte_swap(p_Val(0));
		byte_swap(p_Val(1));
		byte_swap(p_Val(2));
	}

	template<>
	void byte_swap <vector2> (vector2& p_Val)
	{
		byte_swap(p_Val(0));
		byte_swap(p_Val(1));
	}

	template<>
	void byte_swap <texture> (texture& p_Val)
	{
		byte_swap(p_Val.m_Flags);
		byte_swap(p_Val.m_Content);
	}

	template<>
	void byte_swap <plane> (plane& p_Val)
	{
		byte_swap(p_Val.m_Normal);
		byte_swap(p_Val.m_Distance);
	}

	template<>
	void byte_swap <node> (node& p_Val)
	{
		byte_swap(p_Val.mi_Plane);
		array_swap(p_Val.m_Children);
		array_swap(p_Val.m_Mins);
		array_swap(p_Val.m_Maxs);
	}

	template<>
	void byte_swap <leaf> (leaf& p_Val)
	{
		byte_swap(p_Val.m_Cluster);
		byte_swap(p_Val.m_Area);
		array_swap(p_Val.m_Mins);
		array_swap(p_Val.m_Maxs);
		byte_swap(p_Val.m_LeafFace);
		byte_swap(p_Val.m_LeafFaceCount);
		byte_swap(p_Val.m_LeafBrush);
		byte_swap(p_Val.m_LeafBrushCount);
	}

	template<>
	void byte_swap <model> (model& p_Val)
	{
		array_swap(p_Val.m_Mins);
		array_swap(p_Val.m_Maxs);
		byte_swap(p_Val.m_Face);
		byte_swap(p_Val.m_FaceCount);
		byte_swap(p_Val.m_Brush);
		byte_swap(p_Val.m_BrushCount);
	}

	template<>
	void byte_swap <brush> (brush& p_Val)
	{
		byte_swap(p_Val.mi_BrushSide);
		byte_swap(p_Val.m_BrushSideCount);
		byte_swap(p_Val.mi_Texture);
	}

	template<>
	void byte_swap <brush_side> (brush_side& p_Val)
	{
		byte_swap(p_Val.mi_Plane);
		byte_swap(p_Val.mi_Texture);
	}

	template<>
	void byte_swap <vertex> (vertex& p_Val)
	{
		byte_swap(p_Val.m_Position);
		byte_swap(p_Val.m_TextureCoord);
		byte_swap(p_Val.m_LightmapCoord);
		byte_swap(p_Val.m_Normal);
	}

	template<>
	void byte_swap <face> (face& p_Val)
	{
		byte_swap(p_Val.mi_Texture);
		byte_swap(p_Val.mi_Effect);
		byte_swap(p_Val.m_Type);
		byte_swap(p_Val.mi_Vertex);
		byte_swap(p_Val.m_VertexCount);
		byte_swap(p_Val.mi_MeshVertex);
		byte_swap(p_Val.m_MeshVertexCount);
		byte_swap(p_Val.mi_Lightmap);
		array_swap(p_Val.m_LightmapStart);
		array_swap(p_Val.m_LightmapSize);
		byte_swap(p_Val.m_Origin);
		array_swap(p_Val.m_LightmapUnits);
		byte_swap(p_Val.m_Normal);
		array_swap(p_Val.m_PatchDimensions);
	}

	template<>
	void byte_swap <lightmap> (lightmap& p_Val)
	{
	}

	template<>
	void byte_swap <light_volume> (light_volume& p_Val)
	{
	}

	template<>
	void byte_swap <visibility> (visibility& p_Val)
	{
		// *** Swapped during loading
	}

	bsp::bsp(const char* p_FileName)
	{
		FILE* l_File;

		if(!(l_File = fopen(p_FileName, "rb")))
			throw std::runtime_error(cls::system_error());

		header l_Header;

		fread(&l_Header, sizeof(header), 1, l_File);

		byte_swap(l_Header);

		CopyLump(l_File, l_Header, c_LumpEntities, m_Entities, m_EntitySize);
		CopyLump(l_File, l_Header, c_LumpTextures, m_Textures, m_TextureCount);
		CopyLump(l_File, l_Header, c_LumpPlanes, m_Planes, m_PlaneCount);
		CopyLump(l_File, l_Header, c_LumpNodes, m_Nodes, m_NodeCount);
		CopyLump(l_File, l_Header, c_LumpLeaves, m_Leaves, m_LeafCount);
		CopyLump(l_File, l_Header, c_LumpLeafFaces, m_LeafFaces, m_LeafFaceCount);
		CopyLump(l_File, l_Header, c_LumpLeafBrushes, m_LeafBrushes, m_LeafBrushCount);
		CopyLump(l_File, l_Header, c_LumpModels, m_Models, m_ModelCount);
		CopyLump(l_File, l_Header, c_LumpBrushes, m_Brushes, m_BrushCount);
		CopyLump(l_File, l_Header, c_LumpBrushSides, m_BrushSides, m_BrushSideCount);
		CopyLump(l_File, l_Header, c_LumpVertices, m_Vertices, m_VertexCount);
		CopyLump(l_File, l_Header, c_LumpMeshVertices, m_MeshVertices, m_MeshVertexCount);
		CopyLump(l_File, l_Header, c_LumpEffects, m_Effects, m_EffectCount);
		CopyLump(l_File, l_Header, c_LumpFaces, m_Faces, m_FaceCount);
		CopyLump(l_File, l_Header, c_LumpLightmaps, m_Lightmaps, m_LightmapCount);
		CopyLump(l_File, l_Header, c_LumpLightVolumes, m_LightVolumes, m_LightVolumeCount);
		CopyLump(l_File, l_Header, c_LumpVisData, m_VisData, m_VisDataSize);

		fclose(l_File);

		m_Marks = new bool[m_FaceCount];

		// *** Load textures

		m_TextureHandles = new cls::image[m_TextureCount];

		for(size_t i_Texture = 0; i_Texture < m_TextureCount; i_Texture++)
		{
			cls::image& l_TextureHandle = m_TextureHandles[i_Texture];

			try
			{
				l_TextureHandle.load((std::string("data/") + std::string(m_Textures[i_Texture].m_Name)).c_str());
			}
			catch(std::runtime_error p_Error)
			{
				std::cerr << "Failed to load `" << m_Textures[i_Texture].m_Name << "': " << p_Error.what() << std::endl;
			}
		}

		// *** Load lightmaps

		m_LightmapHandles = new cls::image[m_LightmapCount];

		for(size_t i_Lightmap = 0; i_Lightmap < m_LightmapCount; i_Lightmap++)
		{
			cls::image& l_LightmapHandle = m_LightmapHandles[i_Lightmap];

			for(size_t i = 0; i < 128 * 128 * 3; i += 3)
			{
				// *** Overbrighten the colors

				float r = m_Lightmaps[i_Lightmap].m_Data[i    ] * 4.0f / 255.0f;
				float g = m_Lightmaps[i_Lightmap].m_Data[i + 1] * 4.0f / 255.0f;
				float b = m_Lightmaps[i_Lightmap].m_Data[i + 2] * 4.0f / 255.0f;
				float scale = 1.0f, tmp_scale;

				if((r >= 1.0f) && ((tmp_scale = (1.0f / r)) < scale))
					scale = tmp_scale;
				if((g >= 1.0f) && ((tmp_scale = (1.0f / g)) < scale))
					scale = tmp_scale;
				if((b >= 1.0f) && ((tmp_scale = (1.0f / b)) < scale))
					scale = tmp_scale;

				scale *= 255.0f;

				r *= scale;
				g *= scale;
				b *= scale;

				m_Lightmaps[i_Lightmap].m_Data[i    ] = uint8_t(r + 0.5f);
				m_Lightmaps[i_Lightmap].m_Data[i + 1] = uint8_t(g + 0.5f);
				m_Lightmaps[i_Lightmap].m_Data[i + 2] = uint8_t(b + 0.5f);
			}

			l_LightmapHandle.set_pixel_format(cls::image::RGB);
			l_LightmapHandle.set_geometry(128, 128);
			l_LightmapHandle.set_data((uint8_t*) m_Lightmaps[i_Lightmap].m_Data);
		}

		size_t l_PatchVertexCount = 0;

		for(size_t i_Face = 0; i_Face < m_FaceCount; i_Face++)
		{
			face& l_Face = m_Faces[i_Face];

			if(l_Face.m_Type != face::Patch)
				continue;

			l_Face.mi_MeshVertex = m_VertexCount + l_PatchVertexCount;
			l_Face.m_MeshVertexCount = (l_Face.m_PatchDimensions[0] * 4 - 1) * (l_Face.m_PatchDimensions[1] * 4 - 1);

			l_PatchVertexCount += l_Face.m_MeshVertexCount;
		}

		// *** Reallocate vertex buffer to fit Bezier patches

		vertex* l_NewVertices = new vertex[m_VertexCount + l_PatchVertexCount];

		memcpy(l_NewVertices, m_Vertices, m_VertexCount * sizeof(vertex));

		delete [] m_Vertices;

		m_VertexCount += l_PatchVertexCount;
		m_Vertices = l_NewVertices;

		// *** Load the Bezier patch faces

		for(size_t i_Face = 0; i_Face < m_FaceCount; i_Face++)
		{
			face& l_Face = m_Faces[i_Face];

			if(l_Face.m_Type != face::Patch)
				continue;

			cls::bezier<vertex> l_Patch(
				l_Face.m_PatchDimensions[0],
				l_Face.m_PatchDimensions[1],
				&m_Vertices[l_Face.mi_Vertex]);

			(l_Face.m_PatchDimensions[0] *= 4) -= 1;
			(l_Face.m_PatchDimensions[1] *= 4) -= 1;

			l_Patch.create_vertices(
				l_Face.m_PatchDimensions[0],
				l_Face.m_PatchDimensions[1],
				&m_Vertices[l_Face.mi_MeshVertex]);
		}
	}

	bsp::~bsp()
	{
	}

	size_t bsp::find_leaf(const vector3& p_Position) const
	{
		signed int r_Node = 0;

		while(r_Node >= 0)
		{
			plane& l_Plane = m_Planes[m_Nodes[r_Node].mi_Plane];

			r_Node = m_Nodes[r_Node].m_Children[
				  (l_Plane.m_Normal * p_Position >= l_Plane.m_Distance) ? 0 : 1];
		}

		return size_t(-(r_Node + 1));
	}


	static float s_Nearest;

	bool bsp::trace_ray(const signed int pi_Node, const ray p_Ray, color& r_Color)
	{
		if(pi_Node < 0)
		{
			leaf& l_Leaf = m_Leaves[-(pi_Node + 1)];

			if((m_CurrentCluster != -1)
			&&(	  (l_Leaf.m_Cluster == -1)
				||(!test_visibility(m_CurrentCluster, l_Leaf.m_Cluster))))
				return false;

			if(!p_Ray.intersect(
				vector3(l_Leaf.m_Mins[0], l_Leaf.m_Mins[1], l_Leaf.m_Mins[2]),
				vector3(l_Leaf.m_Maxs[0], l_Leaf.m_Maxs[1], l_Leaf.m_Maxs[2])))
				return false;

			for(size_t i_LeafFace = 0;
			           i_LeafFace < l_Leaf.m_LeafFaceCount; i_LeafFace++)
			{
				const int& i_Face =
					m_LeafFaces[l_Leaf.m_LeafFace + i_LeafFace];

				if(m_Marks[i_Face])
					continue;

				face& l_Face = m_Faces[i_Face];

				m_Marks[i_Face] = true;

				switch(l_Face.m_Type)
				{
				case face::Polygon:
				case face::Mesh:

					for(signed int i = 0; i < l_Face.m_MeshVertexCount; i += 3)
					{
						vertex& l_Vertex0 = m_Vertices[l_Face.mi_Vertex
							+ m_MeshVertices[l_Face.mi_MeshVertex + i]];
						vertex& l_Vertex1 = m_Vertices[l_Face.mi_Vertex
							+ m_MeshVertices[l_Face.mi_MeshVertex + i + 1]];
						vertex& l_Vertex2 = m_Vertices[l_Face.mi_Vertex
							+ m_MeshVertices[l_Face.mi_MeshVertex + i + 2]];

						float l_Alpha, l_Beta;

						const float l_Distance = p_Ray.hit(
							l_Alpha, l_Beta,
							l_Vertex0.m_Position,
							l_Vertex1.m_Position,
							l_Vertex2.m_Position,
							l_Face.m_Normal);

						if(l_Distance >= s_Nearest)
							continue;

						s_Nearest = l_Distance;

						const vector2 l_TextureCoord =
							  l_Vertex0.m_TextureCoord * (1 - l_Alpha - l_Beta)
							+ l_Vertex1.m_TextureCoord * l_Alpha
							+ l_Vertex2.m_TextureCoord * l_Beta;

						const vector2 l_LightmapCoord =
							  l_Vertex0.m_LightmapCoord * (1 - l_Alpha - l_Beta)
							+ l_Vertex1.m_LightmapCoord * l_Alpha
							+ l_Vertex2.m_LightmapCoord * l_Beta;

						const image& l_Texture = m_TextureHandles[l_Face.mi_Texture];
						const image& l_Lightmap = m_LightmapHandles[l_Face.mi_Lightmap];

						const uint8_t* l_TextureData    = l_Texture.get_data();
						const size_t   l_TextureWidth   = l_Texture.get_width();
						const size_t   l_TextureHeight  = l_Texture.get_height();
						const size_t   l_TextureBPP     = l_Texture.get_bytes_per_pixel();
						const uint8_t* l_LightmapData   = l_Lightmap.get_data();
						const size_t   l_LightmapWidth  = l_Lightmap.get_width();
						const size_t   l_LightmapHeight = l_Lightmap.get_height();
						const size_t   l_LightmapBPP    = l_Lightmap.get_bytes_per_pixel();

						const size_t l_U = size_t(l_TextureCoord(0) * l_TextureWidth)
							% l_TextureWidth;
						const size_t l_V = size_t(l_TextureCoord(1) * l_TextureHeight)
							% l_TextureHeight;
						const size_t l_S = size_t(l_LightmapCoord(0) * l_LightmapWidth)
							% l_LightmapWidth;
						const size_t l_T = size_t(l_LightmapCoord(1) * l_LightmapHeight)
							% l_LightmapHeight;

						const uint8_t l_Red = l_TextureData[
							(l_V  * l_TextureWidth + l_U)  * l_TextureBPP];
						const uint8_t l_Green = l_TextureData[
							(l_V  * l_TextureWidth + l_U)  * l_TextureBPP + 1];
						const uint8_t l_Blue = l_TextureData[
							(l_V  * l_TextureWidth + l_U)  * l_TextureBPP + 2];

						const uint8_t l_RedScale = l_LightmapData[
							(l_T  * l_LightmapWidth + l_S)  * l_LightmapBPP];
						const uint8_t l_GreenScale = l_LightmapData[
							(l_T  * l_LightmapWidth + l_S)  * l_LightmapBPP + 1];
						const uint8_t l_BlueScale = l_LightmapData[
							(l_T  * l_LightmapWidth + l_S)  * l_LightmapBPP + 2];

						r_Color.m_Red = (l_Red * l_RedScale) >> 8;
						r_Color.m_Green = (l_Green * l_GreenScale) >> 8;
						r_Color.m_Blue = (l_Blue * l_BlueScale) >> 8;

						return true;
					}

					break;

				case face::Billboard:

					break;

				case face::Patch:

					// XXX: fixme

					break;
				}
			}
		}
		else
		{
			node& l_Node = m_Nodes[pi_Node];
			plane& l_Plane = m_Planes[l_Node.mi_Plane];

			if(!p_Ray.intersect(
				vector3(l_Node.m_Mins[0], l_Node.m_Mins[1], l_Node.m_Mins[2]),
				vector3(l_Node.m_Maxs[0], l_Node.m_Maxs[1], l_Node.m_Maxs[2])))
				return false;

			if(l_Plane.m_Normal * p_Ray.get_point() >= l_Plane.m_Distance)
			{
				if(trace_ray(l_Node.m_Children[1], p_Ray, r_Color))
				{}//	return true;

				if(trace_ray(l_Node.m_Children[0], p_Ray, r_Color))
				{}//	return true;
			}
			else
			{
				if(trace_ray(l_Node.m_Children[0], p_Ray, r_Color))
				{}//	return true;

				if(trace_ray(l_Node.m_Children[1], p_Ray, r_Color))
				{}//	return true;
			}
		}

		return false;
	}

	color bsp::trace_ray(const ray p_Ray)
	{
		s_Nearest = HUGE_VAL;
		m_CurrentCluster = m_Leaves[find_leaf(p_Ray.get_point())].m_Cluster;

		memset(m_Marks, false, m_FaceCount * sizeof(bool));

		color r_Color;

		trace_ray(0, p_Ray, r_Color);

		return r_Color;
	}

	void bsp::set_state(bool p_Activate)
	{
		if(p_Activate)
		{
			glColor4f(1, 1, 1, 1);

			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);

			glEnable(GL_DEPTH_TEST);

			glActiveTextureARB(GL_TEXTURE0_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_TEXTURE_2D);

			glActiveTextureARB(GL_TEXTURE1_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_2D);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(vertex), &m_Vertices->m_Position);
		}
		else
		{
			glDisable(GL_CULL_FACE);
			glFrontFace(GL_CCW);

			glDisable(GL_DEPTH_TEST);

			glActiveTextureARB(GL_TEXTURE1_ARB);
			glDisable(GL_TEXTURE_2D);

			glActiveTextureARB(GL_TEXTURE0_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glDisable(GL_TEXTURE_2D);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}

	void bsp::render(const camera p_Camera)
	{
		m_Camera = p_Camera;

		memset(m_Marks, false, m_FaceCount * sizeof(bool));

		m_CurrentCluster = m_Leaves[find_leaf(m_Camera.m_Position)].m_Cluster;

		draw_node(0);
	}

	void bsp::draw_node(const signed int i_Node) const
	{
		using namespace cls;

		if(i_Node < 0)
		{
			leaf& l_Leaf = m_Leaves[-(i_Node + 1)];

			if((m_CurrentCluster != -1)
			&&(	  (l_Leaf.m_Cluster == -1)
				||(!test_visibility(m_CurrentCluster, l_Leaf.m_Cluster))))
				return;

			for(size_t i_LeafFace = 0;
			           i_LeafFace < l_Leaf.m_LeafFaceCount; i_LeafFace++)
			{
				int& i_Face =
					m_LeafFaces[l_Leaf.m_LeafFace + i_LeafFace];

				if(m_Marks[i_Face])
					continue;

				face& l_Face = m_Faces[i_Face];

				m_Marks[i_Face] = true;

				// *** Preliminary inaccurate backface culling, so we
				//     can throw some faces away, without loading their
				//     textures and transforming their vertices.

				if(l_Face.m_Normal * m_Camera.m_Direction > M_PI_4)
					continue;

				m_TextureHandles[l_Face.mi_Texture].activate(0);
				m_LightmapHandles[l_Face.mi_Lightmap].activate(1);

				switch(l_Face.m_Type)
				{
				case face::Mesh:

					glBegin(GL_TRIANGLES);

					for(size_t i_Vertex = 0; i_Vertex < l_Face.m_VertexCount; i_Vertex++)
					{
						vertex& l_Vertex =
							m_Vertices[
								  l_Face.mi_Vertex
								+ m_MeshVertices[
									l_Face.mi_MeshVertex + i_Vertex]];

						glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, l_Vertex.m_TextureCoord.get_data());
						glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, l_Vertex.m_LightmapCoord.get_data());
						glArrayElement(l_Face.mi_Vertex +
							m_MeshVertices[l_Face.mi_MeshVertex + i_Vertex]);
					}

					glEnd();

					break;

				case face::Polygon:

					glBegin(GL_POLYGON);

					for(size_t i_Vertex = 0; i_Vertex < l_Face.m_VertexCount; i_Vertex++)
					{
						vertex& l_Vertex =
							m_Vertices[l_Face.mi_Vertex + i_Vertex];

						glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, l_Vertex.m_TextureCoord.get_data());
						glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, l_Vertex.m_LightmapCoord.get_data());
						glArrayElement(l_Face.mi_Vertex + i_Vertex);
					}

					glEnd();

					break;

				case face::Billboard:

					break;

				case face::Patch:

					cls::patch_strips l_PatchStrips(
						l_Face.m_PatchDimensions[0],
						l_Face.m_PatchDimensions[1]);

					glBegin(GL_TRIANGLE_STRIP);

					for(size_t i_Strip = 0; i_Strip < l_PatchStrips.size(); i_Strip++)
					{
						signed int i_Vertex = l_PatchStrips[i_Strip];

						if(i_Vertex < 0)
						{
							if(i_Strip)
								glEnd(), glBegin(GL_TRIANGLE_STRIP);

							vertex& l_Vertex = m_Vertices[l_Face.mi_MeshVertex - i_Vertex];

							glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, l_Vertex.m_TextureCoord.get_data());
							glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, l_Vertex.m_LightmapCoord.get_data());
							glArrayElement(l_Face.mi_MeshVertex - i_Vertex);
						}
						else
						{
							vertex& l_Vertex = m_Vertices[l_Face.mi_MeshVertex + i_Vertex];

							glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, l_Vertex.m_TextureCoord.get_data());
							glMultiTexCoord2fvARB(GL_TEXTURE1_ARB, l_Vertex.m_LightmapCoord.get_data());
							glArrayElement(l_Face.mi_MeshVertex + i_Vertex);
						}
					}

					glEnd();

					break;
				}
			}
		}
		else
		{
			node& l_Node = m_Nodes[i_Node];
			plane& l_Plane = m_Planes[l_Node.mi_Plane];

			if(!m_Camera.inside(
				vector3(l_Node.m_Mins[0], l_Node.m_Mins[1], l_Node.m_Mins[2]),
				vector3(l_Node.m_Maxs[0], l_Node.m_Maxs[1], l_Node.m_Maxs[2])))
				return;

			if(l_Plane.m_Normal * m_Camera.m_Position >= l_Plane.m_Distance)
			{
				draw_node(l_Node.m_Children[0]);
				draw_node(l_Node.m_Children[1]);
			}
			else
			{
				draw_node(l_Node.m_Children[1]);
				draw_node(l_Node.m_Children[0]);
			}
		}
	}

	vertex vertex::operator*(const double p_Value) const
	{
		vertex r_Vertex;

		r_Vertex.m_Position = m_Position * p_Value;
		r_Vertex.m_TextureCoord = m_TextureCoord * p_Value;
		r_Vertex.m_LightmapCoord = m_LightmapCoord * p_Value;
		r_Vertex.m_Normal = m_Normal * p_Value;
		r_Vertex.m_Color = m_Color;

		return r_Vertex;
	}

	vertex& vertex::operator+=(const vertex p_Vertex)
	{
		m_Position += p_Vertex.m_Position;
		m_TextureCoord += p_Vertex.m_TextureCoord;
		m_LightmapCoord += p_Vertex.m_LightmapCoord;
		m_Normal += p_Vertex.m_Normal;
		m_Color = p_Vertex.m_Color;

		return *this;
	}
