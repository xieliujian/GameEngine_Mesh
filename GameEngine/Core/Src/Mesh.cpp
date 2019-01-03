

#include "Mesh.h"
#include "ResourceManager.h"
#include "RenderPass.h"
#include "HardwareBuffer.h"

NAMESPACEBEGIN(GameEngine)

//-----------------------------------
//-----------CMSMdxPointer-------------
//-----------------------------------
MeshPointer::MeshPointer(void* in) 
{
	p = in;
}

//-----------------------------------
//-----------MemoryBlock------------
//-----------------------------------
MemoryBlock::MemoryBlock()
{
	m_pBuffer = NULL;
	m_nSize = 0;
}

MemoryBlock::~MemoryBlock()
{
	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_nSize = 0;
}

BOOL MemoryBlock::Create( BYTE* pbyBuffer, int nBufferSize )
{
	if( pbyBuffer == NULL || nBufferSize == 0 )
	{
		assert( false );
		return FALSE;
	}

	m_nSize = nBufferSize;
	m_pBuffer = new BYTE[m_nSize];
	memcpy( m_pBuffer, pbyBuffer, m_nSize );
		
	return TRUE;
}

MeshPointer MemoryBlock::GetPointer()
{
	MeshPointer p( m_pBuffer );
	return p;
}

int MemoryBlock::GetBufferSize()
{ 
	return m_nSize;
}

//--------------------------------
//---------Material-----------
//--------------------------------
Material::Material(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
	m_bHasSpecular = false;
	m_bHasNormal = false;
	m_pszTexName = NULL;
	m_pszSpecularName = NULL;
	m_pszNormalName = NULL;
	m_dwRenderFlag = 0;
	m_nColorId = -1;
	m_pBaseTexture = NULL;
}

Material::~Material()
{

}

void Material::ReadBaseInfo(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	m_pszTexName = p.c;
	p.c += MATERIAL_NAME_SIZE;
		
	m_bHasSpecular = *p.i++;
	if (m_bHasSpecular)
	{
		m_pszSpecularName = p.c;
		p.c += MATERIAL_NAME_SIZE;
	}

	m_bHasNormal = *p.i++;
	if (m_bHasNormal)
	{
		m_pszNormalName = p.c;
		p.c += MATERIAL_NAME_SIZE;
	}

	m_dwRenderFlag = *p.dw++;
	m_nColorId = *p.i++;
}

char* Material::GetTexName()
{
	return m_pszTexName;
}

char* Material::GetNormal()
{
	return m_pszNormalName;
}

char* Material::GetSpecular()
{
	return m_pszSpecularName;
}

int Material::GetColorId()
{
	return m_nColorId;
}

bool Material::HasAlphaTest()
{
	return (m_dwRenderFlag & MATERIAL_ALPHA_TEST);
}

void Material::SetBaseTexture(Resource *pResource)
{
	m_pBaseTexture = (Texture*)pResource;
}

