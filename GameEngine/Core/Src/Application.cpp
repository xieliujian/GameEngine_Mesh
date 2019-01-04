
#include "Application.h"

NAMESPACEBEGIN(GameEngine)

Application::Application(HINSTANCE hInstance)
	:mMainWndCaption(L"D3D11 Application"),
	mClientWidth(1024),
	mClientHeight(768)
{
	mClearColor = Color(49.0f / 255.0f, 77.0f / 255.0f, 121.0f / 255.0f, 1.0f);
}

Application::~Application()
{

}

bool Application::Init()
{
	m_pLogManager = new LogManager;
	m_pLogManager->CreateLog("Log.txt", true);

	// Archive Manager
	m_pArchiveManager = new ArchiveManager;
	m_pFileArchiveFactory = new FileArchiveFactory;
	m_pArchiveManager->AddArchiveFactory(m_pFileArchiveFactory);
	m_pZipArchiveFactory = new ZipArchiveFactory;
	m_pArchiveManager->AddArchiveFactory(m_pZipArchiveFactory);

	m_pResourceGroupManager = new ResourceGroupManager;
	m_pMeshManager = new MeshManager;

	return true;
}

NAMESPACEEND
