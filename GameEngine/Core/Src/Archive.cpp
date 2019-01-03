

#include "Archive.h"
#include "Exception.h"

#include <sys/types.h>
#include <sys/stat.h>

NAMESPACEBEGIN(GameEngine)

// -------------------------
// Global Param
// -------------------------
CArchiveManager* CArchiveManager::s_Instance = NULL;

// -------------------------
// Archive
// -------------------------
Archive::Archive()
{

}

Archive::Archive(String type, String name)
{
	m_szType = type;
	m_szName = name;
}

Archive::~Archive()
{

}

String& Archive::GetName()
{
	return m_szName;
}

String& Archive::GetType()
{
	return m_szType;
}

void Archive::Load()
{

}

void Archive::UnLoad()
{

}

// -------------------------
// FileArchive
// -------------------------
FileArchive::FileArchive(String name)
	:Archive("File", name)
{
}

FileArchive::~FileArchive()
{

}

DataStream* FileArchive::Open(char* filename)
{
	DataStream *pDataStream = new FileHandleDataStream;
	pDataStream->Open(filename);
		
	return pDataStream;
}

int FileArchive::GetFileNum()
{
	return 0;
}

String FileArchive::GetFileName(int nId)
{
	return "";
}

// -------------------------
// ZipArchive
// -------------------------
ZipArchive::ZipArchive(String name)
{

}

ZipArchive::~ZipArchive()
{

}

void ZipArchive::Load()
{

}

void ZipArchive::UnLoad()
{

}

DataStream* ZipArchive::Open(char* filename)
{
	return NULL;
}

int ZipArchive::GetFileNum()
{
	return 0;
}

String ZipArchive::GetFileName(int nId)
{
	return "";
}

// -------------------------
// ArchiveFactory
// -------------------------
ArchiveFactory::ArchiveFactory()
{

}

ArchiveFactory::~ArchiveFactory()
{

}

String ArchiveFactory::GetType()
{
	return m_szType;
}

void ArchiveFactory::DestroyInstance(Archive *pArch)
{
	SAFE_DELETE(pArch);
}

// -------------------------
// FileArchiveFactory
// -------------------------
FileArchiveFactory::FileArchiveFactory()
{
	m_szType = "File";
}

FileArchiveFactory::~FileArchiveFactory()
{

}

Archive* FileArchiveFactory::CreateInstance(String filename)
{
	return new FileArchive(filename);
}

// -------------------------
// ZipArchiveFactory
// -------------------------
ZipArchiveFactory::ZipArchiveFactory()
{
	m_szType = "Zip";
}

ZipArchiveFactory::~ZipArchiveFactory()
{

}

Archive* ZipArchiveFactory::CreateInstance(String filename)
{
	return new ZipArchive(filename);
}

// -------------------------
// DataStream
// -------------------------
DataStream::DataStream()
{
	m_nSize = 0;
}

DataStream::~DataStream()
{

}

int DataStream::GetSize()
{
	return m_nSize;
}

String DataStream::GetLine()
{
	// keep looping while not hitting delimiter
	int nReadCount;
	char szTmpBuf[MAX_PATH];
	String szRet;
	while ( (nReadCount = Read(szTmpBuf, MAX_PATH - 1)) != 0)
	{
		// Terminate string
		szTmpBuf[nReadCount] = '\0';

		char *p = strchr(szTmpBuf, '\n');
		if (p != 0)
		{
			// reposition backwards
			Skip((long)(p + 1 - szTmpBuf - nReadCount));
			*p = '\0';
		}

		szRet = szTmpBuf;

		if (p != 0)
		{
			// trim off trailing cr if this was a cr/lf entry
			if (szRet.length() && szRet[szRet.length() - 1] == '\r')
			{
				szRet.erase(szRet.length() - 1, 1);
			}

			// found terminator, break out
			break;
		}
	}

	StringUtil::Trim(szRet);

	return szRet;
}

// -------------------------
// MemoryDataStream
// -------------------------
MemoryDataStream::MemoryDataStream()
{

}

MemoryDataStream::~MemoryDataStream()
{

}

// -------------------------
// FileStreamDataStream
// -------------------------
FileStreamDataStream::FileStreamDataStream(String name, std::ifstream *fp)
	:DataStream()
{
	m_pStream = fp;
		
	// calculate the size
	m_pStream->seekg(0, std::ios_base::end);
	m_nSize = m_pStream->tellg();
	m_pStream->seekg(0, std::ios_base::beg);
}

