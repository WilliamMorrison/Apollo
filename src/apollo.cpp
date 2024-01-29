#include <thread>
#include <future>
#include "framework/framework.hpp"
#include "network/network.hpp"
#include "printer/printer_management.hpp"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	std::thread main_thread( apollo::launch );
	main_thread.join( );
}
