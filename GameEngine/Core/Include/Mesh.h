
#ifndef __Mesh_h__
#define __Mesh_h__

#include "Prerequest.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "MyString.h"
#include "Resource.h"

NAMESPACEBEGIN(GameEngine)

class Mesh;
class RenderOperation;
class Texture;

typedef std::pair<size_t, size_t> MdxAnimRange;
template <class T> T Interpolate(const float r, const T &v1, const T &v2)
{
	return static_cast<T>(v1 * (1.0f - r) + v2 * r);
}

union MeshPointer
{
	MeshPointer(void *in);

	void		*p;
	byte		*byte;
	int			*i;
	float		*f;
	short		*int16;
	char		*c;
	uint32		*dw;
	uint16		*word;
	Vector2	*v2;
	Vector3		*v3;
	Quaternion *quat;
};

class MemoryBlock
{
public:
	MemoryBlock();
	~MemoryBlock();

	BOOL Create(BYTE* pbyBuffer, int nBufferSize);
	MeshPointer GetPointer();
	int GetBufferSize();
private:
	byte*	m_pBuffer;
	int		m_nSize;
};

class Material
{
public:
	Material(Mesh *pMdx);
	~Material();

	enum
	{
		MATERIAL_NAME_SIZE = 80,
		MATERIAL_BASE_INFO = 'mtbs',
	};

	enum
	{
		MATERIAL_ALPHA_TEST = 1 << 0,
	};

	void ReadMaterial(MeshPointer inP, int size);
	char* GetTexName();
	char* GetNormal();
	char* GetSpecular();
	int GetColorId();
	Texture* GetBaseTexture() { return m_pBaseTexture; }
	bool HasNormal() { return m_bHasNormal; }
	bool HasSpecular() { return m_bHasSpecular; }
	bool HasAlphaTest();
	void SetBaseTexture(Resource *pResource);
private:
	void ReadBaseInfo(MeshPointer inP, int iSize);

	Mesh *m_pSrcMdx;
	Texture *m_pBaseTexture;

	char *m_pszTexName;
	char *m_pszSpecularName;
	char *m_pszNormalName;
	bool m_bHasSpecular;
	bool m_bHasNormal;
	DWORD m_dwRenderFlag;
	int m_nColorId;
};

class Materials
{
public:
	Materials(Mesh *pMdx);
	~Materials();

	enum
	{
		MATERIALS_INFO = 'mtrl',
	};

	void ReadMaterials(MeshPointer inP, int iSize);
	Material* GetMaterial(int i);
private:
	void ReadMaterial(MeshPointer inP, int iSize);

	Mesh *m_pSrcMdx;
	typedef std::vector<Material*> MaterialList;
	MaterialList m_vecMaterial;
};

class BaseBone
{
public:
	BaseBone(Mesh *pMdx);
	~BaseBone();

	virtual void ReadBoneInfo(MeshPointer inP, int nSize) = 0;
	virtual void CalcBone(int nAnimID, DWORD dwTime) = 0;
	virtual void Clear() = 0;
	virtual Matrix4X4 GetMatrix() = 0;
protected:
	Mesh *m_pSrcMdx;
	int m_nParentID;
	char *m_pszName;
};

class WOWBone : public BaseBone
{
public:
	WOWBone(Mesh *pMdx);
	~WOWBone();

	enum
	{
		WOW_BONE_BASE_INFO = 'wowb',
	};

	virtual void ReadBoneInfo(MeshPointer inP, int nSize);
	virtual void CalcBone(int nAnimID, DWORD dwTime);
	virtual void Clear();
	virtual Matrix4X4 GetMatrix();
private:
	void ReadWOWBase(MeshPointer inP, int nSize);
	Vector3 GetTrans(int nAnimID, DWORD dwTime);
	Quaternion GetQuat(int nAnimID, DWORD dwTime);
	Vector3 GetScale(int nAnimID, DWORD dwTime);

