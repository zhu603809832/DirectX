#include "GameApplication.h"
#pragma warning(disable:4244)

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	GameApplication tApp(hInstance);
	if (!tApp.Init())
		return 1;

	tApp.Run();
	tApp.UnInit();
	return 0;
}