void Material::ReadMaterial(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	while(p.c < inP.c + iSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case MATERIAL_BASE_INFO:
			{
				p.dw++;
				int size = *p.i++;
				ReadBaseInfo(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

//--------------------------------
//---------Materials----------
//--------------------------------
Materials::Materials(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
}

Materials::~Materials()
{

}

Material* Materials::GetMaterial(int i)
{
	int size = m_vecMaterial.size();
	if (i < 0 || i > size - 1)
	{
		return NULL;
	}

	return m_vecMaterial[i];
}

void Materials::ReadMaterials(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	while (p.c < inP.c + iSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case MATERIALS_INFO:
			{
				p.dw++;
				int size = *p.i++;
				ReadMaterial(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

void Materials::ReadMaterial(MeshPointer inP, int iSize)
{
	Material *pMaterial = new Material(m_pSrcMdx);
	if (pMaterial)
	{
		pMaterial->ReadMaterial(inP, iSize);
		m_vecMaterial.push_back(pMaterial);
	}
}

//--------------------------------
//------------CMdxBone------------
//--------------------------------
BaseBone::BaseBone(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
	m_nParentID = -1;
	m_pszName = NULL;
}

BaseBone::~BaseBone()
{
}

//--------------------------------
//-------------WOWBone-----------
//--------------------------------
WOWBone::WOWBone(Mesh *pMdx)
	:BaseBone(pMdx)
{
	m_bCalc = false;
	m_bUseTrans = false;
	m_bUseScale = false;
	m_bUseQuat = false;
		
	m_nTransTimeNum = 0;
	m_nScaleTimeNum = 0;
	m_nQuatTimeNum = 0;
	m_nTransRangeNum = 0;
	m_nScaleRangeNum = 0;
	m_nQuatRangeNum = 0;
	m_nTransNum = 0;
	m_nScaleNum = 0;
	m_nQuatNum = 0;

	m_puTransTime = NULL;
	m_puScaleTime = NULL;
	m_puQuatTime = NULL;
	m_pTransRange = NULL;
	m_pScaleRange = NULL;
	m_pQuatRange = NULL;
	m_pTrans = NULL;
	m_pScale = NULL;
	m_pQuat = NULL;
}

WOWBone::~WOWBone()
{
}

void WOWBone::ReadBoneInfo(MeshPointer inP, int nSize)
{
	MeshPointer p(inP.p);

	while (p.c < inP.c + nSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case WOW_BONE_BASE_INFO:
			{
				p.dw++;
				int size = *p.i++;
				ReadWOWBase(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

void WOWBone::ReadWOWBase(MeshPointer inP, int nSize)
{
	MeshPointer p(inP.p);
		
	m_nParentID = *p.i++;
	m_bUseTrans = *p.i++;
	m_bUseScale = *p.i++;
	m_bUseQuat = *p.i++;
	m_nTransTimeNum = *p.i++;
	m_nScaleTimeNum = *p.i++;
	m_nQuatTimeNum = *p.i++;
	m_nTransRangeNum = *p.i++;
	m_nScaleRangeNum = *p.i++;
	m_nQuatRangeNum = *p.i++;
	m_nTransNum = *p.i++;
	m_nScaleNum = *p.i++;
	m_nQuatNum = *p.i++;

	m_vPivot = *p.v3++;
	if (m_nTransTimeNum > 0)
	{
		m_puTransTime = p.dw;
		p.dw += m_nTransTimeNum;
	}

	if (m_nScaleTimeNum > 0)
	{
		m_puScaleTime = p.dw;
		p.dw += m_nScaleTimeNum;
	}

	if (m_nQuatTimeNum > 0)
	{
		m_puQuatTime = p.dw;
		p.dw += m_nQuatTimeNum;
	}

	if (m_nTransRangeNum > 0)
	{
		m_pTransRange = (MdxAnimRange*)p.c;
		p.c += m_nTransRangeNum * sizeof(MdxAnimRange);
	}

	if (m_nScaleRangeNum > 0)
	{
		m_pScaleRange = (MdxAnimRange*)p.c;
		p.c += m_nScaleRangeNum * sizeof(MdxAnimRange);
	}

	if (m_nQuatRangeNum > 0)
	{
		m_pQuatRange = (MdxAnimRange*)p.c;
		p.c += m_nQuatRangeNum * sizeof(MdxAnimRange);
	}

	if (m_nTransNum > 0)
	{
		m_pTrans = p.v3;
		p.v3 += m_nTransNum;
	}

	if (m_nScaleNum > 0)
	{
		m_pScale = p.v3;
		p.v3 += m_nScaleNum;
	}

	if (m_nQuatNum > 0)
	{
		m_pQuat = p.quat;
		p.quat += m_nQuatNum;
	}
}

void WOWBone::Clear()
{
	m_bCalc = false;
}

Matrix4X4 WOWBone::GetMatrix()
{
	return m_matBone;
}

Vector3 WOWBone::GetTrans(int nAnimID, DWORD dwTime)
{
	if (m_nTransNum == 0)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	if (m_nTransNum == 1)
	{
		return m_pTrans[0];
	}

	if (m_nTransRangeNum == 0)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	MdxAnimRange range = m_pTransRange[nAnimID];
	if (range.first != range.second)
	{
		size_t t1, t2;
		size_t pos = 0;
		for (size_t i = range.first; i < range.second; i++) 
		{
			if (dwTime >= m_puTransTime[i] && dwTime < m_puTransTime[i + 1]) 
			{
				pos = i;
				break;
			}
		}

		t1 = m_puTransTime[pos];
		t2 = m_puTransTime[pos + 1];
		float r = (dwTime - t1) / (float)(t2 - t1);
		Vector3 v1 = m_pTrans[pos];
		Vector3 v2 = m_pTrans[pos + 1];
		return Interpolate(r, v1, v2);
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

Quaternion WOWBone::GetQuat(int nAnimID, DWORD dwTime)
{
	if (m_nQuatNum == 0)
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (m_nQuatNum == 1)
	{
		return m_pQuat[0];
	}

	if (m_nQuatRangeNum == 0)
	{
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	}

	MdxAnimRange range = m_pQuatRange[nAnimID];
	if (range.first != range.second)
	{
		size_t t1, t2;
		size_t pos = 0;
		for (size_t i = range.first; i < range.second; i++) 
		{
			if (dwTime >= m_puQuatTime[i] && dwTime < m_puQuatTime[i + 1]) 
			{
				pos = i;
				break;
			}
		}

		t1 = m_puQuatTime[pos];
		t2 = m_puQuatTime[pos + 1];

		float r = (dwTime - t1) / (float)(t2 - t1);
		Quaternion v1 = m_pQuat[pos];
		Quaternion v2 = m_pQuat[pos + 1];
		return Interpolate(r, v1, v2);
	}

	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Vector3 WOWBone::GetScale(int nAnimID, DWORD dwTime)
{
	if (m_nScaleNum == 0)
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}

	if (m_nScaleNum == 1)
	{
		return m_pScale[0];
	}

	if (m_nScaleRangeNum == 0)
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}

	MdxAnimRange range = m_pScaleRange[nAnimID];
	if (range.first != range.second)
	{
		size_t t1, t2;
		size_t pos = 0;
		for (size_t i = range.first; i < range.second; i++) 
		{
			if (dwTime >= m_puScaleTime[i] && dwTime < m_puScaleTime[i + 1]) 
			{
				pos = i;
				break;
			}
		}

		t1 = m_puScaleTime[pos];
		t2 = m_puScaleTime[pos + 1];
		float r = (dwTime - t1) / (float)(t2 - t1);
		Vector3 v1 = m_pScale[pos];
		Vector3 v2 = m_pScale[pos + 1];
		return Interpolate(r, v1, v2);
	}

	return Vector3(1.0f, 1.0f, 1.0f);
}

void WOWBone::CalcBone(int nAnimID, DWORD dwTime)
{
	if (m_bCalc)
	{
		return;
	}

	Matrix4X4 m;
	bool bUse = m_bUseTrans || m_bUseScale || m_bUseQuat;
	if (bUse)
	{
		m.Translation(m_vPivot);

		if (m_bUseTrans) 
		{
			Vector3 tr = GetTrans(nAnimID, dwTime);
			m *= Matrix4X4::NewTranslation(tr);
		}

		if (m_bUseQuat) 
		{
			Quaternion q = GetQuat(nAnimID, dwTime);
			m *= Matrix4X4::NewQuatRotate(q);
		}

		if (m_bUseScale)
		{
			Vector3 sc = GetScale(nAnimID, dwTime);
			m *= Matrix4X4::NewScale(sc);
		}

		m *= Matrix4X4::NewTranslation(m_vPivot * -1.0f);
	}
	else
	{
		m.Unit();
	}

	if (m_nParentID > -1) 
	{
		BaseBone *pBone = m_pSrcMdx->GetSkeleton()->GetBone(m_nParentID);
		if (pBone)
		{
			pBone->CalcBone(nAnimID, dwTime);
			m_matBone = pBone->GetMatrix() * m;
		}
		else
		{
			m_matBone = m;
		}
	} 
	else
	{
		m_matBone = m;
	}

	m_bCalc = true;
}

//--------------------------------
//----------Skeleton----------
//--------------------------------
Skeleton::Skeleton(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
	m_nSketType = SKELETONTYPE_NONE;
}

Skeleton::~Skeleton()
{

}

void Skeleton::ReadSkeleton(MeshPointer inP, int nSize)
{
	MeshPointer p(inP.p);

	m_nSketType = *p.i++;

	while (p.c < inP.c + nSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case SKELETON_BONE_WOW:
			{
				p.dw++;
				int size = *p.i++;
				ReadBone(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

void Skeleton::ReadBone(MeshPointer inP, int nSize)
{
	BaseBone *pBone = NULL;
	switch(m_nSketType)
	{
	case SKELETONTYPE_WOW:
		{
			pBone = new WOWBone(m_pSrcMdx);
		}
		break;
	default:
		break;
	}
		
	if (pBone)
	{
		pBone->ReadBoneInfo(inP.p, nSize);
		m_vecBone.push_back(pBone);
	}
}

BaseBone* Skeleton::GetBone(int nID)
{
	int nNum = m_vecBone.size();
	if (nID < 0 || nID > nNum - 1)
	{
		return NULL;
	}

	return m_vecBone[nID];
}

Matrix4X4 Skeleton::GetMatrix(int nID)
{
	Matrix4X4 mat;
	mat.Unit();

	BaseBone *pBone = GetBone(nID);
	if (pBone)
	{
		mat = pBone->GetMatrix();
	}

	return mat;
}

void Skeleton::CalcAllBone(int nAnimID, DWORD dwTime)
{
	int nNum = m_vecBone.size();
	for (int i = 0; i < nNum; i++)
	{
		BaseBone *pBone = m_vecBone[i];
		if (pBone)
		{
			pBone->Clear();
		}
	}

	for (int i = 0; i < nNum; i++)
	{
		BaseBone *pBone = m_vecBone[i];
		if (pBone)
		{
			pBone->CalcBone(nAnimID, dwTime);
		}
	}
}

//--------------------------------
//---------GeoChunk-----------
//--------------------------------
GeoChunk::GeoChunk(int id, Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
	m_nChunkID = id;
	m_nMaterialId = -1;
		
	m_pszName = NULL;
	m_nVertexNum = 0;
	m_nIndexNum = 0;
	m_pVertices = NULL;
	m_pNormals = NULL;
	m_pUVs = NULL;
	m_pDiffuse = NULL;
	m_pIndices = NULL;
	m_pLIndices = NULL;
	m_pBoneWeight = NULL;
	m_pBoneIndex = NULL;

	m_bUseLIndex = false;
	m_bHasBoneInfo = false;
	m_pRenderOp = new RenderOperation;
}

GeoChunk::~GeoChunk()
{

}

int GeoChunk::GetVertexNum()
{
	return m_nVertexNum;
}

int GeoChunk::GetIndexNum()
{
	return m_nIndexNum;
}

int GeoChunk::GetMaterialId()
{
	return m_nMaterialId;
}

Vector3* GeoChunk::GetVertices()
{
	return m_pVertices;
}

Vector3* GeoChunk::GetNormals()
{
	return m_pNormals;
}

Vector2* GeoChunk::GetUVs()
{
	return m_pUVs;
}

WORD* GeoChunk::GetIndices()
{
	return m_pIndices;
}

byte* GeoChunk::GetBoneWeight()
{
	return m_pBoneWeight;
}

byte* GeoChunk::GetBoneIndex()
{
	return m_pBoneIndex;
}

bool GeoChunk::HasBoneInfo()
{
	return m_bHasBoneInfo;
}

void GeoChunk::ReadChunk(MeshPointer inP, int size)
{
	MeshPointer p(inP.p);

	while (p.c < inP.c + size)
	{
		switch(MDX_TAG(*p.dw))
		{
		case CHUNK_BASE_INFO:
			{
				p.dw++;
				int size = *p.i++;
				ReadBaseInfo(p.p, size);
				p.c += size;
			}
			break;
		case CHUNK_BONE_INFO:
			{
				p.dw++;
				int size = *p.i++;
				ReadBoneInfo(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

void GeoChunk::ReadBoneInfo(MeshPointer inP, int size)
{
	MeshPointer p(inP.p);

	m_pBoneWeight = p.byte;
	p.byte += m_nVertexNum * 4;

	m_pBoneIndex = p.byte;
	p.byte += m_nVertexNum * 4;

	m_bHasBoneInfo = true;
}

void GeoChunk::ReadBaseInfo(MeshPointer inP, int size)
{
	MeshPointer p(inP.p);

	m_pszName = p.c;
	p.c += CHUNK_NAME_SIZE;

	m_nVertexNum = *p.i++;
	m_nIndexNum = *p.i++;
	m_nMaterialId = *p.i++;
	m_bUseLIndex = *p.i++;
		
	m_pVertices = p.v3;
	p.v3 += m_nVertexNum;
		
	m_pNormals = p.v3;
	p.v3 += m_nVertexNum;
		
	m_pUVs = p.v2;
	p.v2 += m_nVertexNum;

	if (m_bUseLIndex)
	{
		m_pLIndices = p.dw;
		p.dw += m_nIndexNum;
	}
	else
	{
		m_pIndices = p.word;
		p.word += m_nIndexNum;

	}
}

//--------------------------------
//---------Geometry-----------
//--------------------------------
Geometry::Geometry(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
}

Geometry::~Geometry()
{

}

void Geometry::ReadGeometry(MeshPointer inP, int size)
{
	MeshPointer p(inP.p);

	while(p.c < inP.c + size)
	{
		switch(MDX_TAG(*p.dw))
		{
		case GEOMETRY_CHUNK:
			{
				p.dw++;
				int size = *p.i++;
				ReadChunk(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

GeoChunk* Geometry::GetChunk(int id)
{
	int nSize = m_vecChunk.size();
	if (id < 0 || id > nSize - 1)
	{
		return NULL;
	}

	return m_vecChunk[id];
}

int Geometry::GetChunkNum()
{
	int nSize = m_vecChunk.size();
	return nSize;
}

void Geometry::ReadChunk(MeshPointer inP, int size)
{
	int nID = m_vecChunk.size();
	GeoChunk *pChunk = new GeoChunk(nID, m_pSrcMdx);
	if (pChunk)
	{
		pChunk->ReadChunk(inP, size);
		m_vecChunk.push_back(pChunk);
	}
}

//--------------------------------
//-----------Animation------------
//--------------------------------
Animation::Animation()
{
	m_pszName = NULL;
}

Animation::~Animation()
{

}

char* Animation::GetName()
{
	return m_pszName;
}

//--------------------------------
//------------WOWAnimation------------
//--------------------------------
WOWAnimation::WOWAnimation()
{

}

WOWAnimation::~WOWAnimation()
{

}

void WOWAnimation::ReadAnim(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);
	m_pszName = p.c;
	p.c += ANIM_NAME_SIZE;

	m_nStartTime = *p.dw++;
	m_nEndTime = *p.dw++;
}

DWORD WOWAnimation::GetStartTime()
{
	return m_nStartTime;
}

DWORD WOWAnimation::GetEndTime()
{
	return m_nEndTime;
}

//--------------------------------
//----------AnimationSet-----------
//--------------------------------
AnimationSet::AnimationSet()
{
	m_nAnimType = ANIMSET_TYPE_NONE;
}

AnimationSet::~AnimationSet()
{

}

void AnimationSet::ReadAnimSet(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	m_nAnimType = *p.i++;

	while (p.c < inP.c + iSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case ANIMSET_ANIM:
			{
				p.dw++;
				int size = *p.i++;
				ReadAnim(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

void AnimationSet::ReadAnim(MeshPointer inP, int iSize)
{
	Animation *pAnim = new WOWAnimation;
	if (pAnim)
	{
		pAnim->ReadAnim(inP.p, iSize);
		m_vecAnim.push_back(pAnim);
		m_mapAnim[pAnim->GetName()] = pAnim;
		m_mapAnimID[m_vecAnim.size() - 1] = pAnim;
	}
}

Animation* AnimationSet::FindAnim(int nID)
{
	AnimIDMap::iterator iter = m_mapAnimID.find(nID);
	if (iter != m_mapAnimID.end())
	{
		return iter->second;
	}

	return NULL;
}

int AnimationSet::GetAnimNum()
{
	return m_vecAnim.size();
}

int AnimationSet::GetType()
{
	return m_nAnimType;
}

//--------------------------------
//--------WOWColorGroup----------
//--------------------------------
WOWColorGroup::WOWColorGroup(Mesh *pMdx)
{
	m_pSrcMdx = pMdx;
}

WOWColorGroup::~WOWColorGroup()
{

}

WOWColorGroup::WOWColor::WOWColor()
{
	nOpacityTimeNum = 0;
	nOpacityRangeNum = 0;
	nOpacityNum = 0;
	puOpacityTime = NULL;
	pOpacityRange = NULL;
	pOpacity = NULL;
}

WOWColorGroup::WOWColor::~WOWColor()
{

}

void WOWColorGroup::ReadWOWColor(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	while (p.c < inP.c + iSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case WOWCOLOR_COLOR:
			{
				p.dw++;
				int size = *p.i++;

				MeshPointer s(p.p);
				{
					WOWColor *pColor = new WOWColor;
					pColor->nOpacityTimeNum = *s.i++;
					pColor->nOpacityRangeNum = *s.i++;
					pColor->nOpacityNum = *s.i++;
						
					if (pColor->nOpacityTimeNum > 0)
					{
						pColor->puOpacityTime = s.dw;
						s.dw += pColor->nOpacityTimeNum;
					}
						
					if(pColor->nOpacityRangeNum)
					{
						pColor->pOpacityRange = (MdxAnimRange*)s.c;
						s.c += pColor->nOpacityRangeNum * sizeof(MdxAnimRange);
					}
						
					if (pColor->nOpacityNum)
					{
						pColor->pOpacity = s.f;
						s.f += pColor->nOpacityNum;
					}
						
					m_vecColor.push_back(pColor);
				}

				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}
}

float WOWColorGroup::GetColor(int nID, int nAnimId)
{
	if (nID < 0 || nID > m_vecColor.size() - 1)
	{
		return 0.0f;
	}

	WOWColor *pColor = m_vecColor[nID];
	if (pColor == NULL)
	{
		return 0.0f;
	}

	DWORD dwTime = m_pSrcMdx->GetFrame();

	if (pColor->nOpacityNum == 0)
	{
		return 0.0f;
	}

	if (pColor->nOpacityNum == 1)
	{
		return pColor->pOpacity[0];
	}

	if (pColor->nOpacityRangeNum == 0)
	{
		return 0.0f;
	}

	MdxAnimRange range = pColor->pOpacityRange[nAnimId];
	if (range.first != range.second)
	{
		size_t t1, t2;
		size_t pos = 0;
		for (size_t i = range.first; i < range.second; i++) 
		{
			if (dwTime >= pColor->puOpacityTime[i] && dwTime < pColor->puOpacityTime[i + 1]) 
			{
				pos = i;
				break;
			}
		}

		t1 = pColor->puOpacityTime[pos];
		t2 = pColor->puOpacityTime[pos + 1];
		float r = (dwTime - t1) / (float)(t2 - t1);
		float v1 = pColor->pOpacity[pos];
		float v2 = pColor->pOpacity[pos + 1];
			
		return Interpolate(r, v1, v2);
	}

	return 0.0f;
}

//--------------------------------
//--------------Mesh--------------
//--------------------------------
Mesh::Mesh(String szGroup, String szName)
	:Resource(szGroup, szName)
{
	m_szName = szName;
	m_pGeometry = NULL;
	m_pMaterials = NULL;
	m_pSkeleton = NULL;
	m_pAnimSet = NULL;
	m_pWOWColorGroup = NULL;
	m_nFrame = 0;
}

Mesh::~Mesh()
{

}

bool Mesh::LoadImpl()
{
	// load mdx mesh ...
	if (m_pDataStream == NULL) return false;
	int nSize = m_pDataStream->GetSize();
	byte *pBuf = m_pDataStream->GetBuffer();
	LoadFromMdxFile(pBuf, nSize);

	// load mdx mesh skeleton ...
	String szSket = StringUtil::Combine(m_szName, ".skeleton", true);
	DataStream *pSketStream = ResourceGroupManager::Instance().OpenResource(m_szGroup, szSket);
	if (pSketStream != NULL)
	{
		nSize = pSketStream->GetSize();
		pBuf = pSketStream->GetBuffer();
		LoadFromSketFile(pBuf, nSize);
		SAFE_DELETE(pSketStream);
	}

	// load texture ...
	int nChunkNum = m_pGeometry->GetChunkNum();
	for (int nChunk = 0; nChunk < nChunkNum; nChunk++)
	{
		GeoChunk *pChunk = m_pGeometry->GetChunk(nChunk);
		int nMatId = pChunk->GetMaterialId();
		Material *pMaterial = m_pMaterials->GetMaterial(nMatId);
		String szMat = StringUtil::Combine(m_szName, pMaterial->GetTexName(), false);
		pMaterial->SetBaseTexture(TextureManager::Instance().Load(m_szGroup, szMat));

		if (pMaterial->HasSpecular())
		{
			szMat = StringUtil::Combine(m_szName, pMaterial->GetSpecular(), false);
			TextureManager::Instance().Load(m_szGroup, szMat);
		}
	}

	// vertex buffer and index buffer
	for (int nChunk = 0; nChunk < nChunkNum; nChunk++)
	{
		GeoChunk *pChunk = m_pGeometry->GetChunk(nChunk);
		if (pChunk == NULL)
		{
			continue;
		}

		Vector3 *pVertices = pChunk->GetVertices();
		Vector3 *pNormals = pChunk->GetNormals();
		Vector2 *pUVs = pChunk->GetUVs();
		WORD *pIndices = pChunk->GetIndices();
		int nVertexNum = pChunk->GetVertexNum();
		int nIndexNum = pChunk->GetIndexNum();

		HardwareVertexBuffer *pVertBuf = HardwareBufferManager::Instance().CreateVertexBuffer(
					32, nVertexNum, HardwareBuffer::HBU_WRITE_ONLY);
		if (pVertBuf != NULL)
		{		
			MeshFormat *pBuf = (MeshFormat*)pVertBuf->Lock(HardwareBuffer::HBL_DISCARD);
			if (pBuf != NULL)
			{
				for (int nVertex = 0; nVertex < nVertexNum; nVertex++, pBuf++)
				{
					pBuf->vPos = pVertices[nVertex];
					pBuf->vNormal = pNormals[nVertex];
					pBuf->vTex = pUVs[nVertex];
				}
			}
			pVertBuf->Unlock();
		}

		HardwareIndexBuffer *pIndexBuf = HardwareBufferManager::Instance().CreateIndexBuffer(
			nIndexNum, HardwareIndexBuffer::IT_16BIT, HardwareBuffer::HBU_WRITE_ONLY);
		if (pIndexBuf != NULL)
		{
			void *pBuf = pIndexBuf->Lock(HardwareBuffer::HBL_DISCARD);
			if (pBuf != NULL)
				memcpy(pBuf, pIndices, nIndexNum * sizeof(WORD));
			pIndexBuf->Unlock();
		}

		// add render op ...
		RenderOperation *pRenderOp = pChunk->GetRenderOp();
		if (pRenderOp != NULL)
		{
			VertexData *pVertexData = new VertexData;
			IndexData *pIndexData = new IndexData;
			VertexBufferBinding *pBing = pVertexData->GetVertexBufferBinding();
			VertexDeclaration *pDecl = pVertexData->GetVertexDeclaration();	
			pVertexData->SetVertexRange(0, pChunk->GetVertexNum());
			pIndexData->SetIndexRange(0, pChunk->GetIndexNum());
			pIndexData->SetIndexBuffer(pIndexBuf);
			pBing->AddBinding(0, pVertBuf);
			pDecl->AddElement(0, 0, VET_SHORT3, VES_POSITION, 0);
			pDecl->AddElement(0, 12, VET_FLOAT3, VES_NORMAL, 0);
			pDecl->AddElement(0, 24, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

			pRenderOp->m_bUseIndex = true;
			pRenderOp->m_pVertexData = pVertexData;
			pRenderOp->m_pIndexData = pIndexData;
			pRenderOp->m_OpType = RenderOperation::OT_TRIANGLE_LIST;
		}
	}

	return true;
}

bool Mesh::LoadFromMdxFile(BYTE* pbyBuffer, int nBufferSize)
{
	MeshPointer p(pbyBuffer);
	while (p.byte < pbyBuffer + nBufferSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case GEOMETRY:
			{
				p.dw++;
				int size = *p.i++;
				ReadGeometry(p.p, size);
				p.c += size;
			}
			break;
		case MATERIALS:
			{
				p.dw++;
				int size = *p.i++;
				ReadMaterials(p.p, size);
				p.c += size;
			}
			break;
		case WOWCOLOR:
			{
				p.dw++;
				int size = *p.i++;
				ReadWOWColor(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}

	return true;
}

bool Mesh::LoadFromSketFile(byte *pbyBuffer, int nBufSize)
{
	MeshPointer p(pbyBuffer);

	while(p.byte < pbyBuffer + nBufSize)
	{
		switch(MDX_TAG(*p.dw))
		{
		case SKELETON:
			{
				p.dw++;
				int size = *p.i++;
				ReadSkeleton(p.p, size);
				p.c += size;
			}
			break;
		case ANIMSET:
			{
				p.dw++;
				int size = *p.i++;
				ReadAnimSet(p.p, size);
				p.c += size;
			}
			break;
		default:
			{
				p.dw++;
				int size = *p.i++;
				p.c += size;
			}
			break;
		}
	}

	return true;
}

void Mesh::UpdateSkeleton(int nAnimID, bool bSingleFrame, int nFrame)
{
	int t = 0;
	if (m_pAnimSet)
	{
		Animation *pAnim = m_pAnimSet->FindAnim(nAnimID);
		if (pAnim)
		{
			if (m_pAnimSet->GetType() == AnimationSet::ANIMSET_TYPE_WOW)
			{
				WOWAnimation *pWOWAnim = (WOWAnimation*)pAnim;

				DWORD dwStartTime = pWOWAnim->GetStartTime();
				DWORD dwEndTime = pWOWAnim->GetEndTime();
					
				if (bSingleFrame)
				{
					t = nFrame;
					if (t < dwStartTime || t >= dwEndTime)
					{
						t = dwStartTime;
					}
				}
				else
				{
					static DWORD dwLastTime = HQ_TimeGetTime();
					DWORD dwCurTime = HQ_TimeGetTime();
					t = dwStartTime + (dwCurTime - dwLastTime);
					while (t >= dwEndTime)
					{
						t -= (dwEndTime - dwStartTime);
						dwLastTime = HQ_TimeGetTime();
					}
				}
			}
		}
	}

	m_nFrame = t;

	if (m_pSkeleton)
	{
		m_pSkeleton->CalcAllBone(nAnimID, m_nFrame);
	}	
}

DWORD Mesh::GetCurAnimStartTime(int nAnimId)
{
	if (m_pAnimSet)
	{
		Animation *pAnim = m_pAnimSet->FindAnim(nAnimId);
		if (pAnim)
		{
			if (m_pAnimSet->GetType() == AnimationSet::ANIMSET_TYPE_WOW)
			{
				WOWAnimation *pWOWAnim = (WOWAnimation*)pAnim;

				DWORD dwStartTime = pWOWAnim->GetStartTime();
				return dwStartTime;
			}
		}
	}

	return 0;
}

DWORD Mesh::GetCurAnimEndTime(int nAnimId)
{
	if (m_pAnimSet)
	{
		Animation *pAnim = m_pAnimSet->FindAnim(nAnimId);
		if (pAnim)
		{
			if (m_pAnimSet->GetType() == AnimationSet::ANIMSET_TYPE_WOW)
			{
				WOWAnimation *pWOWAnim = (WOWAnimation*)pAnim;

				DWORD dwEndTime = pWOWAnim->GetEndTime();
				return dwEndTime;
			}
		}
	}

	return 0;
}

void Mesh::ReadGeometry(MeshPointer inP, int size)
{
	MeshPointer p(inP.p);

	m_GeometryMB.Create(p.byte, size);
	m_pGeometry = new Geometry(this);
	m_pGeometry->ReadGeometry(m_GeometryMB.GetPointer(), m_GeometryMB.GetBufferSize());
}

void Mesh::ReadMaterials(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	m_MaterialsMB.Create(p.byte, iSize);
	m_pMaterials = new Materials(this);
	m_pMaterials->ReadMaterials(m_MaterialsMB.GetPointer(), m_MaterialsMB.GetBufferSize());
}

void Mesh::ReadWOWColor(MeshPointer inP, int iSize)
{
	MeshPointer p(inP.p);

	m_WOWColorMB.Create(p.byte, iSize);
	m_pWOWColorGroup = new WOWColorGroup(this);
	m_pWOWColorGroup->ReadWOWColor(m_WOWColorMB.GetPointer(), m_WOWColorMB.GetBufferSize());
}

void Mesh::ReadSkeleton(MeshPointer inP, int iSize)
{
	m_SkeletonMB.Create(inP.byte, iSize);
	m_pSkeleton = new Skeleton(this);
	m_pSkeleton->ReadSkeleton(m_SkeletonMB.GetPointer(), m_SkeletonMB.GetBufferSize());
}

void Mesh::ReadAnimSet(MeshPointer inP, int iSize)
{
	m_AnimSetMB.Create(inP.byte, iSize);
	m_pAnimSet = new AnimationSet;
	m_pAnimSet->ReadAnimSet(m_AnimSetMB.GetPointer(), m_AnimSetMB.GetBufferSize());
}

NAMESPACEEND