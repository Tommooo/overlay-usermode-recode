#pragma once
#ifndef DIRECT3D_CPP
#define DIRECT3D_CPP

#include "direct3d.hpp"

auto D3DInitialize( HWND hwnd ) -> BOOL 
{
	Direct3DCreate9Ex( D3D_SDK_VERSION, &p_instance );

	p_params.Windowed = TRUE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferHeight = window_settings.h;
	p_params.BackBufferWidth = window_settings.w;
	p_params.MultiSampleQuality = DEFAULT_QUALITY;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; /* use frame_limiter instead as some people may have 60hz monitors ect */

	p_instance->CreateDeviceEx( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device );

	if ( p_device == NULL )
		return FALSE;

	ImGui::CreateContext( );

	ImFontConfig fontCfg = ImFontConfig( );

	fontCfg.OversampleH = 1;
	fontCfg.OversampleV = 1;
	fontCfg.PixelSnapH = true;

	ImGui_ImplWin32_Init( window_settings.hwnd );
	ImGui_ImplDX9_Init( p_device );

	return TRUE;
}

auto D3DRender( ) -> void
{

	SetWindowLong(window_settings.hwnd, GWL_EXSTYLE, window_settings.show_window ? WS_EX_LAYERED | WS_EX_TOPMOST : WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT );

	if ( GetAsyncKeyState( VK_INSERT ) & 1 ) 		
		window_settings.show_window = !window_settings.show_window;

	p_params.BackBufferHeight = window_settings.h;
	p_params.BackBufferWidth = window_settings.w;

	p_device->Reset( &p_params );
	p_device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
	p_device->BeginScene( );

    ImGui_ImplDX9_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );
    

	if ( window_settings.hwnd_target == GetForegroundWindow( ) || GetActiveWindow() == GetForegroundWindow( ) ) 
	{
		if ( window_settings.show_window )
		{
			// when menu open / input enabled
		}
		render::draw( );		
	}

	ImGui::EndFrame( );
	ImGui::Render( );

	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

	p_device->EndScene( );
	p_device->PresentEx( 0, 0, 0, 0, 0 );

	render_context::frame_rate.sleep( );
}

#endif // !DIRECT3D_CPP