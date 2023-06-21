#include "../vendor/imgui/src/imgui.h"
#include "../vendor/imgui/src/backend/imgui_impl_dx11.h"
#include "../vendor/imgui/src/backend/imgui_impl_win32.h"
#include "framework/framework.hpp"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	apollo::launch( );
}