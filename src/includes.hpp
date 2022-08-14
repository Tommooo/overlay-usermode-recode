#pragma once
#ifndef INCLUDES_HPP
#define INCLUDES_HPP

// windows
#include <Windows.h>
#include <Process.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <dwmapi.h>

struct window_settings_t
{
	int w = 0;
	int h = 0;

	HWND hwnd_target = 0x0;
	HWND hwnd = 0x0;

	bool show_window = false;

	char window_name[16] = "Overlay"; 
	char window_name_target[64] = "Apex Legends"; 

	WNDCLASSEX window_class;
	DWORD process_id;
}inline window_settings;


template<std::intmax_t frames>
class frame_rater 
{
public:
    frame_rater() : time_between_frames{ 1 },tp{std::chrono::steady_clock::now()}{}

    void sleep()
	{
        tp += time_between_frames;
        std::this_thread::sleep_until(tp);
    }
private:
    std::chrono::duration<double, std::ratio<1, frames>> time_between_frames;
    std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

// imgui
#include "dependencies/ImGui/imgui.h"
#include "dependencies/ImGui/imgui_impl_dx9.h"
#include "dependencies/ImGui/imgui_impl_win32.h"
#include "dependencies/ImGui/imgui_internal.h"
#include "dependencies/ImGui/imgui_fonts.hpp"

// dx
#include "dependencies/DirectX9/Include/d3d9.h"
#include "dependencies/DirectX9/Include/d3dx9.h"

// dx impl
inline IDirect3D9Ex* p_instance;
inline IDirect3DDevice9Ex* p_device;
inline D3DPRESENT_PARAMETERS p_params;

// overlay
#include "overlay/d3d/direct3d.hpp"
#include "overlay/render/render.hpp"

#endif // !INCLUDES_HPP