FileStreamDataStream::~FileStreamDataStream()
{

}

bool FileStreamDataStream::Eof()
{
	if (m_pStream != NULL)
	{
		return m_pStream->eof();
	}

	return true;
}

int FileStreamDataStream::Read(void* pBuf, int nCount)
{
	m_pStream->read(static_cast<char*>(pBuf), static_cast<std::streamsize>(nCount));
	return m_pStream->gcount();
}

void FileStreamDataStream::Skip(long nCount)
{
	if (m_pStream != NULL)
	{
		m_pStream->clear();
		m_pStream->seekg(static_cast<std::ifstream::pos_type>(nCount), std::ios::cur);
	}
}

// -------------------------
// FileHandleDataStream
// -------------------------
FileHandleDataStream::FileHandleDataStream()
{
	m_pFile = NULL;
}

FileHandleDataStream::~FileHandleDataStream()
{

}

void FileHandleDataStream::Open(String filename)
{
	fopen_s(&m_pFile, filename.c_str(), "rb");
	if (m_pFile == NULL)
	{
		GAMEENGINE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
			"Cannot open file: " + filename,
			"FileHandleDataStream::Open");
	}

	struct stat tagStat;
	int ret = stat(filename.c_str(), &tagStat);
	m_nSize = tagStat.st_size;
}

int FileHandleDataStream::Read(void* pBuf, int nCount)
{
	fread(pBuf, nCount, 1, m_pFile);
	return 0;
}

void FileHandleDataStream::Close()
{
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

// -------------------------
// ZipDataStream
// -------------------------
ZipDataStream::ZipDataStream()
{

}

ZipDataStream::~ZipDataStream()
{

}

int ZipDataStream::Read(void* pBuf, int nCount)
{
	return 0;
}

void ZipDataStream::Close()
{

}

// -------------------------
// CArchiveManager
// -------------------------
CArchiveManager::CArchiveManager()
{
	s_Instance = this;
	m_mapFactory.clear();
	m_mapArchive.clear();
}

CArchiveManager::~CArchiveManager()
{

}

CArchiveManager& CArchiveManager::Instance()
{
	return *s_Instance;
}

void CArchiveManager::AddArchiveFactory(ArchiveFactory *pArchFactory)
{
	String &type = pArchFactory->GetType();
	m_mapFactory[type] = pArchFactory;
}

Archive* CArchiveManager::Load(String filename, String szType)
{
	ArchiveMap::iterator iter = m_mapArchive.find(filename);
	if (iter != m_mapArchive.end())
	{
		return iter->second;
	}

	ArchiveFactoryMap::iterator iterFac = m_mapFactory.find(szType);
	if (iterFac == m_mapFactory.end())
	{	
		// factory not found
		GAMEENGINE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"Cannot find an archive factory to deal with archive of type " + szType,
				"ArchiveManager::load");
	}

	Archive *pArchive = NULL;
	ArchiveFactory *pFactory = iterFac->second;
	if (pFactory != NULL)
	{
		pArchive = pFactory->CreateInstance(filename);
		pArchive->Load();
		m_mapArchive[filename] = pArchive;
	}

	return pArchive;
}

void CArchiveManager::UnLoad(char *filename)
{
	ArchiveMap::iterator iter = m_mapArchive.find(filename);
	if (iter == m_mapArchive.end())
	{
		return;
	}

	Archive *pArchive = iter->second;
	if (pArchive != NULL)
	{
		pArchive->UnLoad();

		ArchiveFactoryMap::iterator iterFac = m_mapFactory.find(pArchive->GetType());
		if (iterFac == m_mapFactory.end())
		{
			// Factory not found
			GAMEENGINE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"Cannot find an archive factory to deal with archive of type " + pArchive->GetType(),
				"ArchiveManager::UnLoad");
		}

		iterFac->second->DestroyInstance(pArchive);
		m_mapArchive.erase(iter);
	}
}

Archive* CArchiveManager::Find(String filename)
{
	ArchiveMap::iterator iter = m_mapArchive.find(filename);
	if (iter != m_mapArchive.end())
	{
		return iter->second;
	}

	return NULL;
}

NAMESPACEEND