	bool m_bCalc;
	bool m_bUseTrans;
	bool m_bUseScale;
	bool m_bUseQuat;
	Matrix4X4 m_matBone;

	int m_nTransTimeNum;
	int m_nScaleTimeNum;
	int m_nQuatTimeNum;
	int m_nTransRangeNum;
	int m_nScaleRangeNum;
	int m_nQuatRangeNum;
	int m_nTransNum;
	int m_nScaleNum;
	int m_nQuatNum;

	Vector3 m_vPivot;
	DWORD *m_puTransTime;
	DWORD *m_puScaleTime;
	DWORD *m_puQuatTime;
	MdxAnimRange *m_pTransRange;
	MdxAnimRange *m_pScaleRange;
	MdxAnimRange *m_pQuatRange;
	Vector3 *m_pTrans;
	Vector3 *m_pScale;
	Quaternion *m_pQuat;
};

class Skeleton
{
public:
	Skeleton(Mesh *pMdx);
	~Skeleton();

	enum
	{
		SKELETONTYPE_NONE = -1,
		SKELETONTYPE_WOW = 0,
		SKELETON_BONE_WOW = 'wowb',
	};

	void ReadSkeleton(MeshPointer inP, int nSize);
	void CalcAllBone(int nAnimID, DWORD dwTime);
	BaseBone* GetBone(int nID);
	Matrix4X4 GetMatrix(int nID);
private:
	void ReadBone(MeshPointer inP, int nSize);

	Mesh *m_pSrcMdx;
	int m_nSketType;

	typedef std::vector<BaseBone*> BoneList;
	BoneList m_vecBone;
};

class Animation
{
public:
	Animation();
	~Animation();

	enum
	{
		ANIM_NAME_SIZE = 50,
	};

	virtual void ReadAnim(MeshPointer inP, int iSize) = 0;
	char* GetName();
protected:
	char *m_pszName;
};

class WOWAnimation : public Animation
{
public:
	WOWAnimation();
	~WOWAnimation();

	virtual void ReadAnim(MeshPointer inP, int iSize);
	DWORD GetStartTime();
	DWORD GetEndTime();
private:
	DWORD m_nStartTime;
	DWORD m_nEndTime;
};

class AnimationSet
{
public:
	AnimationSet();
	~AnimationSet();

	enum
	{
		ANIMSET_ANIM = 'anim',
		ANIMSET_TYPE_NONE = 0,
		ANIMSET_TYPE_WOW = 1,
	};

	void ReadAnimSet(MeshPointer inP, int iSize);
	Animation* FindAnim(int nID);
	int GetAnimNum();
	int GetType();
private:
	void ReadAnim(MeshPointer inP, int iSize);

	int m_nAnimType;
	typedef std::vector<Animation*> AnimList;
	typedef std::map<char*, Animation*> AnimMap;
	typedef std::map<int, Animation*> AnimIDMap;
	AnimList m_vecAnim;
	AnimMap m_mapAnim;
	AnimIDMap m_mapAnimID;
};

class GeoChunk
{
public:
	GeoChunk(int id, Mesh *pMdx);
	~GeoChunk();

	enum
	{
		CHUNK_NAME_SIZE = 80,
		CHUNK_BASE_INFO = 'base',
		CHUNK_BONE_INFO = 'bone',
	};

	void ReadChunk(MeshPointer inP, int size);
	int GetVertexNum();
	int GetIndexNum();
	int GetMaterialId();
	Vector3* GetVertices();
	Vector3* GetNormals();
	Vector2* GetUVs();
	WORD* GetIndices();
	byte* GetBoneWeight();
	byte* GetBoneIndex();
	bool HasBoneInfo();
	RenderOperation* GetRenderOp() { return m_pRenderOp; }
private:
	void ReadBaseInfo(MeshPointer inP, int size);
	void ReadBoneInfo(MeshPointer inP, int size);

	RenderOperation *m_pRenderOp;

