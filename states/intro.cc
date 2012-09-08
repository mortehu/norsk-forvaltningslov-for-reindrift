#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <GL/gl.h>

#include <state.h>
#include <matrix.h>
#include <image.h>
#include <font.h>
#include <states/intro.h>

namespace cls
{
	static vector3 s_Points[15];

	intro::intro(const char* p_Name, geom* p_Geometry) : state(p_Name)
	{
		m_Geometry = p_Geometry;

		s_Points[0] = vector3(0, 0, 64);
		s_Points[1] = vector3(0,             0, 64 + 128);
		s_Points[2] = vector3(0 + 128,       0, 64 + 128);
		s_Points[3] = vector3(0 + 256,       0, 64 + 128);
		s_Points[4] = vector3(0 + 256,       0 - 128, 64 + 128);
		s_Points[5] = vector3(0 + 256,       0 - 256, 64 + 128);
		s_Points[6] = vector3(0 + 256,       0 - 256, 64 + 128 - 128);
		s_Points[7] = vector3(0 + 256,       0 - 256 - 128, 64 + 128 - 128);
		s_Points[8] = vector3(0 + 256,       0 - 256 - 256, 64 + 128 - 128);
		s_Points[9] = vector3(0 + 256,       0 - 256 - 384, 64 + 128 - 128);
		s_Points[10] = vector3(0 + 256,       0 - 256 - 384,       64 + 128 - 128 + 256);
		s_Points[11] = vector3(0 + 256,       0 - 256 - 384 + 128, 64 + 128 - 128 + 256);
		s_Points[12] = vector3(0 + 256 - 128, 0 - 256 - 384 + 128, 64 + 128 - 128 + 256);
		s_Points[13] = vector3(0 + 256 - 128, 0 - 256 - 384 + 128, 64 + 128 - 128 + 256 - 128);
	}

	void intro::on_enter(void* p_Parameter)
	{
	}

	void intro::on_leave()
	{
	}

