#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class UseImGui
{
	public:
		void Init(GLFWwindow* window);
		virtual void NewFrame();
		void Update();
		void Render();
		void Shutdown();
};
