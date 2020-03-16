#include "lsepch.h"

#include "WindowsWindow.h"

#include "LSEngine/Events/ApplicationEvent.h"
#include "LSEngine/Events/MouseEvent.h"
#include "LSEngine/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

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
		m_Data.CursorState = true;
		m_Data.CursorStateChange = true;

		LSE_CORE_INFO("Creating Window Title=\"{0}\" Width={1} Height={2}", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			LSE_CORE_ASSERT(success, "Could not initialize GLFW");
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		s_Context = new OpenGLContext(m_Window);
		s_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		glfwSetInputMode(m_Window, GLFW_CURSOR_DISABLED, GLFW_CURSOR_NORMAL);

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
				
				MouseScrolledEvent e((float)(xoffset), (float)(yoffset));
				data.EventCallback(e);

			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				static float x = 0.f, y = 0.f;

				if (data.CursorStateChange)
				{
					data.CursorStateChange = false;
					MouseMovedEvent e((float)(xpos), (float)(ypos), 0.f, 0.f);
					data.EventCallback(e);
				}
				else
				{
					MouseMovedEvent e((float)(xpos), (float)(ypos), (float)(xpos)-x, (float)(ypos)-y);
					data.EventCallback(e);
				}

				x = (float)xpos;
				y = (float)ypos;
			});
		
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowMovedEvent e(xpos, ypos);
				data.EventCallback(e);
			});

		if (glfwRawMouseMotionSupported()) glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		else LSE_CORE_WARN("Raw input not supported");

		glfwSetErrorCallback(GLFWErrorCallback);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval((int)enabled);

		m_Data.VSync = true;
	}

	void WindowsWindow::SetCursorState(bool enabled)
	{
		m_Data.CursorState = enabled;
		m_Data.CursorStateChange = true;
		if (enabled) glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void WindowsWindow::OnUpdate()
	{
		s_Context->SwapBuffers();
		glfwPollEvents();
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}