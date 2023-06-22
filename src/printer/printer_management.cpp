#include "printer_management.hpp"
#include <filesystem>
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include "printer.hpp"

bool file_exists_in_exec_directory( const std::string& fileName ) {
	char path[ MAX_PATH ];
	GetModuleFileNameA( NULL, path, MAX_PATH );

	std::string executablePath = path;
	size_t pos = executablePath.find_last_of( "\\/" );
	if ( pos != std::string::npos ) {
		executablePath = executablePath.substr( 0, pos );
	}

	std::string filePath = executablePath + "\\" + fileName;

	DWORD fileAttributes = GetFileAttributesA( filePath.c_str( ) );
	return ( fileAttributes != INVALID_FILE_ATTRIBUTES && !( fileAttributes & FILE_ATTRIBUTE_DIRECTORY ) );
}

bool printer_management::check_database_available( ) {

	if ( !file_exists_in_exec_directory( "printers.json" ) )
		return false;
	return true;
}

int printer_management::check_current_printer_objects( ) {
	int counter = 0;
	

	return counter;
}
