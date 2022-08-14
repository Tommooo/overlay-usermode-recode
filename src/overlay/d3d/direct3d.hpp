#pragma once
#ifndef DIRECT3D_HPP
#define DIRECT3D_HPP

#include "../../includes.hpp"

auto D3DRender( ) -> void;
auto D3DInitialize( HWND hWnd ) -> BOOL;

namespace render_context
{
	static frame_rater<120> frame_rate; 
}

#endif // !DIRECT3D_HPP
