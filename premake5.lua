output_dir = '%{cfg.buildcfg}_%{cfg.architecture}_%{cfg.system}'

workspace 'RayTrace'
	startproject		'App'
	architecture		'x64'

	configurations		{ 'Debug', 'Development', 'Release' }

	flags				'MultiProcessorCompile'

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

include 'vendor'
include 'Raytracing'
include 'App'
