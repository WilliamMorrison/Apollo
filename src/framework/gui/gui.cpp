#include "gui.hpp"

void gui::render( ) {
	static bool test = true;
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

	static bool testing = true;
	if ( ImGui::Button( "End testing" ) ) {
		testing ^= 1;
	}

	if ( testing ) {
		auto middle = ImVec2( ( io.DisplaySize.x / 2 ) - ( ImGui::CalcTextSize( strings::get_string_from_fault_code( DATABASE_LOAD_ERROR ).c_str( ) ).x / 2 ), ( io.DisplaySize.y / 2 ) );
		ImGui::SetCursorScreenPos( middle );
		ImGui::TextColored( ImColor( 255, 0, 0 ), 
			strings::get_string_from_fault_code( DATABASE_LOAD_ERROR ).c_str( ) );


		/*ImGui::SetCursorPos( ImVec2( ImGui::GetWindowSize( ).x * 0.25f + ( ImGui::GetWindowSize( ).x * 0.125f), ( io.DisplaySize.y / 2 ) ) );
		ImGui::Button( "Create new printers.json", ImVec2( ImGui::GetWindowSize().x * 0.25f, 0.f) );*/

		ImGui::SetCursorPos( ImVec2( ImGui::GetWindowSize( ).x * 0.25f , ( io.DisplaySize.y / 2 ) ) );
		ImGui::Button( "Yes", ImVec2( ImGui::GetWindowSize( ).x * 0.25f, 0.f ) );
		ImGui::SameLine( );
		ImGui::Button( "No", ImVec2( ImGui::GetWindowSize( ).x * 0.25f, 0.f ) );

		ImGui::Button( "Hello world" );
	}

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
