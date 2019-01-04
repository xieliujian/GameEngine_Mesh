

#include "Resource.h"

NAMESPACEBEGIN(GameEngine)

// -------------------------
// GlobalParam
// -------------------------
ResourceGroupManager* ResourceGroupManager::s_Instance = NULL;
	
// ------------------------------
// Resource
// ------------------------------
Resource::Resource(String szGroup, String szName)
{
	m_szGroup = szGroup;
	m_szName = szName;
	m_nSize = 0;
	m_nRefer = 0;
	m_pDataStream = NULL;
}

Resource::~Resource()
{

}

String& Resource::GetName()
{
	return m_szName;
}

String& Resource::GetGroupName()
{
	return m_szGroup;
}

int Resource::GetSize()
{
	return m_nSize;
}

bool Resource::Load()
{
	PreLoadImpl();
	LoadImpl();
	PostLoadImpl();
		
	m_nSize = CalcSize();
	return true;
}

void Resource::UnLoad()
{

}

bool Resource::LoadImpl()
{
	return true;
}

void Resource::PreLoadImpl()
{
	m_pDataStream = ResourceGroupManager::Instance().OpenResource(m_szGroup, m_szName);
}

void Resource::PostLoadImpl()
{
	SAFE_DELETE(m_pDataStream);
}

int Resource::CalcSize()
{
	return 0;
}

void Resource::AddRefer()
{
	m_nRefer++;
}

void Resource::DelRefer()
{
	m_nRefer--;
}

// -------------------------
// ResourceManager
// -------------------------
ResourceManager::ResourceManager()
{
	m_mapResource.clear();
}

ResourceManager::~ResourceManager()
{

}

Resource* ResourceManager::GetResource(String szName)
{
	ResourceMap::iterator iter = m_mapResource.find(szName);
	if (iter != m_mapResource.end())
	{
		return iter->second;
	}

	return NULL;
}

Resource* ResourceManager::Create(String szName, String szGroup)
{
	Resource *pResource = GetResource(szName);
	if (pResource != NULL)
	{
		pResource->AddRefer();
		return pResource;
	}

	pResource = CreateImpl(szGroup, szName);
	pResource->AddRefer();
	m_mapResource[szName] = pResource;
	return pResource;
}

// -------------------------
// ResourceGroupManager
// -------------------------
ResourceGroupManager::CResourceGroup::CResourceGroup()
{

}

ResourceGroupManager::CResourceGroup::~CResourceGroup()
{

}

ResourceGroupManager::ResourceGroupManager()
{
	s_Instance = this;
	m_mapResourceGroup.clear();
	m_vecArchive.clear();
}

ResourceGroupManager::~ResourceGroupManager()
{

}

ResourceGroupManager& ResourceGroupManager::Instance()
{
	return *s_Instance;
}

ResourceGroupManager::CResourceGroup* ResourceGroupManager::GetResourceGroup(String szGroupName)
{
	ResourceGroupMap::iterator iter = m_mapResourceGroup.find(szGroupName);
	if (iter != m_mapResourceGroup.end())
	{
		return iter->second;
	}

	return NULL;
}

ResourceGroupManager::CResourceGroup* ResourceGroupManager::CreateResourceGroup(String szGroupName)
{
	CResourceGroup *pGroup = new CResourceGroup;
	if (pGroup != NULL)
	{
		pGroup->m_szName = szGroupName;
		m_mapResourceGroup[szGroupName] = pGroup;
	}

	return pGroup;
}

DataStream* ResourceGroupManager::OpenResource(String szGroupName, String szResName)
{
	CResourceGroup *pGroup = GetResourceGroup(szGroupName);
	if (pGroup == NULL)
	{
		return NULL;
	}

	ResourceLocationIndex::iterator iter = pGroup->m_mapResIndexCaseSensitive.find(szResName);
	if (iter == pGroup->m_mapResIndexCaseSensitive.end())
	{
		return NULL;
	}

	Archive *pArchive = iter->second;
	if (pArchive == NULL)
	{
		return NULL;
	}

	return pArchive->Open((char*)szResName.c_str());
}

void ResourceGroupManager::AddResourceLocation(String szArcType, String szArcName, String szGroupName)
{
	CResourceGroup *pGroup = GetResourceGroup(szGroupName);
	if (pGroup == NULL)
	{
		pGroup = CreateResourceGroup(szGroupName);
	}

	if (pGroup == NULL)
	{
		return;
	}

	Archive *pArchive = ArchiveManager::Instance().Load(szArcName, szArcType);
	if (pArchive == NULL)
	{
		return;
	}

	m_vecArchive.push_back(pArchive);

	int nNum = pArchive->GetFileNum();
	for (int i = 0; i < nNum; i++)
	{
		String szName = pArchive->GetFileName(i);
		pGroup->m_mapResIndexCaseSensitive[szName] = pArchive;
	}
}

StringVector& ResourceGroupManager::ListResourceNames(String ext)
{
	m_vecFindFile.clear();

	ResourceGroupMap::iterator iter = m_mapResourceGroup.begin();
	for (; iter != m_mapResourceGroup.end(); iter++)
	{
		CResourceGroup *pGroup = iter->second;
		if (pGroup == NULL)
		{
			continue;
		}

		ResourceLocationIndex::iterator iterIndex = pGroup->m_mapResIndexCaseSensitive.begin();
		for (; iterIndex != pGroup->m_mapResIndexCaseSensitive.end(); iterIndex++)
		{
			String szName = iterIndex->first;
			bool bFind = StringUtil::Find(szName, ext);
			if (bFind)
			{
				m_vecFindFile.push_back(szName);
			}	
		}
	}

	return m_vecFindFile;
}

NAMESPACEEND