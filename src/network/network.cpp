#include <future>	
#include <cpr/cpr.h>
#include "network.hpp"

nlohmann::json network::send_base_network_request( std::string request_uri ) {
	cpr::Response resp = cpr::Get( cpr::Url{ request_uri } );
	return nlohmann::json::parse( resp.text );
}