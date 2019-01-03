

#ifndef __ResourceManager_h__
#define __ResourceManager_h__

#include "Prerequest.h"
#include "Texture.h"
#include "Archive.h"
#include "Resource.h"
#include "ResourceManager.h"

NAMESPACEBEGIN(GameEngine)

class ResourceGroupManager;
class HighLevelGpuProgram;

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	virtual ~TextureManager();

	static TextureManager& Instance();
	virtual Resource* Load(String szGroup, String szName) { return NULL; }
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}
private:
	virtual Resource* CreateImpl(String szGroup, String szName) { return NULL; }
	static TextureManager *s_Instance;
};

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	~MeshManager();

	static MeshManager& Instance();

	virtual Resource* Load(String szGroup, String szName);
private:
	virtual Resource* CreateImpl(String szGroup, String szName);

	static MeshManager *s_Instance;
};

class GpuProgramManager : public ResourceManager
{
public:
	GpuProgramManager();
	virtual ~GpuProgramManager();

	static GpuProgramManager& Instance();
	virtual Resource* Load(String szGroup, String szName) { return NULL; }
	virtual HighLevelGpuProgram* GetHLSLProgram(std::string szName) { return NULL; }
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;
protected:
	virtual Resource* CreateImpl(String szGroup, String szName) { return NULL; }
	static GpuProgramManager *s_Instance;
};

NAMESPACEEND

#endif