project 'Raytracing'
	kind 'StaticLib'

	language			'C++'
	cppdialect			'C++latest'
	conformancemode		'On'
	exceptionhandling	'Off'
	warnings			'Extra'
	floatingpoint		'Fast'
	staticruntime 'On'

	files { 'src/**.cpp', 'src/**.h', '../vendor/glm/glm/**.hpp', '../vendor/glm/glm/**.inl' }
	includedirs	{ 'src', '../vendor/glm' }
