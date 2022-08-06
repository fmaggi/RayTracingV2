#include "application.h"
#include <GL/gl.h>
#include <iostream>

#include "Raytracing.h"

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::seconds;

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

static int createTexture() {
	int tID;
	glGenTextures(1, (GLuint*)&tID);
	glBindTexture(GL_TEXTURE_2D, tID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return tID;
}

static void loadTextureData(int tID, const Image& image) {
	glBindTexture(GL_TEXTURE_2D, tID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels);
}

Application* Application::Create(ApplicationSpecification spec) {
	if (app == nullptr) {
		app = new Application(spec);
	}
	return app;
}

Application::Application(ApplicationSpecification spec) {
	this->spec = spec;
	Init();
}

void Application::Init() {
	if (!glfwInit())
		return;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL)
		return;
	glfwMakeContextCurrent((GLFWwindow*)window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

  // Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::run() {
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	float a = 16.0f / 9.0f;

	Camera camera(glm::vec3(0, 0.5f, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 70, a);

//	Scene scene = randomScene();
	Scene scene;
	Material* left = scene.addMaterial<Dielectric>("l", glm::vec3(1.0f), 1.5f);
	Material* ground = scene.addMaterial<Lambertian>("g", glm::vec3(0.8, 0.8, 0.0));
	Material* center = scene.addMaterial<Lambertian>("c", glm::vec3(0.1, 0.2, 0.5));
	Material* right = scene.addMaterial<Metal>("r", glm::vec3(0.8, 0.6, 0.2));
	Material* em = scene.addMaterial<Emissive>("e", glm::vec3(1.f), 30);
	scene.add<Sphere>(glm::vec3(0, -100.5f, -1), 100, ground);
	scene.add<Sphere>(glm::vec3(0, 0.f, -1), 0.5f, center);
	scene.add<Sphere>(glm::vec3(1, 0.f, -1), 0.5f, right);
	scene.add<Sphere>(glm::vec3(-1, 0.5f, -1), 0.5f, left);
	scene.add<Sphere>(glm::vec3(-1, 0.5f, -1), -0.4f, left);
	scene.add<Sphere>(glm::vec3(1, 0., 2), 0.5f, em);
	scene.add<Sphere>(glm::vec3(-1, 8, -1), 0.5f, em);
	scene.background = [](Ray r) {
			return glm::vec3(0.03f);
		};

	Renderer renderer;
	renderer.imageWidth = 400*a;
	renderer.imageHeight = 400;
	renderer.samples = 5;

	const Image& image = renderer.getImage();
	int tID = createTexture();
	// loadTextureData(tID, image);
	while (!glfwWindowShouldClose((GLFWwindow*)window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", nullptr, window_flags);
			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("AppDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Viewport");

			width = ImGui::GetContentRegionAvail().x;
			height = ImGui::GetContentRegionAvail().y;

			ImVec2 size;
			size.x = image.width;
			size.y = image.height;
			ImGui::Image((void*)tID, size);

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::Begin("Settings");

			loadTextureData(tID, image);
			if (renderer.working()) {
				ImGui::Text("Rendering... (%u)", renderer.rowsToGo());
			} else {
				if (ImGui::Button("Render")) {
					renderer.render(camera, scene);
				}
			}

			ImGui::SliderInt("Samples", (int*)&renderer.samples, 5, 5000);

			ImGui::Text("Image size %u %u", image.width, image.height);

			ImGui::End();

			ImGui::End();
		}

	 	ImGui::Render();
	 	int display_w, display_h;
	 	glfwGetFramebufferSize((GLFWwindow*)window, &display_w, &display_h);
	 	glViewport(0, 0, display_w, display_h);
	 	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	 	glClear(GL_COLOR_BUFFER_BIT);
	 	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		auto& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers((GLFWwindow*)window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow((GLFWwindow*)window);
	glfwTerminate();

	// Need to change format from RGBA to RGB to save it as a ppm
	// std::string path = "output/" + spec.outputFile;
	// FILE* im = std::fopen(path.c_str(), "wb");
	// std::fprintf(im, "P6\n%i %i\n255\n", image.width, image.height);
	// std::fwrite(image.pixels, image.pixelSize, image.width*image.height, im);
}
