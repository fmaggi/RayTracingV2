project 'ImGui'
	location 'imgui'

	kind 'StaticLib'

	language 'C++'
	cppdialect 'C++latest'
	staticruntime 'Off'

	files {
		'imgui/imconfig.h',
		'imgui/imgui.h',
		'imgui/imgui.cpp',
		'imgui/imgui_draw.cpp',
		'imgui/imgui_internal.h',
		'imgui/imgui_tables.cpp',
		'imgui/imgui_widgets.cpp',
		'imgui/imstb_rectpack.h',
		'imgui/imstb_textedit.h',
		'imgui/imstb_truetype.h',
		'imgui/imgui_demo.cpp',
	}

project 'GLFW'
	location 'GLFW'

	kind 'StaticLib'

	language 'C'
	staticruntime 'on'

	files {
		'GLFW/include/GLFW/glfw3.h',
    'GLFW/include/GLFW/glfw3native.h',
    'GLFW/src/internal.h',
    'GLFW/src/platform.h',
    'GLFW/src/mappings.h',
    'GLFW/src/context.c',
    'GLFW/src/init.c',
    'GLFW/src/input.c',
    'GLFW/src/monitor.c',
    'GLFW/src/platform.c',
    'GLFW/src/window.c',
    'GLFW/src/egl_context.c',
		'GLFW/src/glx_context.c',
    'GLFW/src/osmesa_context.c',
    'GLFW/src/null_platform.h',
    'GLFW/src/null_joystick.h',
    'GLFW/src/null_init.c',

    'GLFW/src/null_monitor.c',
    'GLFW/src/null_window.c',
    'GLFW/src/null_joystick.c',

		'GLFW/src/vulkan.c'
	}


	filter 'system:linux'
		pic 'On'

		systemversion 'latest'

		files {
			'GLFW/src/x11_init.c',
			'GLFW/src/x11_monitor.c',
			'GLFW/src/x11_window.c',
			'GLFW/src/xkb_unicode.c',
			'GLFW/src/posix_time.c',
			'GLFW/src/posix_thread.c',
			'GLFW/src/posix_module.c',
			'GLFW/src/posix_poll.c',
			'GLFW/src/osmesa_context.c',
			'GLFW/src/linux_joystick.c'
		}

		defines
		{
			'_GLFW_X11'
		}

	filter 'system:windows'
		systemversion 'latest'

		files {
			'GLFW/src/win32_init.c',
			'GLFW/src/win32_joystick.c',
			'GLFW/src/win32_module.c',
			'GLFW/src/win32_monitor.c',
			'GLFW/src/win32_time.c',
			'GLFW/src/win32_thread.c',
			'GLFW/src/win32_window.c',
			'GLFW/src/wgl_context.c',
			'GLFW/src/egl_context.c',
			'GLFW/src/osmesa_context.c'
		}

		defines {
			'_GLFW_WIN32',
			'_CRT_SECURE_NO_WARNINGS'
		}

		links {
			'Dwmapi.lib'
		}

