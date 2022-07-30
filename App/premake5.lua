project 'App'
    kind 'ConsoleApp'

    language			'C++'
	cppdialect			'C++latest'
	conformancemode		'On'
	exceptionhandling	'Off'
	warnings			'Extra'
	floatingpoint		'Fast'
	staticruntime 'On'

    files { 'src/**.cpp', 'src/**.h' }
	includedirs	{ 'src', '../Raytracing', '../vendor/imgui', '../vendor/glm', '../vendor/GLFW/include' }

    links { 'Raytracing', 'ImGui', 'GLFW' }
