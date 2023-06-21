#pragma once

#include <d3d9.h>
#include <D3D11.h>	
#include <tchar.h>
#include "../../vendor/imgui/src/imgui.h"
#include "../../vendor/imgui/src/backend/imgui_impl_dx11.h"
#include "../../vendor/imgui/src/backend/imgui_impl_win32.h"


namespace apollo {
	namespace directx {
		static ID3D11Device*				d3d_device = nullptr;
		static ID3D11DeviceContext*			d3d_device_ctx= nullptr;
		static IDXGISwapChain*				swap_chain = nullptr;
		static UINT							resize_width = 0, resize_height = 0;
		static ID3D11RenderTargetView*		render_target_view = nullptr;


		bool create_device(HWND hwnd);
		void cleanup_device();
		void create_render_target( );
		void cleanup_render_target( );
		void reset_device();
	}

	void launch();
	void initialize();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );