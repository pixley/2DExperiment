#include <Windows.h>

#include "engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Engine* engine = new Engine();

	try
	{
		engine->Go();
		delete engine;
	}
	catch(char* e)
	{
		delete engine;
		MessageBoxA(NULL, e, "Exception Occured", MB_OK | MB_ICONERROR);
	}
}
