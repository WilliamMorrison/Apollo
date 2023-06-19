#pragma once

#include <string>

class printer {
public: 
	printer(std::string ip_address = "", std::string printer_name = "", int printer_id = 0);
	~printer();

	// get functions
	class job_status {
	public:

	};

private:
	std::string _printer_name;
	std::string _ip_address;
	uint32_t	_printer_id;
};