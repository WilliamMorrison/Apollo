#include "framework.hpp"



void apollo::launch( ) { 
    start( );
}

void apollo::start( ) {
    WNDCLASSEXW wc = { sizeof( wc ), CS_CLASSDC, wnd_proc, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW( &wc );
    HWND hwnd = ::CreateWindowW( wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr );

    // Initialize Direct3D
    if ( !apollo::directx::create_device( hwnd ) ) {
        apollo::directx::cleanup_device( );
        ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
        return;
    }
    ::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );
    ::ShowWindow( hwnd, SW_SHOWDEFAULT );
    ::UpdateWindow( hwnd );

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); ( void )io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark( );

    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX9_Init( apollo::directx::d3d_device );
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

    // Main loop
    bool done = false;
    while ( !done ) {
        MSG msg;
        while ( ::PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            if ( msg.message == WM_QUIT )
                done = true;
        }
        if ( done )
            break;

        if ( apollo::directx::resize_width != 0 && apollo::directx::resize_height != 0 ) {
            apollo::directx::d3dpp.BackBufferWidth = apollo::directx::resize_width;
            apollo::directx::d3dpp.BackBufferHeight = apollo::directx::resize_height;
            apollo::directx::resize_width = apollo::directx::resize_height = 0;
            apollo::directx::reset_device( );
        }

        ImGui_ImplDX9_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        ImGui::Begin( "Test" );
        {
            ImGui::Text( "This is test text" );
            ImGui::End( );
        }

        ImGui::EndFrame( );
        apollo::directx::d3d_device->SetRenderState( D3DRS_ZENABLE, FALSE );
        apollo::directx::d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        apollo::directx::d3d_device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA( ( int )( clear_color.x * clear_color.w * 255.0f ), ( int )( clear_color.y * clear_color.w * 255.0f ), ( int )( clear_color.z * clear_color.w * 255.0f ), ( int )( clear_color.w * 255.0f ) );
        apollo::directx::d3d_device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
        if ( apollo::directx::d3d_device->BeginScene( ) >= 0 ) {
            ImGui::Render( );
            ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
            apollo::directx::d3d_device->EndScene( );
        }
        HRESULT result = apollo::directx::d3d_device->Present( nullptr, nullptr, nullptr, nullptr );

        if ( result == D3DERR_DEVICELOST && apollo::directx::d3d_device->TestCooperativeLevel( ) == D3DERR_DEVICENOTRESET )
            apollo::directx::reset_device( );
    }

    ImGui_ImplDX9_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    apollo::directx::cleanup_device( );
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}

void apollo::reload( ) { }

void apollo::exit( ) { }

void apollo::app_load_printer_details( ) { }

bool apollo::directx::create_device( HWND hwnd ) {
	if ( ( p_d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) == nullptr )
		return false;

    // Create the D3DDevice
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; 
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if ( p_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d_device ) < 0 )
        return false;

    return true;
}

void apollo::directx::cleanup_device( ) {
    if ( d3d_device ) { d3d_device->Release( ); d3d_device = nullptr; }
    if ( p_d3d ) { p_d3d->Release( ); p_d3d = nullptr; }
}

void apollo::directx::reset_device( ) { 
    ImGui_ImplDX9_InvalidateDeviceObjects( );
    auto hr = d3d_device->Reset( &d3dpp );
    if ( hr == D3DERR_INVALIDCALL )
        IM_ASSERT( 0 );

    ImGui_ImplDX9_CreateDeviceObjects( );
}

LRESULT WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg ) {
        case WM_SIZE:
            if ( wParam == SIZE_MINIMIZED )
                return 0;
            apollo::directx::resize_width = ( UINT )LOWORD( lParam );
            apollo::directx::resize_height = ( UINT )HIWORD( lParam );
            return 0;
        case WM_SYSCOMMAND:
            if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) 
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0;
    }
    return ::DefWindowProcW( hWnd, msg, wParam, lParam );
}
