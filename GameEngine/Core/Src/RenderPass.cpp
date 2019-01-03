

#include "RenderPass.h"
#include "Exception.h"
#include "HardwareBuffer.h"

NAMESPACEBEGIN(GameEngine)

// -------------------------
// VertexElement
// -------------------------
VertexElement::VertexElement(int source, int offset, VertexElementType theType,
	VertexElementSemantic semantic, unsigned short index)
{
	m_nSource = source;
	m_nOffset = offset;
	m_nType = theType;
	m_nSemantic = semantic;
	m_nIndex = index;
}

VertexElement::~VertexElement()
{

}

int VertexElement::GetTypeSize(VertexElementType type)
{
	switch(type)
	{
	case VET_COLOUR:
	case VET_COLOUR_ABGR:
	case VET_COLOUR_ARGB:
		return sizeof(DWORD);
	case VET_FLOAT1:
		return sizeof(float);
	case VET_FLOAT2:
		return sizeof(float)*2;
	case VET_FLOAT3:
		return sizeof(float)*3;
	case VET_FLOAT4:
		return sizeof(float)*4;
	case VET_SHORT1:
		return sizeof(short);
	case VET_SHORT2:
		return sizeof(short)*2;
	case VET_SHORT3:
		return sizeof(short)*3;
	case VET_SHORT4:
		return sizeof(short)*4;
	case VET_UBYTE4:
		return sizeof(unsigned char)*4;
	}

	return 0;
}

int VertexElement::GetTypeCount(VertexElementType type)
{
	switch (type)
	{
	case VET_COLOUR:
	case VET_COLOUR_ABGR:
	case VET_COLOUR_ARGB:
		return 1;
	case VET_FLOAT1:
		return 1;
	case VET_FLOAT2:
		return 2;
	case VET_FLOAT3:
		return 3;
	case VET_FLOAT4:
		return 4;
	case VET_SHORT1:
		return 1;
	case VET_SHORT2:
		return 2;
	case VET_SHORT3:
		return 3;
	case VET_SHORT4:
		return 4;
	case VET_UBYTE4:
		return 4;
	}

	GAMEENGINE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid type", 
		"VertexElement::getTypeCount");
}

// -------------------------
// VertexDeclaration
// -------------------------
VertexDeclaration::VertexDeclaration()
{

}

VertexDeclaration::~VertexDeclaration()
{

}

int VertexDeclaration::GetElementCount()
{
	return m_vecElement.size();
}

VertexElement* VertexDeclaration::GetElement(int i)
{
	int nNum = m_vecElement.size();
	if (nNum == 0)
	{
		return NULL;
	}

	if (i < 0 || i > nNum - 1)
	{
		return NULL;
	}

	return m_vecElement[i];
}

void VertexDeclaration::AddElement(int source, int offset, VertexElementType theType, 
		VertexElementSemantic semantic, unsigned short index)
{
	VertexElement *pElement = new VertexElement(source, offset, theType, semantic, index);
	m_vecElement.push_back(pElement);
}

// -------------------------
// VertexBufferBinding
// -------------------------
VertexBufferBinding::VertexBufferBinding()
{

}

VertexBufferBinding::~VertexBufferBinding()
{

}

HardwareVertexBuffer* VertexBufferBinding::AddBinding(int nIndex, HardwareVertexBuffer *pBuf)
{
	VertexBufMap::iterator iter = m_mapVertexBuf.find(nIndex);
	if (iter != m_mapVertexBuf.end())
	{
		return iter->second;
	}

	m_mapVertexBuf[nIndex] = pBuf;

	iter = m_mapVertexBuf.find(nIndex);
	if (iter != m_mapVertexBuf.end())
	{
		return iter->second;
	}

	return NULL;
}

// -------------------------
// VertexData
// -------------------------
VertexData::VertexData()
{
	m_pVertexDeclaration = HardwareBufferManager::Instance().CreateVertexDeclaration();
	m_pVertexBufBinding = HardwareBufferManager::Instance().CreateVertexBufferBinding();
	m_nVertexStart = 0;
	m_nVertexCount = 0;
}

VertexData::~VertexData()
{

}

// -------------------------
// IndexData
// -------------------------
IndexData::IndexData()
{
	m_pIndexBuffer = NULL;
	m_nIndexStart = 0;
	m_nIndexCount = 0;
}

IndexData::~IndexData()
{

}

// -------------------------
// RenderOperation
// -------------------------
RenderOperation::RenderOperation()
{
	m_OpType = OT_TRIANGLE_LIST;
	m_pVertexData = NULL;
	m_pIndexData = NULL;
	m_bUseIndex = true;
}

RenderOperation::~RenderOperation()
{

}

NAMESPACEEND