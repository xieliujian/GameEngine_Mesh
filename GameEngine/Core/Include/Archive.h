

#ifndef __Archive_h__
#define __Archive_h__

#include "Prerequest.h"
#include "MyString.h"

NAMESPACEBEGIN(GameEngine)

class Archive;
class FileArchive;
class ZipArchive;
class ArchiveFactory;
class FileArchiveFactory;
class ZipArchiveFactory;
class DataStream;
class MemoryDataStream;
class FileStreamDataStream;
class FileHandleDataStream;
class ZipDataStream;

struct FileInfo
{
	String m_szName;
	int m_nSize;
	int m_nCompSize;
};

class Archive
{
public:
	Archive();
	Archive(String type, String name);
	virtual ~Archive();

	String& GetName();
	String& GetType();

	virtual void Load();
	virtual void UnLoad();
	virtual int GetFileNum() = 0;
	virtual String GetFileName(int nId) = 0;
	virtual DataStream* Open(char* filename) = 0;
protected:
	String m_szName;
	String m_szType;
};

class FileArchive : public Archive
{
public:
	FileArchive(String name);
	~FileArchive();

	virtual DataStream* Open(char* filename);
	virtual int GetFileNum();
	virtual String GetFileName(int nId);
};

class ZipArchive : public Archive
{
public:
	ZipArchive(String name);
	~ZipArchive();

	virtual void Load();
	virtual void UnLoad();
	virtual DataStream* Open(char* filename);
	virtual int GetFileNum();
	virtual String GetFileName(int nId);
};

class ArchiveFactory
{
public:
	ArchiveFactory();
	~ArchiveFactory();

	String GetType();
	void DestroyInstance(Archive *pArch);
	virtual Archive* CreateInstance(String filename) = 0;
protected:
	String m_szType;
};

class FileArchiveFactory : public ArchiveFactory
{
public:
	FileArchiveFactory();
	~FileArchiveFactory();

	virtual Archive* CreateInstance(String filename);
};

class ZipArchiveFactory : public ArchiveFactory
{
public:
	ZipArchiveFactory();
	~ZipArchiveFactory();

	virtual Archive* CreateInstance(String filename);
};

class DataStream
{
public:
	DataStream();
	~DataStream();

	virtual void Open(String filename){}
	virtual int Read(void* pBuf, int nCount) = 0;
	virtual void Close() = 0;
	virtual byte* GetBuffer(){return NULL;}
	virtual bool Eof(){return true;}
	virtual void Skip(long nCount){}
	String GetLine();
	int GetSize();
protected:
	String m_szName;
	int m_nSize;
};

class MemoryDataStream : public DataStream
{
public:
	MemoryDataStream();
	~MemoryDataStream();

	virtual int Read(void* pBuf, int nCount){return true;}
	virtual void Close(){}
};

class FileStreamDataStream : public DataStream
{
public:
	FileStreamDataStream(String name, std::ifstream *fp);
	~FileStreamDataStream();

	virtual int Read(void* pBuf, int nCount);
	virtual void Close(){}
	virtual bool Eof();
	virtual void Skip(long nCount);
private:
	std::ifstream *m_pStream;
};

class FileHandleDataStream : public DataStream
{
public:
	FileHandleDataStream();
	~FileHandleDataStream();

	virtual void Open(String filename);
	virtual int Read(void* pBuf, int nCount);
	virtual void Close();
private:
	FILE *m_pFile;
};

class ZipDataStream : public DataStream
{
public:
	ZipDataStream();
	~ZipDataStream();

	virtual int Read(void* pBuf, int nCount);
	virtual void Close();
};

class ArchiveManager
{
public:
	ArchiveManager();
	~ArchiveManager();

	static ArchiveManager& Instance();

	void AddArchiveFactory(ArchiveFactory *pArchFactory);
	Archive* Load(String filename, String szType);
	void UnLoad(char *filename);
	Archive* Find(String filename);
private:
	static ArchiveManager *s_Instance;

	typedef std::map<String, ArchiveFactory*> ArchiveFactoryMap;
	typedef std::map<String, Archive*> ArchiveMap;
	ArchiveFactoryMap m_mapFactory;
	ArchiveMap m_mapArchive;
};

NAMESPACEEND

#endif