#pragma once

#include <string>

enum printer_info {
	STATE = 1,
	STATE_MESSAGE,
	HOSTNAME,
	SOFTWARE_VERSION,
	CPU_INFO,
	KLIPPER_PATH,
	PYTHON_PATH,
	LOG_FILE,
	CONFIG_FILE,
	ALL
};

class printer {
public: 
	printer( const std::string& ip, const std::string& name, std::string id ) : _ip_address( ip ), _printer_name( name ), _printer_id( id ) { }
	printer( ) { }
	~printer( ) {
		// clear data members
		_ip_address.clear( );
		_printer_name.clear( );
		_printer_id.clear();
	}

	// get functions
	std::string get_printer_ip( ) { return _ip_address; }
	std::string get_printer_name( ) { return _printer_name;  }
	std::string	get_printer_id( ) { return _printer_id; }

	void set_printer_ip( std::string ip ) { _ip_address = ip; }
	void set_printer_name( std::string name ) { _printer_name = name; }
	void set_printer_id( std::string id ) { _printer_id = id; }

	std::string get_printer_info( uint32_t info_type );

protected:
	std::string _ip_address;
	std::string _printer_name;
	std::string	_printer_id;
};

class printer_administration : printer {
public:
	void set_host_firmware_restart( );
	void set_host_restart( );
	void set_emergency_stop( );
};