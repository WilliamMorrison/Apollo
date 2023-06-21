#pragma once 

#include <string>	
#include <cpr/cpr.h>
#include <fstream>
#include <nlohmann/json.hpp>


namespace network {
	nlohmann::json process_request( std::string request_uri );

	namespace printer {

	}
}