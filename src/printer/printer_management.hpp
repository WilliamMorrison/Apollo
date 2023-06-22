#pragma once

// printer management, this will be used for adding / removing printers from the database

namespace printer_management {
	bool check_database_available( );
	int	 check_current_printer_objects( );
}