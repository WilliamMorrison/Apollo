#include <thread>
#include <future>
#include "framework/framework.hpp"
#include "network/network.hpp"
#include "printer/printer_management.hpp"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//std::thread web_thread( network::test );
	std::thread main_thread( apollo::launch );
	//web_thread.join( ); 
	main_thread.join( );
}