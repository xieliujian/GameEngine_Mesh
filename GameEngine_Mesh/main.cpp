
#include "D3D11Application.h"
#include <Windows.h>
#include "Color.h"

using namespace GameEngine;

class GameEngine_Mesh : public D3D11Application
{
public:
	GameEngine_Mesh(HINSTANCE hInstance);
};

GameEngine_Mesh::GameEngine_Mesh(HINSTANCE hInstance)
	:D3D11Application(hInstance)
{

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	GameEngine_Mesh theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();

	return 0;
}