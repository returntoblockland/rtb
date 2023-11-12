// Copyright (c) Johnny Patterson

#include <csignal>
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

static void signal_handler(int signal);

#ifdef _WIN32
static BOOL WINAPI signal_handler(DWORD signal);
static LRESULT CALLBACK window_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static WNDPROC s_original_window_callback = nullptr;
#endif

int main(int argc, char* argv[])
{
	std::signal(SIGABRT, signal_handler);
	std::signal(SIGINT, signal_handler);
	std::signal(SIGSEGV, signal_handler);
	std::signal(SIGTERM, signal_handler);

#ifdef _WIN32
	SetConsoleCtrlHandler(signal_handler, TRUE);
#endif

	imgui_rtb_app_state app;
	app.title = "Return to Blockland";
	app.clear_color = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	app.width = 1280;
	app.height = 720;
	app.show_demo_window = false;

	app.init = [&app]()
	{
		auto& io = ImGui::GetIO();

		// Do not save UI state.
		io.IniFilename = nullptr;

#ifdef _WIN32
		// Add our own window callback to the chain in order to handle interrupts.
		HWND hWnd = glfwGetWin32Window(app.window);
		s_original_window_callback = reinterpret_cast<WNDPROC>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_callback));
#endif
	};

	app.loop = [&app]()
	{
	};

	return imgui_main_loop(app);
}

static void signal_handler(int signal)
{
	std::quick_exit(0);
}

#ifdef _WIN32

static BOOL signal_handler(DWORD signal)
{
	std::quick_exit(0);
	return FALSE;
}

static LRESULT CALLBACK window_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_QUIT || msg == WM_QUERYENDSESSION || msg == WM_ENDSESSION)
		std::quick_exit(0);

	if (s_original_window_callback)
		return CallWindowProc(s_original_window_callback, hWnd, msg, wParam, lParam);

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

#endif
