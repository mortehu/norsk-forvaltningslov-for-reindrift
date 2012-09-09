#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <GL/gl.h>

#include <vector.h>
#include <state.h>
#include <matrix.h>
#include <image.h>
#include <font.h>
#include <primitives.h>
#include <states/space0.h>

namespace cls
{
	space0::space0(const char* p_Name) : state(p_Name)
	{
		m_Sphere = new sphere(4, 1);

		uint8_t* l_Data = new uint8_t[512 * 512 * 3];

		for(size_t i = 0; i < 512 * 512; i++)
		{
			l_Data[i * 3] =
			l_Data[i * 3 + 1] =
			l_Data[i * 3 + 2] =
				(rand() % 120 == 0) ?
					  ((size_t) ((double) rand() / RAND_MAX * 255))
					: 0;
		}

		m_Stars.set_geometry(512, 512);
		m_Stars.set_pixel_format(image::RGB);
		m_Stars.set_data(l_Data);

		m_Sun.load("data/textures/common/sun");
		m_Planet.load("data/textures/common/planet");
		m_Flares[0].load("data/textures/common/flare0");
		m_Flares[1].load("data/textures/common/flare1");
		m_Flares[2].load("data/textures/common/flare2");
		m_Flares[3].load("data/textures/common/flare3");
		m_Flares[4].load("data/textures/common/flare4");
	}

	void space0::on_enter(void* p_Parameter)
	{
		glEnable(GL_DEPTH_TEST);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
	}

	void space0::on_leave()
	{
	}

	void space0::on_display()
	{
		size_t l_Time = get_time();

		if(l_Time >= 34390 - BEAT / 2)
			state::leave();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);

		vector3 l_Position(100 - l_Time / 1500.0, -140, 20);
		vector3 l_Target(50 + l_Time / 1000.0, 0, -30);

		camera l_Camera = camera(l_Position, l_Target - l_Position, 0, 60);

		l_Camera.activate();

		glColor3f(1.0, 1.0, 1.0);

