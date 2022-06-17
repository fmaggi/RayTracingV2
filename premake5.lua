output_dir = '%{cfg.buildcfg}_%{cfg.architecture}_%{cfg.system}'

workspace 'RayTrace'
	startproject		'raytrace'
	architecture		'x64'
	configurations		{ 'Debug', 'Development', 'Release' }
	flags				'MultiProcessorCompile'
	language			'C++'
	cppdialect			'C++latest'
	conformancemode		'On'
	exceptionhandling	'Off'
	warnings			'Extra'
	floatingpoint		'Fast'
	staticruntime		'On'

	files {
			'src/**.cpp',
			'src/**.h',
			'vendor/glm/glm/**.hpp',
			'vendor/glm/glm/**.inl'
	}

	debugdir			('bin/'	 .. output_dir .. '/%{prj.name}')
	targetdir			('bin/'	 .. output_dir .. '/%{prj.name}')
	objdir				('bin_int/' .. output_dir .. '/%{prj.name}')

	filter 'Debug'
		symbols			'On'
		runtime			'Debug'

	filter 'Development'
		symbols			'On'
		optimize		'Speed'
		runtime			'Debug'
		flags			'LinkTimeOptimization'

	filter 'Release'
		optimize		'Speed'
		runtime			'Release'
		flags			'LinkTimeOptimization'
		defines			'NDEBUG'

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		toolset("clang")

project 'raytrace'
	location			'src'
	kind				'ConsoleApp'
	includedirs			{ '', 'src', 'vendor/glm'}

