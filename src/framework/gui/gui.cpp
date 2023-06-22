#include <future>
#include "gui.hpp"
#include "../../printer/printer.hpp"
#include "../../printer/printer_management.hpp"
#include <Windows.h>
#include <fstream>

bool file_exists_in_exec_directoryl( const std::string& fileName ) {
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

void test5( ) {
	if ( !file_exists_in_exec_directoryl( "printers.json" ) )
		return;

	std::ifstream json_file( "printers.json" );
	if ( !json_file.is_open( ) ) {
		// failed to open the file 
		return;
	}

	nlohmann::json json_data;
	json_file >> json_data;

	std::vector<printer*> objects;
	for ( const auto& json_object : json_data ) {
		printer* a = new printer( );
		std::string ip = json_object[ "ip_address" ].dump();
		std::string name = json_object[ "name" ].dump();
		std::string id = json_object[ "id" ].dump();
		
		a->set_printer_ip( ip.c_str() );
		a->set_printer_name( name.c_str() );
		a->set_printer_id( id.c_str() );

		

		objects.push_back( a );
	}
	if ( ImGui::TreeNode( "Printers" ) ) {
		for ( const auto& object : objects ) {
			for ( int i = 0; i < objects.size( ); i++ ) {
				if ( i == 0 )
					ImGui::SetNextItemOpen( true, ImGuiCond_Once );

				if ( ImGui::TreeNode( ( void* )( intptr_t )i, "Printer %d", i ) ) {
					ImGui::Text( "Printer IP: %s", object->get_printer_ip( ).c_str( ) );
					ImGui::Text( "Printer Name: %s", object->get_printer_name( ).c_str( ) );
					ImGui::Text( "Printer ID: %s", object->get_printer_id( ).c_str() );
					ImGui::SameLine( );
				}
				ImGui::TreePop( );
			}
		}
	}
	ImGui::TreePop( );
}

void gui::render( ) {
	auto io = ImGui::GetIO( );


	if ( ImGui::BeginMainMenuBar( ) ) {
		if ( ImGui::BeginMenu( "File" ) ) {
			if ( ImGui::MenuItem( "Create Printer" ) ) {

			}

			if ( ImGui::MenuItem( "Open Printer Settings" ) ) {
				// Open printer settings dialog
			}

			ImGui::EndMenu( );
		}
	}ImGui::EndMainMenuBar( );

	ImGui::SetNextWindowPos( ImVec2( 0, 19 ) );
	ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x, io.DisplaySize.y + 19 ) );

	ImGui::Begin( "##Testing", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );

	/*static bool testing = false;
	if ( ImGui::Button( "Testing" ) ) {
		testing ^= 1;
	}*/

	std::future<bool> check_database = std::async( printer_management::check_database_available );
	std::future<int> num_objects = std::async( printer_management::check_current_printer_objects );
	int test = num_objects.get( );
	if ( check_database.get() ) {
		auto middle = ImVec2( ( io.DisplaySize.x / 2 ) - ( ImGui::CalcTextSize( strings::get_string_from_fault_code( DATABASE_LOAD_ERROR ).c_str( ) ).x / 2 ), ( io.DisplaySize.y / 2 ) );
		ImGui::SetCursorScreenPos( middle );
		ImGui::TextColored( ImColor( 255, 0, 0 ),
			strings::get_string_from_fault_code( DATABASE_LOAD_ERROR ).c_str( ) );

		/*ImGui::SetCursorPos( ImVec2( ImGui::GetWindowSize( ).x * 0.25f + ( ImGui::GetWindowSize( ).x * 0.125f), ( io.DisplaySize.y / 2 ) ) );
		ImGui::Button( "Create new printers.json", ImVec2( ImGui::GetWindowSize().x * 0.25f, 0.f) );*/

		ImGui::SetCursorPos( ImVec2( ImGui::GetWindowSize( ).x * 0.25f, ( io.DisplaySize.y / 2 ) ) );
		ImGui::Button( "Yes", ImVec2( ImGui::GetWindowSize( ).x * 0.25f, 0.f ) );
		ImGui::SameLine( );
		ImGui::Button( "No", ImVec2( ImGui::GetWindowSize( ).x * 0.25f, 0.f ) );

		printer* voron = new printer( "192.168.0.18", "voron 2.4", "1" );

		
		ImGui::TextWrapped( "Name: %s", voron->get_printer_name( ).c_str( ) );
		ImGui::TextWrapped( "State: %s", voron->get_printer_info( printer_info::STATE ).c_str( ) );
		ImGui::TextWrapped( "Sofware Version: %s", voron->get_printer_info( printer_info::SOFTWARE_VERSION ).c_str( ) );
	}

	//test5( );

	ImGui::End( );
}



void gui::add_printer( ) {

}

std::map<int, std::string> fault_code_strings {
	{0, "Printer database could not be loaded. Please ensure there is a \"printers.json\" in your app directory."},
	{ 1, "Database encountered an error while opening." },
	{ 2, "TEST" }
};

std::string gui::strings::get_string_from_fault_code( FAULT_CODE code ) {
	auto it = fault_code_strings.find( code );
	if ( it != fault_code_strings.end( ) ) {
		return it->second;
	}
	return "UNKNOWN_STRING";
}