		m_Stars.activate();
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glScalef(1.0, 1.0, 1.0);
		glCullFace(GL_FRONT);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(5000, 5000, 5000);
		m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.5, 0.5, 0.5);
		glScalef(2.0, 2.0, 2.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(4000, 4000, 4000);
		m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.75, 0.75, 0.75);
		glScalef(2.0, 2.0, 2.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(2000, 2000, 2000);
		m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);

		m_Sun.activate();
		glColor3f(0.9, 0.9, 0.9);

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glRotatef(l_Time / 12.5, 0, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(10, 10, 10);
		m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glPushMatrix();
		glRotatef(20 - l_Time / 11.0, 0, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glScalef(10.5, 10.5, 10.5);
		m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();

		glDisable(GL_BLEND);

		glColor3f(1, 1, 1);

		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glScalef(2.0, 2.0, 2.0);
		m_Planet.activate();

		/*
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		vector3 l_LightPos(0, 0, 0);
		glLightfv(GL_LIGHT0, GL_POSITION, l_LightPos.get_data());
		float l_Ambient[] = { 0.4, 0.4, 0.4, 1 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, l_Ambient);
		float l_Diffuse[] = { 2, 2, 2, 1 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, l_Diffuse);
		float l_Specular[] = { 1, 1, 1, 1 };
		glLightfv(GL_LIGHT0, GL_SPECULAR, l_Specular);
		*/
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(115, -90, 0);
		glScalef(30, 30, 30);
		vector3 l_Direction(115, -90, 0);
		l_Direction.normalize();
		glBegin(GL_TRIANGLES);
		for(size_t i_Index = 0; i_Index < m_Sphere->m_IndexCount; i_Index++)
		{
			size_t& l_Index = m_Sphere->m_Indices[i_Index];
			vector3& l_Vertex = m_Sphere->m_Vertices[l_Index];

			float l_Intensity = pow(acos(l_Direction * m_Sphere->m_Normals[l_Index]) / M_PI, 2);

			glColor3f(l_Intensity, l_Intensity, l_Intensity);
			glTexCoord2fv(m_Sphere->m_TexCoords[0][l_Index].get_data());
			glVertex3fv(l_Vertex.get_data());
		}
		glEnd();
		//m_Sphere->render(l_Camera);
		glPopMatrix();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glDisable(GL_LIGHTING);

		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_BLEND);
		// *** Very bogus lens flare

		glBlendFunc(GL_ONE, GL_ONE);

		vector2 l_Sun = vector2(-0.37 - l_Time / 75000.0, 0.35);
		vector2 l_Dir = -l_Sun * 2.0;

		glColor3f(1, 1, 0.95);
		m_Flares[2].activate();
		glBegin(GL_QUADS);
		draw_billboard(
			vector2(l_Sun(0) - 0.3, l_Sun(1) + 0.4),
			vector2(l_Sun(0) + 0.3, l_Sun(1) - 0.4));
		glEnd();
		float l_Intensity = 1 - l_Sun.magnitude() * 0.5;

		for(size_t i_Flare = 0; i_Flare < 7; i_Flare++)
		{
			if(i_Flare == 2 || i_Flare == 6)
				glColor3f(0.6 * l_Intensity, 0.8 * l_Intensity, 1.0 * l_Intensity);
			else
				glColor3f(0.8 * l_Intensity, 0.9 * l_Intensity, 1.0 * l_Intensity);

			m_Flares[i_Flare % 5].activate();

			vector2 l_Pos = l_Sun + l_Dir * ((1 + i_Flare) / 7.0);

			float l_Scale = 1 - l_Pos.magnitude();

			glBegin(GL_QUADS);

			draw_billboard(
				vector2(l_Pos(0) - 0.15 * l_Scale, l_Pos(1) + 0.2 * l_Scale),
				vector2(l_Pos(0) + 0.15 * l_Scale, l_Pos(1) - 0.2 * l_Scale));

			glEnd();
		}

		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
		if(l_Time > BEAT * 16 && l_Time < BEAT * 32)
		{
			float l_Scale = pow(sin((l_Time - BEAT * 16) / (BEAT * 16) * M_PI), 0.5);
			glColor3f(l_Scale, l_Scale, l_Scale);

			font::put_text(vector2( -7 * 0.10, 0.4), vector2( 7 * 0.10, 0.0), "fadeout");
			font::put_text(vector2( -8 * 0.08, 0.0), vector2( 8 * 0.08,-0.4), "presents");
		}
		else if(l_Time > BEAT * 32 && l_Time < BEAT * 48)
		{
			float l_Scale = pow(sin((l_Time - BEAT * 32) / (BEAT * 16) * M_PI), 0.5);
			glColor3f(l_Scale, l_Scale, l_Scale);

			font::put_text(vector2( -1 * 0.10, 0.5), vector2( 1 * 0.10, 0.2), "a");
			font::put_text(vector2( -7 * 0.10, 0.2), vector2( 7 * 0.10,-0.1), "TG 2002");
			font::put_text(vector2(-12 * 0.08,-0.1), vector2(12 * 0.08,-0.4), "contribution");
		}
		else if(l_Time > BEAT * 48 && l_Time < 34390 - BEAT / 2)
		{
			float l_Scale = pow(sin((l_Time - BEAT * 48) / ((34390 - BEAT / 2) - BEAT * 48) * M_PI), 0.5);
			glColor3f(l_Scale, l_Scale, l_Scale);

			font::put_text(vector2( -5 * 0.06, 0.7), vector2( 5 * 0.06, 0.4), "Norsk");
			font::put_text(vector2(-15 * 0.06, 0.4), vector2(15 * 0.06, 0.1), "Forvaltningslov");
			font::put_text(vector2( -3 * 0.06, 0.1), vector2( 3 * 0.06,-0.2), "for");
			font::put_text(vector2(-10 * 0.06,-0.2), vector2(10 * 0.06,-0.5), "Reindrift");
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
};

