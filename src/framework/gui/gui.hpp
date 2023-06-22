#pragma once

#include "../../../vendor/imgui/src/imgui.h"
#include "../../../vendor/imgui/src/backend/imgui_impl_dx11.h"
#include "../../../vendor/imgui/src/backend/imgui_impl_win32.h"
#include "../../network/network.hpp"

#include <string>
#include <map>

enum FAULT_CODE {
	DATABASE_LOAD_ERROR = 0,
	DATABASE_SAVE_ERROR,
	TEST_TEST
};



namespace gui {
	
	void render( );
	void add_printer(  );

	namespace window {
		static bool printer_add = false;
		static bool printer_update = false;
	}

	namespace strings {
		std::string get_string_from_fault_code( FAULT_CODE code );
	}
}