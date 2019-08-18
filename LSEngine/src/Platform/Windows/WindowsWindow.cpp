#include "lsepch.h"

#include "WindowsWindow.h"

#include "LSEngine/Events/ApplicationEvent.h"
#include "LSEngine/Events/MouseEvent.h"
#include "LSEngine/Events/KeyEvent.h"

#include <glad/glad.h>

namespace LSE {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LSE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		LSE_CORE_INFO("Creating Window Title=\"{0}\" Width={1} Height={2}", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			LSE_CORE_ASSERT(success, "Could not initialize GLFW");
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LSE_CORE_ASSERT(status, "Failed to load GLAD");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent e(width, height);
				data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent e;
				data.EventCallback(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_RELEASE)
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
				}
				else if (action == GLFW_PRESS)
				{
					KeyPressedEvent e(key);
					data.EventCallback(e);
				}
				else // action == GLFW_REPEAT
				{
					KeyRepeatedEvent e(key);
					data.EventCallback(e);
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				KeyTypedEvent e(key);
				data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				if (action == GLFW_RELEASE)
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
				}
				else // action == GLFW_PRESS
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				MouseScrolledEvent e(xoffset, yoffset);
				data.EventCallback(e);

			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent e(xpos, ypos);
				data.EventCallback(e);

			});
		
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowMovedEvent e(xpos, ypos);
				data.EventCallback(e);
			});

		glfwSetErrorCallback(GLFWErrorCallback);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval((int)enabled);

		m_Data.VSync = true;
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}