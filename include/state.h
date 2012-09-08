#ifndef _STATE_H_
#define _STATE_H_

#include <string>
#include <sys/time.h>
#include <unistd.h>
#ifdef X11
#	include <X11/keysym.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#endif

#include <common.h>

namespace cls
{
	class state
	{
	private:
		static state*      s_CurrentState;
		static std::string s_Title;
		static size_t      s_Width, s_Height;
		static bool        s_FullScreen;
		static bool        s_Quit;

		std::string        m_Name;
		state*             m_Parent;

		size_t             m_StartTime;
	
	public:
		static void init();
		static void enter(state* p_State, void* p_Parameter, size_t p_StartTime);
		static void close();

		state(const char* p_Name = "unnamed");
		virtual ~state();

		void   leave();
		size_t get_time();

		virtual void on_enter(void* p_Parameter);
		virtual void on_leave();
		virtual void on_display();
#ifdef X11
		virtual void on_key_press(XKeyPressedEvent& p_Event);
		virtual void on_key_release(XKeyReleasedEvent& p_Event);
#endif

		const char*  get_name() const;
	};
};

#endif
