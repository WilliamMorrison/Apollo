#pragma once

#include <d3d9.h>
#include <tchar.h>
#include <Windows.h>


#include "../../vendor/imgui/src/imgui.h"
#include "../../vendor/imgui/src/backend/imgui_impl_dx9.h"
#include "../../vendor/imgui/src/backend/imgui_impl_win32.h"

namespace apollo {
	namespace directx {
		static LPDIRECT3D9              p_d3d = nullptr;
		static LPDIRECT3DDEVICE9        d3d_device = nullptr;
		static UINT                     resize_width = 0, resize_height = 0;
		static D3DPRESENT_PARAMETERS    d3dpp = {};

		bool create_device(HWND hwnd);
		void cleanup_device();
		void reset_device();


	}

	void launch();
	void start();
	void reload();
	void exit();

	void app_load_printer_details();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );