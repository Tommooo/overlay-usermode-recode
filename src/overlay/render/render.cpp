#pragma once
#ifndef RENDER_CPP
#define RENDER_CPP

#include "render.hpp"

namespace render 
{
	void draw( )
	{
		ImVec4* colors	= ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.62f, 0.93f, 0.06f, 1.00f);

		ImGui::GetBackgroundDrawList()->AddTextOutline( { 10, 10 }, ImColor( 255,255,255,255 ), std::string( std::to_string( ( int )ImGui::GetIO( ).Framerate) + "fps" ).c_str( ) );
	}
}

#endif // !RENDER_CPP