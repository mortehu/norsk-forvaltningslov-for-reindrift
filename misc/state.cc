#include <stdexcept>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <GL/gl.h>
#if defined(X11)
#	include <GL/glx.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#elif defined(WIN32)
#	include <windows.h>
#else
#	error "Platform not supported"
#endif

#include <state.h>
#include <sound.h>

namespace cls
{
	state*      state::s_CurrentState = NULL;
	std::string state::s_Title = "Celsius";
	size_t      state::s_Width = 640, state::s_Height = 480;
	bool        state::s_FullScreen = false;
	bool        state::s_Quit = false;

#if defined(X11)
	static Display* s_Display;
	static Window   s_Window;
	static int      s_Attributes[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };

	static Bool wait_for_map_notify(Display* p_Display, XEvent* p_Event, char* p_Argument)
	{
		if((p_Event->type == MapNotify)
		&& (p_Event->xmap.window == (Window) p_Argument))
			return GL_TRUE;

		return GL_FALSE;
	}
#elif defined(WIN32)
	static HWND  s_Window;
	static HDC   s_DeviceContext;
	static HGLRC s_RenderContext;

	static long PASCAL render_window_callback(HWND p_Window, UINT p_Message, 
		UINT p_WParam, LONG p_LParam)
	{
		switch(p_Message)
		{
		case WM_KEYDOWN:
			switch((int) p_WParam)
			{
			case VK_ESCAPE:
				break;
			default:
				return DefWindowProc(p_Window, p_Message, p_WParam, p_LParam);
			}
			break;
		case WM_CREATE:
			break;
		case WM_CLOSE:
			DestroyWindow(p_Window);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(p_Window, p_Message, p_WParam, p_LParam);
		}

		return 0;
	}
#endif

	void state::init()
	{
		std::cerr << "Initializing graphics state machine..." << std::endl;

#if defined(X11)
		XVisualInfo*         l_XVisualInfo;
		Colormap             l_Colormap;
		XSetWindowAttributes l_XSetWindowAttributes;
		GLXContext           l_GLXContext;
		XEvent               l_Event;
		int                  l_Dummy;

		if(s_FullScreen)
			putenv(strdup("MESA_GLX_FX=fullscreen"));
		else
			putenv(strdup("MESA_GLX_FX=window"));

		if(!getenv("DISPLAY"))
			throw std::runtime_error("DISPLAY variable not set");

		if(!(s_Display = XOpenDisplay(NULL)))
			throw std::runtime_error(
				  std::string("Failed to open display: ") 
				+ std::string(getenv("DISPLAY")));

		if(!glXQueryExtension(s_Display, &l_Dummy, &l_Dummy))
			throw std::runtime_error("X server does not support the GLX extension");

		// XXX: create own visual chooser

		if(!(l_XVisualInfo = glXChooseVisual(s_Display, DefaultScreen(s_Display), s_Attributes)))
			throw std::runtime_error("glXChooseVisual() failed");

		if(!(l_GLXContext = glXCreateContext(s_Display, l_XVisualInfo, None, GL_TRUE)))
			throw std::runtime_error("glXCreateContext() failed");

		l_Colormap = XCreateColormap(s_Display, RootWindow(s_Display, l_XVisualInfo->screen), l_XVisualInfo->visual, AllocNone);

		l_XSetWindowAttributes.colormap = l_Colormap;
		l_XSetWindowAttributes.border_pixel = 0;
		l_XSetWindowAttributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;

		s_Window = XCreateWindow(s_Display, RootWindow(s_Display, l_XVisualInfo->screen), 0, 0, s_Width, s_Height, 0, l_XVisualInfo->depth, InputOutput, l_XVisualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &l_XSetWindowAttributes);

		XSetStandardProperties(s_Display, s_Window, s_Title.c_str(), s_Title.c_str(), None, NULL, 0, NULL);

		glXMakeCurrent(s_Display, s_Window, l_GLXContext);

		XMapWindow(s_Display, s_Window);
		XIfEvent(s_Display, &l_Event, wait_for_map_notify, (char*) s_Window);
#elif defined(WIN32)
		WNDCLASS l_WindowClass;

		memset(&l_WindowClass, 0, sizeof(l_WindowClass));
		l_WindowClass.lpfnWndProc = render_window_callback;
		l_WindowClass.hInstance = GetModuleHandle(NULL);
		l_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		l_WindowClass.lpszClassName = "RenderWindowClass";

		if(!RegisterClass(&l_WindowClass))
			throw std::runtime_error("Unable to register window class");
	
		if(!(s_Window = CreateWindow("RenderWindowClass", "RenderWindow",
			WS_POPUP, 0, 0, s_Width, s_Height, NULL, NULL,
			GetModuleHandle(NULL), NULL)))
			throw std::runtime_error("Failed to create render window");

		ShowWindow(s_Window, SW_SHOWDEFAULT);

		if(!UpdateWindow(s_Window))
			throw std::runtime_error("UpdateWindow() failed");

		if(!(s_DeviceContext = GetDC(s_Window)))
			throw std::runtime_error("Failed to get device context");

		DEVMODE l_DeviceMode;

		memset(&l_DeviceMode, 0, sizeof(l_DeviceMode));
		l_DeviceMode.dmSize = sizeof(l_DeviceMode);
		l_DeviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		l_DeviceMode.dmPelsWidth = s_Width;
		l_DeviceMode.dmPelsHeight = s_Height;
		l_DeviceMode.dmBitsPerPel = 32; // XXX: query this

		ChangeDisplaySettings(&l_DeviceMode, CDS_FULLSCREEN);

		PIXELFORMATDESCRIPTOR l_PixelFormat;

		memset(&l_PixelFormat, 0, sizeof(l_PixelFormat));
		l_PixelFormat.nSize = sizeof(l_PixelFormat);
		l_PixelFormat.nVersion = 1;
		l_PixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		l_PixelFormat.iPixelType = PFD_TYPE_RGBA;
		l_PixelFormat.cColorBits = 24;
		l_PixelFormat.cDepthBits = 16;
		l_PixelFormat.cAlphaBits = 0;
		l_PixelFormat.cStencilBits = 8;
		l_PixelFormat.iLayerType = PFD_MAIN_PLANE;

		int i_PixelFormat = ChoosePixelFormat(s_DeviceContext, &l_PixelFormat);

		SetPixelFormat(s_DeviceContext, i_PixelFormat, &l_PixelFormat);

		if(!(s_RenderContext = wglCreateContext(s_DeviceContext)))
			throw std::runtime_error("wglCreateContext() failed");

		if(!wglMakeCurrent(s_DeviceContext, s_RenderContext))
			throw std::runtime_error("wglMakeCurrent() failed");
#endif
	}

