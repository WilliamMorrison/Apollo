#pragma once 

#include <string>	
#include <nlohmann/json.hpp>


namespace network {
	nlohmann::json send_base_network_request( std::string request_uri );
}