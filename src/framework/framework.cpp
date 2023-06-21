#include "framework.hpp"
#include "gui/gui.hpp"


void apollo::launch( ) { 
    initialize( );
}

void apollo::initialize( ) {
    WNDCLASSEXW wc = { sizeof( wc ), CS_CLASSDC, wnd_proc, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr, L"Apollo", nullptr };
    ::RegisterClassExW( &wc );
    auto window_style = ( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
    HWND hwnd = ::CreateWindowW( wc.lpszClassName, L"Apollo - A Print Farm Solution", window_style, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr );

    // Initialize Direct3D
    if ( !directx::create_device( hwnd ) ) {
        directx::cleanup_device( );
        ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
        return;
    }
    ::ShowWindow( hwnd, SW_SHOWDEFAULT );
    ::UpdateWindow( hwnd );

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); ( void )io;

    ImGui::StyleColorsDark( );

    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( directx::d3d_device, directx::d3d_device_ctx );
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

        if ( directx::resize_width != 0 && directx::resize_height != 0 ) {
            directx::cleanup_render_target( );
            directx::swap_chain->ResizeBuffers( 0, directx::resize_width, directx::resize_height, DXGI_FORMAT_UNKNOWN, 0 );
            directx::resize_width = directx::resize_width = 0;
            directx::create_render_target( );
        }

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        gui::render( );

        ImGui::Render( );
        const float clear_color_with_alpha[ 4 ] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        directx::d3d_device_ctx->OMSetRenderTargets( 1, &directx::render_target_view, nullptr );
        directx::d3d_device_ctx->ClearRenderTargetView( directx::render_target_view, clear_color_with_alpha );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        directx::swap_chain->Present( 1, 0 );
    }

    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    directx::cleanup_device( );
    ::DestroyWindow( hwnd );
    ::UnregisterClassW( wc.lpszClassName, wc.hInstance );
}

bool apollo::directx::create_device( HWND hwnd ) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL feature_level;
    const D3D_FEATURE_LEVEL feature_level_array[ 2 ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &d3d_device, &feature_level, &d3d_device_ctx );
    if ( hr == DXGI_ERROR_UNSUPPORTED )
        hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_array, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &d3d_device, &feature_level, &d3d_device_ctx );
    if ( hr != S_OK )
        return false;

    create_render_target( );
    return true;
}

void apollo::directx::cleanup_device( ) {
    cleanup_render_target( );

    if ( swap_chain ) { swap_chain->Release( ); swap_chain = nullptr; }
    if ( d3d_device_ctx ) { d3d_device_ctx->Release( ); d3d_device_ctx = nullptr; }
    if ( d3d_device ) { d3d_device->Release( ); d3d_device = nullptr; }
}

void apollo::directx::create_render_target( ) { 
    ID3D11Texture2D* back_buffer;
    swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) );
    d3d_device->CreateRenderTargetView( back_buffer, nullptr, &render_target_view );
    back_buffer->Release( );
}

void apollo::directx::cleanup_render_target( ) { 
    if ( render_target_view ) { render_target_view->Release( ); render_target_view = nullptr; }
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