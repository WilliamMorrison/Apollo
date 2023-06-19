#include "printer.hpp"

printer::printer(std::string ip_address, std::string printer_name, int printer_id){
	this->_ip_address = ip_address;
	this->_printer_name = printer_name;
	this->_printer_id = printer_id;
}

printer::~printer() {
	// clear data members

	_ip_address.clear();
	_printer_name.clear();
	_printer_id = 0; 
}