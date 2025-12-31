#include "Application.h"

namespace HydrogenViewer
{

	Application::Application()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "Unable to Init GLFW";
		}

		m_Window.MakeWindow(1000, 500, "Hydrogen Viewer");

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Something Went Wrong, Kill yourself \n";
			std::cin.get();
		}
	}

	Application::~Application()
	{
		for (auto &i : m_Models)
			Hydrogen::Loader::Free(&i);

		m_Window.DestroyWindow();
		glfwTerminate();
	}

	void Application::Run()
	{
		Setup();

		static double LastTime = 0;
		while (m_IsRunning)
		{
			

			Event();
			Update();
			Render();

			double Current = glfwGetTime();
			m_deltaTime = (Current-LastTime)*600.0f;
			LastTime = Current;
		}
	}




	void Application::Setup()
	{


		//Configure the keyboard and keys
		Keyboard::InitKeyboard(m_Window.GetWindow());

		//Configure the Mouse:
		Mouse::InitMouse(m_Window.GetWindow());

		Hydrogen::Loader::SetUpHydrogen(Hydrogen::OPENGL);

		std::string RootPath = "C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\";
		ModelNames = { "Cage.glb" , "ComplexModel.gltf", "Cone.gltf", "scene.gltf"};
		

		for (auto &i : ModelNames)
		{
			Hydrogen::Model* model = Hydrogen::Loader::Load(RootPath+i);
			if (!model)
			{
				std::cout << "Failed to Load the Model\n";
				continue;
			}
			m_Models.push_back(model);
		}

		m_CurrentModel = (m_Models.size() != 0)? m_Models[0] : nullptr;


		m_Shader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");

		m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, 3.0f), 2, 0.01, 800.0f);
		m_IsRunning = true;


		//ImGui Setup
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		ImGui_ImplGlfw_InitForOpenGL(m_Window.GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}

	void Application::Event()
	{

		if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse)
		{
			m_Camera.DisableCameraMovement(true);
		}
		else
		{
			m_Camera.DisableCameraMovement(false);
		}

		m_Camera.HandleCameraMovement(m_deltaTime);
		m_Camera.HandleCameraLooking();

		glfwPollEvents();
		m_IsRunning = !glfwWindowShouldClose(m_Window.GetWindow());
	}

	void Application::Update()
	{
		m_Shader.SetUniformMat4("View", glm::value_ptr (m_Camera.GetView()));
		m_Shader.SetUniformMat4("Projection", glm::value_ptr(m_Camera.GetProjection()));
		
	}

	void Application::Render()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_CurrentModel != nullptr)
			m_CurrentModel->RenderScene(m_Shader, m_CurrentModel->GetDefaultScene());

		RenderUI();

		m_Window.ProcessWindow();
	}

	void Application::RenderUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Run Status");
		ImGui::Text("deltaTime: %f", m_deltaTime);
		
		for (int i = 0; i < ModelNames.size(); i++)
		{
			if (ImGui::Button(ModelNames[i].c_str()))
			{
				m_CurrentModel = (m_Models.size() > i) ? m_Models[i] : nullptr;
			}
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

};