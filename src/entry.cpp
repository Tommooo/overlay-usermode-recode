#pragma once
#ifndef ENTRY_CPP
#define ENTRY_CPP

#include "includes.hpp"

// not needed
auto update_pos( ) -> void
{
	while ( true ) 
	{
		HWND g_target_hwnd = { };
		while ( !g_target_hwnd )
			g_target_hwnd = FindWindowA( NULL, window_settings.window_name_target );

		RECT g_window_size = { };
		if ( g_target_hwnd )
		{
			GetWindowRect( g_target_hwnd, &g_window_size );

			window_settings.w = g_window_size.right - g_window_size.left;
			window_settings.h = g_window_size.bottom - g_window_size.top;

			if ( window_settings.hwnd )
				MoveWindow( window_settings.hwnd, g_window_size.left, g_window_size.top, window_settings.w, window_settings.h, TRUE );
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

auto wnd_proc( HWND hwnd, UINT u_message, WPARAM w_param, LPARAM l_param ) -> LRESULT CALLBACK
{
	if ( u_message == WM_PAINT )
	{
		D3DRender( );
	}
	else if ( u_message == WM_DESTROY )
	{
		ImGui::Shutdown( );

		DeleteObject( window_settings.window_class.hbrBackground );
		DestroyCursor( window_settings.window_class.hCursor );
		DestroyIcon( window_settings.window_class.hIcon );
		DestroyIcon( window_settings.window_class.hIconSm );

		PostQuitMessage( NULL );
	}
	else
	{
		ImGui_ImplWin32_WndProcHandler( hwnd, u_message, w_param, l_param );
		return DefWindowProcA( hwnd, u_message, w_param, l_param );
	}
}

auto initialize_window( HINSTANCE h_instance ) -> HWND WINAPI
{
	window_settings.window_class.cbSize = sizeof( WNDCLASSEX );
	window_settings.window_class.cbClsExtra = NULL;
	window_settings.window_class.cbWndExtra = NULL;
	window_settings.window_class.hCursor = LoadCursor( 0, IDC_ARROW );
	window_settings.window_class.hIcon = LoadIcon( 0, IDI_APPLICATION );
	window_settings.window_class.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	window_settings.window_class.hbrBackground = ( HBRUSH )CreateSolidBrush( RGB( 0, 0, 0 ) );
	window_settings.window_class.hInstance = h_instance; 
	window_settings.window_class.lpfnWndProc = wnd_proc;
	window_settings.window_class.lpszClassName = window_settings.window_name;
	window_settings.window_class.lpszMenuName = window_settings.window_name;
	window_settings.window_class.style = CS_VREDRAW | CS_HREDRAW;

	if ( !RegisterClassEx( &window_settings.window_class ) ) 
		exit( NULL );
	
	window_settings.hwnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, window_settings.window_name, window_settings.window_name, WS_POPUP, 1, 1, window_settings.w, window_settings.h, 0, 0, 0, 0 );

	MARGINS g_margin = { NULL, NULL, window_settings.w, window_settings.h };

	SetLayeredWindowAttributes(window_settings.hwnd, NULL, NULL, NULL);
	DwmExtendFrameIntoClientArea( window_settings.hwnd, &g_margin );
	D3DInitialize( window_settings.hwnd );

	return window_settings.hwnd;
}

auto WinMain(HINSTANCE h_instance, HINSTANCE instance_previous, LPSTR cmd_line, int cmd_show) -> int WINAPI
{
	ShowWindow( GetConsoleWindow ( ), SW_SHOW );

	AllocConsole( );

	freopen( "CONIN$" ,"r" , stdin );
	freopen( "CONOUT$" ,"w" , stdout );

	CreateThread( NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>( update_pos ), NULL, NULL, NULL );

	while ( window_settings.w < 640 && window_settings.h < 480 ) 
	{
		window_settings.hwnd_target = FindWindowA( NULL, window_settings.window_name_target );

		RECT window_size = { };
		GetWindowRect(window_settings.hwnd_target, &window_size);

		window_settings.w = window_size.right - window_size.left;
		window_settings.h = window_size.bottom - window_size.top;
	}
	
	TIMECAPS time_caps;
    if (timeGetDevCaps( &time_caps, sizeof( TIMECAPS ) ) != TIMERR_NOERROR ) 
	{
		UINT timer_resoltion = min( max ( time_caps.wPeriodMin, ( 1 /*ms / s*/ ) ), time_caps.wPeriodMax );
		timeBeginPeriod( timer_resoltion ); 
	}

	GetWindowThreadProcessId( window_settings.hwnd_target, &window_settings.process_id );

	window_settings.hwnd = initialize_window( reinterpret_cast<HINSTANCE>( h_instance ) );

	MSG g_message;
	if (window_settings.hwnd == NULL) 
		exit( NULL ); 

	ShowWindow(window_settings.hwnd, SW_SHOW);

	while ( true ) 
	{
		if ( PeekMessageA( &g_message, window_settings.hwnd, 0, 0, PM_REMOVE ) ) 
		{
			DispatchMessageA( &g_message );
			TranslateMessage( &g_message );
		}
	}

	DestroyWindow( window_settings.hwnd );
	UnregisterClassA( window_settings.window_name, reinterpret_cast<HINSTANCE>( h_instance ) );

	return NULL;
}

#endif // !ENTRY_CPP