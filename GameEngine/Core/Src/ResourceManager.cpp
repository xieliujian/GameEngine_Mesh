

#include "ResourceManager.h"
#include "Mesh.h"

NAMESPACEBEGIN(GameEngine)

// -------------------------
// GlobalParam
// -------------------------
MeshManager* MeshManager::s_Instance = NULL;
TextureManager* TextureManager::s_Instance = NULL;
GpuProgramManager* GpuProgramManager::s_Instance = NULL;

// -------------------------
// TextureManager
// -------------------------
TextureManager::TextureManager()
{
	s_Instance = this;
}

TextureManager::~TextureManager()
{

}

TextureManager& TextureManager::Instance()
{
	return *s_Instance;
}

// -------------------------
// MeshManager
// -------------------------
MeshManager::MeshManager()
{
	s_Instance = this;
}

MeshManager::~MeshManager()
{

}

MeshManager& MeshManager::Instance()
{
	return *s_Instance;
}

Resource* MeshManager::Load(String szGroup, String szName)
{
	Resource *pResource = Create(szGroup, szName);
	pResource->Load();
	return pResource;
}

Resource* MeshManager::CreateImpl(String szGroup, String szName)
{
	Resource *pResource = new Mesh(szGroup, szName);

	return pResource;
}

// -------------------------
// GpuProgramManager
// -------------------------
GpuProgramManager::GpuProgramManager()
	:ResourceManager()
{
	s_Instance = this;
}

GpuProgramManager::~GpuProgramManager()
{

}

GpuProgramManager& GpuProgramManager::Instance()
{
	return *s_Instance;
}

NAMESPACEEND