	void intro::on_display()
	{
		size_t l_Time = get_time();

		if(l_Time >= BEAT * 32)
		{
			state::leave();
			return;
		}

		{
			glClear(GL_DEPTH_BUFFER_BIT);

			/*
			size_t l_Offset = 
				  (int(l_Time / BEAT) >= 12 && int(l_Time / BEAT) < 16) 
				? int((l_Time / BEAT) * 0.25) * 4 
				: int((l_Time / BEAT) * 0.25) * 5;
				*/
			size_t l_Offset;
			if(l_Time / BEAT < 8)
				l_Offset = 0;
			else if(l_Time / BEAT < 12)
				l_Offset = 6;
			else if(l_Time / BEAT < 14)
				l_Offset = 9;
			else if(l_Time / BEAT < 16)
				l_Offset = 2;
			else if(l_Time / BEAT < 24)
				l_Offset = 0;
			else if(l_Time / BEAT < 28)
				l_Offset = 5;
			else if(l_Time / BEAT < 30)
				l_Offset = 10;
			else // l_Time >= BEAT * 30
				l_Offset = 7;

			vector3 l_Point0a = s_Points[(l_Time / 429 + l_Offset) % 14];
			vector3 l_Point1a = s_Points[(l_Time / 429 + l_Offset + 1) % 14];
			vector3 l_Point0d = s_Points[((l_Time + 429) / 429 + l_Offset     ) % 14];
			vector3 l_Point1d = s_Points[((l_Time + 429) / 429 + l_Offset  + 1) % 14];
			vector3 l_Point0b = s_Points[((l_Time + 400) / 429 + l_Offset     ) % 14];
			vector3 l_Point1b = s_Points[((l_Time + 400) / 429 + l_Offset  + 1) % 14];
			vector3 l_Point0c = s_Points[((l_Time + 3600) / 429 + l_Offset    ) % 14];
			vector3 l_Point1c = s_Points[((l_Time + 3600) / 429 + l_Offset + 1) % 14];
			float l_Scalea = (l_Time % 429) / 429.0 * 0.8;
			float l_Scaled = ((l_Time + 429) % 429) / 429.0 * 0.2;
			float l_Scaleb = ((l_Time + 400) % 429) / 429.0 * 0.6;
			float l_Scalec = ((l_Time + 3600) % 429) / 429.0 * 0.4;

			vector3 l_Position = l_Point0a * (0.8 - l_Scalea) + l_Point1a * l_Scalea
							   + l_Point0d * (0.2 - l_Scaled) + l_Point1d * l_Scaled;
			vector3 l_Target =   l_Point0b * (0.6 - l_Scaleb) + l_Point1b * l_Scaleb 
							 +   l_Point0c * (0.4 - l_Scalec) + l_Point1c * l_Scalec;

			camera l_Camera = camera(
				l_Position, 
				l_Target - l_Position, 
				(fmod(l_Time, 2 * BEAT) > BEAT)
				? (l_Time / 500.0 + sin(pow(fmod(l_Time, BEAT) / BEAT, 1.0/8) * M_PI) * 0.5)
				: (l_Time / 500.0 - sin(pow(fmod(l_Time, BEAT) / BEAT, 1.0/8) * M_PI) * 0.5));

			l_Camera.activate();

			m_Geometry->set_state();
			m_Geometry->render(l_Camera);
			m_Geometry->set_state(false);
		}

		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

			float l_Scale = fabs(sin(l_Time / BEAT * M_PI_4 / 2));
			float l_Offset = fmod(l_Time, BEAT * 8) * 0.001;
			const char* l_Text;

			if(l_Time < BEAT * 8)
				l_Text = "contributors: ";
			else if(l_Time < BEAT * 16)
				l_Text = "music: lavizh ";
			else if(l_Time < BEAT * 24)
				l_Text = "  3d: ebbex   ";
			else if(l_Time < BEAT * 32)
				l_Text = " code: circle ";
			else
				l_Text = "";

			glColor3f(l_Scale, l_Scale, l_Scale);

			font::put_text(
				vector2(0.8 - l_Offset, 0.4), 
				vector2(2.8 - l_Offset, -0.3), 
				l_Text);

			glColor3f(l_Scale * 0.5, l_Scale * 0.5, l_Scale * 0.5);

			font::put_text(
				vector2(0.8 - l_Offset * 1.1, 0.4), 
				vector2(1.8 - l_Offset * 1.1, 0.2), 
				l_Text);

			font::put_text(
				vector2(1.3 - l_Offset * 1.1, -0.2), 
				vector2(2.3 - l_Offset * 1.1, -0.4), 
				l_Text);

			glColor3f(l_Scale * 0.2, l_Scale * 0.2, l_Scale * 0.2);

			font::put_text(
				vector2(1.9 - l_Offset * 1.3, 0.4), 
				vector2(2.9 - l_Offset * 1.3, 0.3), 
				l_Text);

			font::put_text(
				vector2(0.4 - l_Offset * 1.3, -0.2), 
				vector2(1.4 - l_Offset * 1.3, -0.3), 
				l_Text);

			font::put_text(
				vector2(1.0 - l_Offset * 1.35, 0.05), 
				vector2(2.0 - l_Offset * 1.35, -0.05), 
				l_Text);

			font::put_text(
				vector2(2.7 - l_Offset * 1.3, 0.05), 
				vector2(3.7 - l_Offset * 1.3, -0.05), 
				l_Text);

		}

		float l_Brightness;
		if(l_Time / BEAT < 15)
			l_Brightness = 1 - l_Time / 1000.0;
		else if(l_Time / BEAT < 31.5)
			l_Brightness = 1 - fmod(l_Time - BEAT * 15, 2 * BEAT) / BEAT;
		else
			l_Brightness = (l_Time - BEAT * 31.5) / (BEAT * 0.5);

		if(l_Brightness > 0)
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

			glBindTexture(GL_TEXTURE_2D, 0);
			glColor3f(l_Brightness, l_Brightness, l_Brightness);
			glBegin(GL_POLYGON);
			glVertex2f(-1, -1);
			glVertex2f( 1, -1);
			glVertex2f( 1,  1);
			glVertex2f(-1,  1);
			glEnd();
		}

		glDisable(GL_BLEND);
	}
}
 
