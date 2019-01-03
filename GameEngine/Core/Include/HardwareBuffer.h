

#ifndef __HardwareBuffer_h__
#define __HardwareBuffer_h__

#include "Prerequest.h"
#include "Resource.h"
#include "MathTemp.h"
#include "MyString.h"

NAMESPACEBEGIN(GameEngine)

class VertexDeclaration;
class VertexBufferBinding;
class Texture;

class HardwareBuffer
{
public:
	enum Usage
	{
		HBU_STATIC = 1,
		HBU_DYNAMIC = 2,
		HBU_WRITE_ONLY = 4,
		HBU_DISCARDABLE = 8,
		HBU_STATIC_WRITE_ONLY = 5,
		HBU_DYNAMIC_WRITE_ONLY = 6,
		HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14
	};

	enum LockOptions
	{
		HBL_NORMAL,
		HBL_DISCARD,
		HBL_READ_ONLY,
		HBL_NO_OVERWRITE
	};

	HardwareBuffer(Usage usage);
	virtual ~HardwareBuffer();

	void* Lock(size_t offset, size_t length, LockOptions options);
	void* Lock(LockOptions options);
	void Unlock(void);
protected:
	virtual void* LockImpl(size_t offset, size_t length, LockOptions options);
	virtual void UnlockImpl(void);

	int m_nSizeInBytes;
	Usage m_eUsage;
	bool m_bSystemMemory;
};

class HardwareVertexBuffer : public HardwareBuffer
{
public:
	HardwareVertexBuffer(size_t vertexSize, size_t numVertices, HardwareBuffer::Usage usage);
	virtual ~HardwareVertexBuffer();

	size_t GetVertexSize(void) const;
	size_t GetNumVertices(void) const;
protected:
	size_t m_nNumVertices;
	size_t m_nVertexSize;
};

class HardwareIndexBuffer : public HardwareBuffer
{
public:
	enum IndexType
	{
		IT_16BIT,
		IT_32BIT
	};

public:
	HardwareIndexBuffer(IndexType idxType, size_t numIndexes, HardwareBuffer::Usage usage);
	~HardwareIndexBuffer();

	IndexType GetType(void) const { return m_IndexType; }
	size_t GetNumIndexes(void) const { return m_nNumIndexes; }
	size_t GetIndexSize(void) const { return m_nIndexSize; }
protected:
	IndexType m_IndexType;
	size_t m_nNumIndexes;
	size_t m_nIndexSize;
};

class HardwareBufferManager
{
public:
	HardwareBufferManager();
	~HardwareBufferManager();

	static HardwareBufferManager& Instance();
	virtual HardwareVertexBuffer* CreateVertexBuffer(int nVertSize, int nVertNum, HardwareBuffer::Usage nUsage) = 0;
	virtual HardwareIndexBuffer* CreateIndexBuffer(int nIndexNum, HardwareIndexBuffer::IndexType nType, HardwareBuffer::Usage nUsage) = 0;
	virtual VertexDeclaration* CreateVertexDeclaration() = 0;
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}
	VertexBufferBinding* CreateVertexBufferBinding();
protected:
	static HardwareBufferManager *s_Instance;
	typedef std::vector<HardwareVertexBuffer*> VertexBufferList;
	typedef std::vector<HardwareIndexBuffer*> IndexBufferList;
	typedef std::vector<VertexDeclaration*> VertexDeclarationList;
	typedef std::vector<VertexBufferBinding*> VertexBufferBindingList;
	VertexBufferList m_vecVertexBuf;
	IndexBufferList m_vecIndexBuf;
	VertexDeclarationList m_vecVertexDecl;
	VertexBufferBindingList m_vecVertexBufBinding;
};

class CGpuProgram : public Resource
{
public:
	CGpuProgram(String szGroup, String szName);
	virtual ~CGpuProgram();
};

class CHighLevelGpuProgram : public CGpuProgram
{
public:
	CHighLevelGpuProgram(String szGroup, String szName);
	virtual ~CHighLevelGpuProgram();

	virtual void BeginRender() {}
	virtual void EndRender() {}
	virtual void SetMatrix(std::string segment, Matrix4X4 *pMatrix) {}
	virtual void SetTexture(std::string segment, Texture *pTexture) {}
	virtual void Commit() {}
};

NAMESPACEEND

#endif