	Mesh *m_pSrcMdx;
	int m_nChunkID;
	int m_nMaterialId;

	char *m_pszName;
	int m_nVertexNum;
	int m_nIndexNum;
	Vector3 *m_pVertices;
	Vector3 *m_pNormals;
	Vector2 *m_pUVs;
	DWORD *m_pDiffuse;
	WORD *m_pIndices;
	DWORD *m_pLIndices;
	byte *m_pBoneWeight;
	byte *m_pBoneIndex;

	bool m_bUseLIndex;
	bool m_bHasBoneInfo;
};

class Geometry
{
public:
	Geometry(Mesh *pMdx);
	~Geometry();

	enum
	{
		GEOMETRY_CHUNK = 'chks',
	};

	void ReadGeometry(MeshPointer inP, int size);
	GeoChunk* GetChunk(int id);
	int GetChunkNum();
private:
	void ReadChunk(MeshPointer inP, int size);

	Mesh *m_pSrcMdx;
	typedef std::vector<GeoChunk*> ChunkList;
	ChunkList m_vecChunk;
};

class WOWColorGroup
{
public:
	WOWColorGroup(Mesh *pMdx);
	~WOWColorGroup();

	enum
	{
		WOWCOLOR_COLOR = 'colr',
	};

	struct WOWColor
	{
		WOWColor();
		~WOWColor();

		int nOpacityTimeNum;
		int nOpacityRangeNum;
		int nOpacityNum;
		DWORD *puOpacityTime;
		MdxAnimRange *pOpacityRange;
		float *pOpacity;
	};

	void ReadWOWColor(MeshPointer inP, int size);
	float GetColor(int nID, int nAnimId);
private:
	Mesh *m_pSrcMdx;

	typedef std::vector<WOWColor*> ColorList;
	ColorList m_vecColor;
};

struct MeshFormat
{
	Vector3 vPos;
	Vector3 vNormal;
	Vector2 vTex;
};

class Mesh : public Resource
{
public:
	Mesh(String szGroup, String szName);
	~Mesh();

	enum
	{
		GEOMETRY = 'geom',
		MATERIALS = 'mtrl',
		SKELETON = 'sket',
		ANIMSET = 'anim',
		WOWCOLOR = 'wowc',
	};

	virtual bool LoadImpl();

	void UpdateSkeleton(int nAnimID, bool bSingleFrame, int nFrame);
	int GetFrame() { return m_nFrame; }
	Geometry* GetGeometry() { return m_pGeometry; }
	Materials* GetMaterials() { return m_pMaterials; }
	Skeleton* GetSkeleton() { return m_pSkeleton; }
	AnimationSet* GetAnimSet() { return m_pAnimSet; }
	WOWColorGroup* GetWOWColorGroup() { return m_pWOWColorGroup; }
	DWORD GetCurAnimStartTime(int nAnimId);
	DWORD GetCurAnimEndTime(int nAnimId);
private:
	bool LoadFromMdxFile(BYTE* pbyBuffer, int nBufferSize);
	bool LoadFromSketFile(byte *pbyBuffer, int nBufSize);
	void ReadGeometry(MeshPointer inP, int size);
	void ReadMaterials(MeshPointer inP, int iSize);
	void ReadWOWColor(MeshPointer inP, int iSize);
	void ReadSkeleton(MeshPointer inP, int iSize);
	void ReadAnimSet(MeshPointer inP, int iSize);

	int m_nFrame;
	MemoryBlock m_GeometryMB;
	Geometry *m_pGeometry;
	MemoryBlock m_MaterialsMB;
	Materials *m_pMaterials;
	MemoryBlock m_SkeletonMB;
	Skeleton *m_pSkeleton;
	MemoryBlock m_AnimSetMB;
	AnimationSet *m_pAnimSet;
	MemoryBlock m_WOWColorMB;
	WOWColorGroup *m_pWOWColorGroup;
};

NAMESPACEEND

#endif