	void state::enter(state* p_State, void* p_Parameter, size_t p_StartTime)
	{
		if(sound::is_open())
		{
			if(sound::stream_position() < p_StartTime)
				return;
		}

		std::cerr << "Entering state `" << p_State->m_Name << "'..." << std::endl;

		s_Quit = false;
		
		p_State->m_Parent = s_CurrentState;
		s_CurrentState = p_State;

		if(sound::is_open())
		{
			s_CurrentState->m_StartTime = p_StartTime;
		}
		else
		{
			struct timeval l_TimeOfDay;

			gettimeofday(&l_TimeOfDay, NULL);

			s_CurrentState->m_StartTime = l_TimeOfDay.tv_sec * 1000 
			                            + l_TimeOfDay.tv_usec / 1000;
		}

		s_CurrentState->on_enter(p_Parameter);

		if(!s_CurrentState->m_Parent)
		{
			while(!s_Quit)
			{
				sound::stream_update();

				s_CurrentState->on_display();

#if defined(X11)
				glXWaitGL();

				glXSwapBuffers(s_Display, s_Window);

				while(!s_Quit && XPending(s_Display))
				{
					XEvent l_Event;

					XNextEvent(s_Display, &l_Event);
				
					switch(l_Event.type)
					{
					case KeyPress:

						s_CurrentState->on_key_press(*(XKeyPressedEvent*) &l_Event);

						break;

					case KeyRelease:

						s_CurrentState->on_key_release(*(XKeyReleasedEvent*) &l_Event);

						break;

					case ConfigureNotify:

						// resize(l_Event.xconfigure.width, l_Event.xconfigure.height);

						break;
					}
				}
#elif defined(WIN32)
				SwapBuffers(s_DeviceContext);

				MSG l_Message;

				while(!s_Quit && PeekMessage(&l_Message, NULL, 0, 0, PM_REMOVE))
				{
					if(l_Message.message == WM_QUIT)
						s_Quit = true;

					TranslateMessage(&l_Message);
					DispatchMessage(&l_Message);
				}
#endif
			}
		}
	}
	
	void state::leave()
	{
		this->on_leave();
		
		s_CurrentState = this->m_Parent;

		if(!s_CurrentState)
			s_Quit = true;
	}

	size_t state::get_time()
	{
		if(sound::is_open())
		{
			return sound::stream_position() - m_StartTime;
		}
		else
		{
			struct timeval l_TimeOfDay;

			gettimeofday(&l_TimeOfDay, NULL);

			return l_TimeOfDay.tv_sec * 1000 + l_TimeOfDay.tv_usec / 1000 - m_StartTime;
		}
	}

	void state::close()
	{
#if defined(WIN32)
		wglMakeCurrent(s_DeviceContext, NULL);
		wglDeleteContext(s_RenderContext);
		DeleteDC(s_DeviceContext);
		DestroyWindow(s_Window);
#endif

		while(s_CurrentState)
			s_CurrentState->leave();
	}

	state::state(const char* p_Name)
	{
		std::cerr << "Creating state `" << p_Name << "'..." << std::endl;

		m_Name = p_Name;
	}

	state::~state()
	{
	}
	
	void state::on_enter(void* p_Parameter)
	{
	}

	void state::on_leave()
	{
	}

	void state::on_display()
	{
	}

#if defined(X11)
	void state::on_key_press(XKeyPressedEvent& p_Event)
	{
		KeySym l_Key;

		XLookupString(&p_Event, NULL, 0, &l_Key, NULL);

		if(l_Key == XK_Escape)
			state::close();
	}

	void state::on_key_release(XKeyReleasedEvent& p_Event)
	{
	}
#endif

	const char* state::get_name() const
	{
		return m_Name.c_str();
	}
};

