#ifndef _QUAKE3_BSP_H_
#define _QUAKE3_BSP_H_

#include <iostream>
#include <stdio.h>
#include <geom.h>
#include <image.h>
#include <color.h>

#pragma pack(1)

	using cls::vector3;
	using cls::vector2;
	using cls::color;
	using cls::ray;
	using cls::image;
	using cls::camera;

	const size_t c_LumpEntities     = 0;
	const size_t c_LumpTextures     = 1;
	const size_t c_LumpPlanes       = 2;
	const size_t c_LumpNodes        = 3;
	const size_t c_LumpLeaves       = 4;
	const size_t c_LumpLeafFaces    = 5;
	const size_t c_LumpLeafBrushes  = 6;
	const size_t c_LumpModels       = 7;
	const size_t c_LumpBrushes      = 8;
	const size_t c_LumpBrushSides   = 9;
	const size_t c_LumpVertices     = 10;
	const size_t c_LumpMeshVertices = 11;
	const size_t c_LumpEffects      = 12;
	const size_t c_LumpFaces        = 13;
	const size_t c_LumpLightmaps    = 14;
	const size_t c_LumpLightVolumes = 15;
	const size_t c_LumpVisData      = 16;
	const size_t c_LumpCount        = 17;

	class header
	{
	public:
		char    m_Magic[4];
		int32_t m_Version;
		struct
		{
			int32_t m_FileOffset;
			int32_t m_FileLength;
		} m_Lumps[c_LumpCount];
	};

	typedef char entity;

	class texture
	{
	public:
		char    m_Name[64];
		int32_t m_Flags;
		int32_t m_Content;
	};

	class plane
	{
	public:
		vector3 m_Normal;
		float   m_Distance;
	};

	class node
	{
	public:
		int32_t mi_Plane;
		int32_t m_Children[2];
		int32_t m_Mins[3];
		int32_t m_Maxs[3];
	};

	class leaf
	{
	public:
		int32_t  m_Cluster;
		int32_t  m_Area;
		int32_t  m_Mins[3];
		int32_t  m_Maxs[3];
		int32_t  m_LeafFace;
		uint32_t m_LeafFaceCount;
		int32_t  m_LeafBrush;
		int32_t  m_LeafBrushCount;
	};

	typedef int32_t leaf_face;
	typedef int32_t leaf_brush;

	class model
	{
	public:
		float   m_Mins[3];
		float   m_Maxs[3];
		int32_t m_Face;
		int32_t m_FaceCount;
		int32_t m_Brush;
		int32_t m_BrushCount;
	};

	class brush
	{
	public:
		int32_t mi_BrushSide;
		int32_t m_BrushSideCount;
		int32_t mi_Texture;
	};

	class brush_side
	{
	public:
		int32_t mi_Plane;
		int32_t mi_Texture;
	};

	class vertex
	{
	public:
		vector3 m_Position;
		vector2 m_TextureCoord;
		vector2 m_LightmapCoord;
		vector3 m_Normal;
		color   m_Color;

		vertex() { }

		// *** Functions needed for creating Bezier patches

		vertex operator*(const double p_Value) const;
		vertex& operator+=(const vertex p_Vertex);
	};

	typedef uint32_t mesh_vertex;

	class effect
	{
	public:
		char    m_Name[64];
		int32_t mi_Brush;
		int32_t m_Unknown;
	};

	class face
	{
	public:
		int32_t  mi_Texture;
		int32_t  mi_Effect;

		enum type {
			Polygon = 1, Patch = 2, Mesh = 3, Billboard = 4
		};

		int32_t  m_Type;

		int32_t  mi_Vertex;
		uint32_t m_VertexCount;
		int32_t  mi_MeshVertex;
		int32_t  m_MeshVertexCount;
		int32_t  mi_Lightmap;
		int32_t  m_LightmapStart[2];
		int32_t  m_LightmapSize[2];
		vector3  m_Origin;
		vector3  m_LightmapUnits[2];
		vector3  m_Normal;
		uint32_t m_PatchDimensions[2];
	};

	class lightmap
	{
	public:
		uint8_t m_Data[128 * 128 * 3];
	};

	class light_volume
	{
	public:
		uint8_t m_Ambient[3];
		uint8_t m_Directional[3];
		uint8_t m_Direction[2];
	};

	class visibility
	{
	public:
		uint32_t  m_VectorCount;
		uint32_t  m_VectorSize;
		uint8_t*  m_Data;
	};

	class bsp : public cls::geom
	{
	private:
		// *** Data loaded directly from file

		entity*       m_Entities;
		size_t        m_EntitySize;
		texture*      m_Textures;
		size_t        m_TextureCount;
		plane*        m_Planes;
		size_t        m_PlaneCount;
		node*         m_Nodes;
		size_t        m_NodeCount;
		leaf*         m_Leaves;
		size_t        m_LeafCount;
		leaf_face*    m_LeafFaces;
		size_t        m_LeafFaceCount;
		leaf_brush*   m_LeafBrushes;
		size_t        m_LeafBrushCount;
		model*        m_Models;
		size_t        m_ModelCount;
		brush*        m_Brushes;
		size_t        m_BrushCount;
		brush_side*   m_BrushSides;
		size_t        m_BrushSideCount;
		vertex*       m_Vertices;
		size_t        m_VertexCount;
		mesh_vertex*  m_MeshVertices;
		size_t        m_MeshVertexCount;
		effect*       m_Effects;
		size_t        m_EffectCount;
		face*         m_Faces;
		size_t        m_FaceCount;
		lightmap*     m_Lightmaps;
		size_t        m_LightmapCount;
		light_volume* m_LightVolumes;
		size_t        m_LightVolumeCount;
		visibility*   m_VisData;
		size_t        m_VisDataSize;

		// *** Data created after loading

		image*        m_TextureHandles;
		image*        m_LightmapHandles;

		// *** Run time functions and data

		bool*         m_Marks;
		camera        m_Camera;
		int32_t       m_CurrentCluster;
		void          draw_node(const signed int p_Index) const;
		size_t        find_leaf(const vector3& p_Position) const;
		bool          trace_ray(const signed int p_NodeIndex, const ray p_Ray, color& r_Color);

	public:
		bsp(const char* p_FileName);
		~bsp();

		color    trace_ray(const ray p_Ray);
		void     set_state(bool p_Activate = true);
		void     render(const camera p_Camera);

		inline char test_visibility(const size_t& p_From, const size_t& p_To) const
		{
			return m_VisData->m_Data[p_From * m_VisData->m_VectorSize + (p_To / 8)] & (1 << (p_To % 8));
		}
	};

#endif

