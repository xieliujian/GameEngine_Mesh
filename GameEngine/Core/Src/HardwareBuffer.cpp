
#include "HardwareBuffer.h"
#include "RenderPass.h"

NAMESPACEBEGIN(GameEngine)

//----------------------
// Global Param
//----------------------
HardwareBufferManager* HardwareBufferManager::s_Instance = NULL;

//----------------------
// HardwareBuffer
//----------------------
HardwareBuffer::HardwareBuffer(Usage usage) 
{
	m_eUsage = usage;
}

HardwareBuffer::~HardwareBuffer()
{
}

void* HardwareBuffer::Lock(size_t offset, size_t length, LockOptions options)
{
	void* ret = LockImpl(offset, length, options);
	return ret;
}

void* HardwareBuffer::Lock(LockOptions options)
{
	return this->Lock(0, m_nSizeInBytes, options);
}

void HardwareBuffer::Unlock(void)
{
	UnlockImpl();
}

void* HardwareBuffer::LockImpl(size_t offset, size_t length, LockOptions options)
{
	return NULL;
}

void HardwareBuffer::UnlockImpl(void)
{
}

//-------------------------
// HardwareVertexBuffer
//-------------------------
HardwareVertexBuffer::HardwareVertexBuffer(size_t vertexSize, size_t numVertices, HardwareBuffer::Usage usage) 
	: HardwareBuffer(usage)
{
	m_nNumVertices = numVertices;
	m_nVertexSize = vertexSize;
	m_nSizeInBytes = vertexSize * numVertices;
}

HardwareVertexBuffer::~HardwareVertexBuffer()
{
}

size_t HardwareVertexBuffer::GetVertexSize(void) const 
{
	return m_nVertexSize;
}

size_t HardwareVertexBuffer::GetNumVertices(void) const 
{ 
	return m_nNumVertices;
}

//--------------------------
// HardwareIndexBuffer
//--------------------------
HardwareIndexBuffer::HardwareIndexBuffer(IndexType idxType, size_t numIndexes, HardwareBuffer::Usage usage) 
	: HardwareBuffer(usage)
{
	m_IndexType = idxType;
	m_nNumIndexes = numIndexes;
		
	// Calculate the size of the indexes
	switch (m_IndexType)
	{
	case IT_16BIT:
		m_nIndexSize = sizeof(unsigned short);
		break;
	case IT_32BIT:
		m_nIndexSize = sizeof(unsigned int);
		break;
	}
		
	m_nSizeInBytes = m_nIndexSize * m_nNumIndexes;
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
}

//--------------------------
// HardwareBufferManager
//--------------------------
HardwareBufferManager::HardwareBufferManager()
{
	s_Instance = this;
	m_vecVertexBuf.clear();
	m_vecIndexBuf.clear();
	m_vecVertexDecl.clear();
	m_vecVertexBufBinding.clear();
}

HardwareBufferManager::~HardwareBufferManager()
{

}

HardwareBufferManager& HardwareBufferManager::Instance()
{
	return *s_Instance;
}

VertexBufferBinding* HardwareBufferManager::CreateVertexBufferBinding()
{
	VertexBufferBinding *pBinding = new VertexBufferBinding;
	if (pBinding != NULL)
	{
		m_vecVertexBufBinding.push_back(pBinding);
	}

	return pBinding;
}

//--------------------------
// CGpuProgram
//--------------------------
CGpuProgram::CGpuProgram(String szGroup, String szName)
	:Resource(szGroup, szName)
{

}

CGpuProgram::~CGpuProgram()
{

}

//--------------------------
// CHighLevelGpuProgram
//--------------------------
CHighLevelGpuProgram::CHighLevelGpuProgram(String szGroup, String szName)
	:CGpuProgram(szGroup, szName)
{

}

CHighLevelGpuProgram::~CHighLevelGpuProgram()
{

}

NAMESPACEEND