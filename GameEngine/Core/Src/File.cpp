
#include "File.h"

NAMESPACEBEGIN(GameEngine)

//-----------------------------
//----------File--------------
//-----------------------------
File::File()
{
	m_fp = 0;
	m_pBuffer = 0;
	m_dwFileSize = 0;
	m_dwFileOffset = 0;
}

File::~File()
{

}

int File::LoadFileToMemory( const char* pszFilename, const char *pszMode, BYTE** ppBuffer )
{
	FILE* fp = NULL;
	::fopen_s(&fp, pszFilename, pszMode);
	if( fp == NULL )
		return false;

	::fseek ( fp, 0, SEEK_END );
	DWORD dwFileSize = ::ftell( fp );
	if( dwFileSize == 0 )
	{
		::fclose( fp );
		return 0;
	}
	BYTE* pBuffer = new BYTE[dwFileSize];
	::rewind( fp );
	::fread( pBuffer, dwFileSize, 1, fp );
	::fclose( fp );
	*ppBuffer = pBuffer;
	return dwFileSize;
}

void File::ReleaseFileMemory( BYTE** ppBuffer )
{
	if( *ppBuffer )
	{
		delete []*ppBuffer;
		*ppBuffer = 0;
	}
}

bool File::fopen( const char* pszFilename, const char* pszMode )
{
	fclose();
		
	m_dwFileSize = LoadFileToMemory( pszFilename, pszMode, &m_pBuffer );
	m_dwFileOffset = 0;
	if( m_dwFileSize > 0 )
		return true;

	return false;
}

void File::fclose()
{
	if( m_fp )
	{
		::fclose( m_fp );
		m_fp = 0;
	}
		
	if(m_pBuffer )
	{
		ReleaseFileMemory( &m_pBuffer );
		m_pBuffer = NULL;
	}
}

BYTE* File::GetBuffer()
{ 
	return m_pBuffer;
}

DWORD File::GetBufferSize()
{ 
	return m_dwFileSize;
}

//-----------------------------
//--------FilePath------------
//-----------------------------
FilePath::FilePath()
{

}

FilePath::~FilePath()
{

}

void FilePath::Split(char *filename)
{
	_splitpath_s( filename, m_szDrive, m_szDir, m_szName, m_szExt );
}

char* FilePath::GetDrive()
{
	return m_szDrive;
}

char* FilePath::GetDirectory()
{
	return m_szDir;
}

char* FilePath::GetFileName()
{
	return m_szName;
}

void FilePath::SetCurDirectory(char *directory)
{
	::SetCurrentDirectoryA(directory);
}

bool FilePath::IsDirectory(char *pathname)
{
	HANDLE fhd;
	WIN32_FIND_DATAA fd;
		
	if((fhd = ::FindFirstFileA(pathname, &fd))==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		::FindClose(fhd);
		return TRUE;
	}
	else
	{
		::FindClose(fhd);
		return FALSE;
	}
}

bool FilePath::IsFileExist(char *filename)
{
	HANDLE fhd;
	WIN32_FIND_DATAA fd;

	if((fhd = ::FindFirstFileA(filename, &fd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if((fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
	{
		::FindClose(fhd);
		return TRUE;
	}
	else
	{
		::FindClose(fhd);
		return FALSE;
	}
}

bool FilePath::MakeDirectory(char *pathname)
{
	if( !pathname )
	{
		return false;
	}

	char szPath[MAX_PATH];
	strncpy_s(szPath, pathname, sizeof(szPath) - 1);

	int nLength = strlen( szPath );
	std::vector<std::string> vectorDir;
	char* pDirNameStart = szPath;
	for( unsigned int i = 0; i < nLength; i++ )
	{
		if( szPath[i] == '\\' || szPath[i] == '/')
		{
			szPath[i] = 0;
			vectorDir.push_back( pDirNameStart );
			pDirNameStart = &szPath[i + 1];
		}
	}

	szPath[0] = 0;
	for( int i = 0; i < vectorDir.size(); i++ )
	{
		size_t freesize = sizeof(szPath) - strlen(szPath);
		strncat_s( szPath, vectorDir[i].c_str(), freesize - 1 );
		if( !IsDirectory( szPath ) )
		{
			::CreateDirectoryA( szPath, NULL );
		}

		freesize = sizeof(szPath) - strlen(szPath);
		strncat_s( szPath, "\\", freesize - 1 );
	}

	return true;
}

NAMESPACEEND