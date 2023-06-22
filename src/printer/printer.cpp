#include <future>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "printer.hpp"
#include "../network/network.hpp"

std::string printer::get_printer_info( uint32_t info_type ) {
	std::string response = "";
	std::stringstream ss;

	ss << "http://" << get_printer_ip( ) << "/printer/info";

	std::string request_uri = cpr::Url{ ss.str( ) }.c_str( );
	std::future<nlohmann::json> send_async_request = std::async( network::send_base_network_request, request_uri );

	switch ( info_type ) {
		case printer_info::STATE:
		{
			response = send_async_request.get( )[ "result" ][ "state" ].get<std::string>( );
			break;
		}
		case printer_info::STATE_MESSAGE:
		{
			response = send_async_request.get( )[ "result" ][ "state_message" ].get<std::string>( );
			break;
		}
		case printer_info::HOSTNAME:
		{
			response = send_async_request.get( )[ "result" ][ "hostname" ].get<std::string>( );
			break;
		}
		case printer_info::SOFTWARE_VERSION:
		{
			response = send_async_request.get( )[ "result" ][ "software_version" ].get<std::string>();
			break;
		}
		case printer_info::CPU_INFO:
		{
			response = send_async_request.get( )[ "result" ][ "cpu_info" ].get<std::string>( );
			break;
		}
		case printer_info::KLIPPER_PATH:
		{
			response = send_async_request.get( )[ "result" ][ "klipper_path" ].get<std::string>( );
			break;
		}
		case printer_info::PYTHON_PATH:
		{
			response = send_async_request.get( )[ "result" ][ "python_path" ].get<std::string>( );
			break;
		}
		case printer_info::LOG_FILE:
		{
			response = send_async_request.get( )[ "result" ][ "log_file" ].get<std::string>( );
			break;
		}
		case printer_info::CONFIG_FILE:
		{
			response = send_async_request.get( )[ "result" ][ "config_file" ].get<std::string>( );
			break;
		}
		case printer_info::ALL:
		{
			response = send_async_request.get( )[ "result" ].dump( 4 );
			break;
		}
	}

	return response;
}
