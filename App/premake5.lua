project 'App'
  kind 'ConsoleApp'

  language			'C++'
	cppdialect			'C++latest'
	conformancemode		'On'
	exceptionhandling	'Off'
	warnings			'Extra'
	floatingpoint		'Fast'
	staticruntime 'On'

  files { 'src/**.cpp', 'src/**.h', '../vendor/imgui/backends/imgui_impl_glfw.cpp', '../vendor/imgui/backends/imgui_impl_opengl3.cpp' }
	includedirs	{ 'src', '../Raytracing', '../vendor/imgui', '../vendor/glm', '../vendor/GLFW/include' }

  links { 'Raytracing', 'GLFW', 'ImGui', 'm', 'GL', 'X11', 'pthread', 'Xrandr', 'Xi', 'dl' } -- linux only for now
