// Copyright (c) Johnny Patterson
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ManagerApp.h"

#include "RTB/Scheduler.h"
#include "RTB/UI/Context.h"

#include <functional>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <imgui.h>

struct imgui_rtb_app_state
{
	std::function<void()> init;
	std::function<void()> loop;
	std::string title;
	GLFWwindow* window;
	ImVec4 clear_color;
	int width;
	int height;
	bool show_demo_window;
};

int imgui_main_loop(imgui_rtb_app_state& app);

namespace RTB {
namespace UI {

#ifdef _WIN32
static LRESULT CALLBACK WindowCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static WNDPROC s_originalWindowCallback = nullptr;
#endif

ManagerApp::ManagerApp(Context* ctx)
	: m_ctx { ctx }
{
	m_uiThread = std::move(std::thread(&ManagerApp::UIThread, this));
}

ManagerApp::~ManagerApp()
{
	if (m_uiThread.joinable())
		m_uiThread.join();
}

int ManagerApp::UIThread()
{
	imgui_rtb_app_state app;
	app.title = "Return to Blockland";
	app.clear_color = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	app.width = 1280;
	app.height = 720;
	app.show_demo_window = true;

	app.init = [&app]()
	{
		auto& io = ImGui::GetIO();

		// Disable viewports.
		io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;

		// Do not save UI state.
		io.IniFilename = nullptr;

#ifdef _WIN32
		// Add our own window callback to the chain in order to handle interrupts.
		HWND hWnd = glfwGetWin32Window(app.window);
		s_originalWindowCallback = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowCallback));
#endif
	};

	app.loop = [&]()
	{
		m_ctx->Scheduler->UIQueue().Flush();

		m_hub.Update(m_ctx);
	};

	int exitCode = imgui_main_loop(app);
	m_stopped = true;
	return exitCode;
}

#ifdef _WIN32
static LRESULT CALLBACK WindowCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_QUIT || msg == WM_QUERYENDSESSION || msg == WM_ENDSESSION)
		std::quick_exit(0);

	if (s_originalWindowCallback)
		return CallWindowProc(s_originalWindowCallback, hWnd, msg, wParam, lParam);

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}
#endif

} // namespace UI
} // namespace RTB
