

#ifndef __OgreResource_h__
#define __OgreResource_h__

#include "Prerequest.h"
#include "MyString.h"
#include "Archive.h"

NAMESPACEBEGIN(GameEngine)

class ResourceManager;

class Resource
{
public:
	Resource(String szGroup, String szName);
	virtual ~Resource();

	bool Load();
	void UnLoad();

	String& GetName();
	String& GetGroupName();
	int GetSize();
	void AddRefer();
	void DelRefer();
protected:
	virtual bool LoadImpl();
	virtual void PreLoadImpl();
	virtual void PostLoadImpl();
	virtual int CalcSize();

	String m_szName;	// Unique name of the resource
	String m_szGroup;	// The name of the resource group
	int m_nSize;	// The size of the resource in bytes
	int m_nRefer;
	DataStream *m_pDataStream;
};

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	virtual Resource* Load(String szGroup, String szName) = 0;
protected:
	virtual Resource* CreateImpl(String szGroup, String szName) = 0;

	Resource* Create(String szName, String szGroup);
	Resource* GetResource(String szName);

	typedef std::map<String, Resource*> ResourceMap;
	ResourceMap m_mapResource;
};

class ResourceGroupManager
{
public:
	ResourceGroupManager();
	~ResourceGroupManager();

	static ResourceGroupManager& Instance();

	typedef std::map<String, Archive*> ResourceLocationIndex;
	class CResourceGroup
	{
	public:
		CResourceGroup();
		~CResourceGroup();

		String m_szName;
		ResourceLocationIndex m_mapResIndexCaseSensitive;
		ResourceLocationIndex m_mapResIndexCaseInsensitive;
	};

	typedef std::map<String, CResourceGroup*> ResourceGroupMap;
	typedef std::vector<Archive*> ArchiveList;

	CResourceGroup* GetResourceGroup(String szGroupName);
	CResourceGroup* CreateResourceGroup(String szGroupName);
	DataStream* OpenResource(String szGroupName, String szResName);
	StringVector& ListResourceNames(String ext);
	void AddResourceLocation(String szArcType, String szArcName, String szGroupName);
private:
	static ResourceGroupManager *s_Instance;
	ResourceGroupMap m_mapResourceGroup;
	ArchiveList m_vecArchive;
	StringVector m_vecFindFile;
};

NAMESPACEEND

#endif