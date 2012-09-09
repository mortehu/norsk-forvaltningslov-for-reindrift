#include <string.h>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <GL/gl.h>

#include <state.h>
#include <matrix.h>
#include <image.h>
#include <font.h>
#include <particle.h>
#include <color.h>
#include <raytracer.h>
#include <states/base.h>

namespace cls
{
	base::base(const char* p_Name, geom* p_Geometry) : state(p_Name)
	{
		m_Geometry = p_Geometry;
		m_RefSphere = new sphere(4, 1);
		m_DynamicBlob = new sphere(4, 2);
		m_StaticBlob = new sphere(3, 1);
		m_Sphere = new sphere(4, 2);
		m_DynamicBlob2 = new sphere(3, 1);

		m_Green.load("data/textures/common/green");
		m_EnvMap.load("data/textures/common/envmap");
		m_LitCorners.load("data/textures/common/litcorners");

		for(size_t i_Vertex = 0; i_Vertex < m_StaticBlob->m_VertexCount; i_Vertex++)
		{
			vector3& l_Source = m_StaticBlob->m_Vertices[i_Vertex];

			m_StaticBlob->m_Vertices[i_Vertex] = vector3(
				(sin(2000 / 150.0 + l_Source(1) * M_PI * 2) * 0.1 + 0.9) * l_Source(0),
				(sin(2000 / 200.0 + l_Source(2) * M_PI    ) * 0.1 + 0.9) * l_Source(1),
				(sin(2000 / 250.0 + l_Source(0) * M_PI * 2) * 0.1 + 0.9) * l_Source(2));
		}

		m_StaticBlob->calc_normals();

		vector3 l_Eyes[10][20];
		vector3 l_Targets[10][20];

		l_Eyes[0][0] = vector3(-745.314, -573.719, -84.769);
		l_Targets[0][0] = vector3(-738.753, -524.152, -84.769);
		l_Eyes[0][1] = vector3(-717.458, -380.458, -38.6138);
		l_Targets[0][1] = vector3(-709.207, -331.143, -38.6138);
		l_Eyes[0][2] = vector3(-679.421, -191.493, 18.1984);
		l_Targets[0][2] = vector3(-665.65, -143.427, 18.1984);
		l_Eyes[0][3] = vector3(-618.395, 0.0625906, 55.336);
		l_Targets[0][3] = vector3(-601.376, 47.077, 55.336);
		l_Eyes[0][4] = vector3(-544.69, 184.891, 74.1692);
		l_Targets[0][4] = vector3(-523.617, 230.234, 74.1692);
		l_Eyes[0][5] = vector3(-437.475, 363.158, 74.1708);
		l_Targets[0][5] = vector3(-389.767, 378.121, 74.1708);
		l_Eyes[0][6] = vector3(-232.228, 388.022, 74.1708);
		l_Targets[0][6] = vector3(-182.501, 393.238, 74.1708);
		l_Eyes[0][7] = vector3(-28.8014, 373.47, 74.1708);
		l_Targets[0][7] = vector3(4.50511, 410.762, 74.1708);
		l_Eyes[0][8] = vector3(-22.6938, 572.282, 74.1708);
		l_Targets[0][8] = vector3(-35.0104, 620.741, 74.1708);
		l_Eyes[0][9] = vector3(-6.18291, 774.205, 74.1708);
		l_Targets[0][9] = vector3(4.27505, 823.099, 74.1708);
		l_Eyes[0][10] = vector3(32.9565, 972.518, 35.5976);
		l_Targets[0][10] = vector3(80.1099, 989.148, 35.5976);
		l_Eyes[0][11] = vector3(236.476, 985.933, 25.9696);
		l_Targets[0][11] = vector3(286.399, 988.692, 25.9696);
		l_Eyes[0][12] = vector3(437.592, 1001.12, 25.9696);
		l_Targets[0][12] = vector3(486.693, 1010.55, 25.9696);
		l_Eyes[0][13] = vector3(618.348, 1067.87, 102.579);
		l_Targets[0][13] = vector3(609.313, 1117.05, 102.579);
		l_Eyes[0][14] = vector3(658.141, 1237.09, 183.943);
		l_Targets[0][14] = vector3(613.576, 1259.76, 183.943);

		l_Eyes[1][0] = vector3(803.17, 1367.78, 223.096);
		l_Targets[1][0] = vector3(760.31, 1342.03, 223.096);
		l_Eyes[1][1] = vector3(636.97, 1260.03, 204.625);
		l_Targets[1][1] = vector3(607.767, 1219.45, 204.625);
		l_Eyes[1][2] = vector3(531.551, 1111.01, 128.751);
		l_Targets[1][2] = vector3(489.043, 1084.68, 128.751);
		l_Eyes[1][3] = vector3(395.456, 974.649, 83.0384);
		l_Targets[1][3] = vector3(346.35, 965.236, 83.0384);
		l_Eyes[1][4] = vector3(193.294, 962.924, 83.0384);
		l_Targets[1][4] = vector3(143.338, 960.832, 83.0384);
		l_Eyes[1][5] = vector3(-6.62603, 922.844, 83.0384);
		l_Targets[1][5] = vector3(-47.7903, 894.463, 83.0384);
		l_Eyes[1][6] = vector3(-19.0472, 712.384, 83.0384);
		l_Targets[1][6] = vector3(-16.5039, 662.449, 83.0384);
		l_Eyes[1][7] = vector3(-11.2514, 510.413, 83.0384);
		l_Targets[1][7] = vector3(-8.37928, 460.495, 83.0384);
		l_Eyes[1][8] = vector3(-5.61205, 309.73, 83.0384);
		l_Targets[1][8] = vector3(39.7879, 288.782, 83.0384);
		l_Eyes[1][9] = vector3(40.2203, 120.258, 83.0384);
		l_Targets[1][9] = vector3(90.2163, 120.892, 83.0384);
		l_Eyes[1][10] = vector3(205.809, 122.577, 83.0384);
		l_Targets[1][10] = vector3(245.485, 153.004, 83.0384);
		l_Eyes[1][11] = vector3(250.63, 316.435, 83.0384);
		l_Targets[1][11] = vector3(255.022, 366.241, 83.0384);
		l_Eyes[1][12] = vector3(277.554, 529.014, 83.0384);
		l_Targets[1][12] = vector3(319.845, 555.686, 83.0384);
		l_Eyes[1][13] = vector3(293.182, 724.677, 83.0384);
		l_Targets[1][13] = vector3(337.165, 700.896, 83.0384);
		l_Eyes[1][14] = vector3(483.4, 789.583, 83.0384);
		l_Targets[1][14] = vector3(473.76, 740.521, 83.0384);
		l_Eyes[1][15] = vector3(644.631, 679.647, 83.0384);
		l_Targets[1][15] = vector3(594.87, 674.762, 83.0384);
		l_Eyes[1][16] = vector3(702.126, 486.423, 83.0384);
		l_Targets[1][16] = vector3(664.109, 518.899, 83.0384);

		l_Eyes[2][0] = vector3(-388.514, 726.827, 1191.71);
		l_Targets[2][0] = vector3(-438.349, 730.875, 1191.71);
		l_Eyes[2][1] = vector3(-594.247, 739.7, 1176.14);
		l_Targets[2][1] = vector3(-643.985, 738.407, 1171.19);
		l_Eyes[2][2] = vector3(-793.995, 733.545, 1134.81);
		l_Targets[2][2] = vector3(-842.975, 731.088, 1125.07);
		l_Eyes[2][3] = vector3(-985.499, 729.381, 1074.56);
		l_Targets[2][3] = vector3(-1015.34, 730.775, 1034.47);
		l_Eyes[2][4] = vector3(-1030.59, 730.724, 869.596);
		l_Targets[2][4] = vector3(-1060.45, 731.54, 829.503);
		l_Eyes[2][5] = vector3(-1049.49, 737.427, 676.744);
		l_Targets[2][5] = vector3(-1034.73, 763.401, 636.651);
		l_Eyes[2][6] = vector3(-1033.14, 742.349, 474.126);
		l_Targets[2][6] = vector3(-993.546, 739.114, 443.768);
		l_Eyes[2][7] = vector3(-866.8, 732.407, 350.049);
		l_Targets[2][7] = vector3(-822.413, 735.47, 327.237);
		l_Eyes[2][8] = vector3(-671.342, 726.846, 281.097);
		l_Targets[2][8] = vector3(-626.875, 725.327, 258.285);
		l_Eyes[2][9] = vector3(-593.116, 727.923, 237.388);
		l_Targets[2][9] = vector3(-567.789, 728.636, 194.283);
		l_Eyes[2][10] = vector3(-576.454, 728.689, 149.969);
		l_Targets[2][10] = vector3(-531.147, 710.591, 139.029);
		l_Eyes[2][11] = vector3(-387.671, 739.034, 68.0474);
		l_Targets[2][11] = vector3(-432.402, 716.705, 68.8307);
		l_Eyes[2][12] = vector3(-491.454, 562.399, 68.0474);
		l_Targets[2][12] = vector3(-481.801, 610.999, 74.7514);

		l_Eyes[3][0] = vector3(752.122, 809.183, 80);
		l_Targets[3][0] = vector3(711.736, 779.704, 80);
		l_Eyes[3][1] = vector3(699.87, 614.685, 96.3456);
		l_Targets[3][1] = vector3(652.755, 625.537, 83.6006);
		l_Eyes[3][2] = vector3(526.618, 510.062, 75.0702);
		l_Targets[3][2] = vector3(515.868, 557.2, 62.3252);
		l_Eyes[3][3] = vector3(334.771, 568.011, 75.0702);
		l_Targets[3][3] = vector3(380.302, 584.274, 62.3252);
		l_Eyes[3][4] = vector3(420.918, 766.003, 75.0702);
		l_Targets[3][4] = vector3(430.774, 718.67, 62.3252);
		l_Eyes[3][5] = vector3(623.847, 794.814, 88.3954);
		l_Targets[3][5] = vector3(592.405, 758.085, 75.6504);
		l_Eyes[3][6] = vector3(757.174, 664.589, 88.3954);
		l_Targets[3][6] = vector3(709.174, 670.382, 75.6504);

		l_Eyes[4][0] = vector3(757.174, 664.589, 88.3954);
		l_Targets[4][0] = vector3(709.174, 670.382, 75.6504);
		l_Eyes[4][1] = vector3(655.672, 656.756, 85.1444);
		l_Targets[4][1] = vector3(606.395, 652.099, 78.072);
		l_Eyes[4][2] = vector3(515.393, 650.894, 85.1444);
		l_Targets[4][2] = vector3(500.08, 645.288, 105.522);
		l_Eyes[4][3] = vector3(486.372, 643.593, 139.284);
		l_Targets[4][3] = vector3(445.484, 623.273, 159.662);
		l_Eyes[4][4] = vector3(486.372, 643.593, 182.441);
		l_Targets[4][4] = vector3(483.647, 574.284, 190.265);
		l_Eyes[4][5] = vector3(481.756, 536.257, 182.441);
		l_Targets[4][5] = vector3(480.47, 486.89, 190.265);
		l_Eyes[4][6] = vector3(483.352, 411.728, 182.441);
		l_Targets[4][6] = vector3(483.009, 362.345, 190.265);

		m_EyeSplines[0] = new vector_spline(l_Eyes[0], 15);
		m_TargetSplines[0] = new vector_spline(l_Targets[0], 15);
		m_EyeSplines[1] = new vector_spline(l_Eyes[1], 17);
		m_TargetSplines[1] = new vector_spline(l_Targets[1], 17);
		m_EyeSplines[2] = new vector_spline(l_Eyes[2], 13);
		m_TargetSplines[2] = new vector_spline(l_Targets[2], 13);
		m_EyeSplines[3] = new vector_spline(l_Eyes[3], 7);
		m_TargetSplines[3] = new vector_spline(l_Targets[3], 7);
		m_EyeSplines[4] = new vector_spline(l_Eyes[4], 7);
		m_TargetSplines[4] = new vector_spline(l_Targets[4], 7);


		m_EyeSplines[0]->generate();
		m_EyeSplines[1]->generate();
		m_EyeSplines[2]->generate();
		m_EyeSplines[3]->generate();
		m_EyeSplines[4]->generate();
		m_TargetSplines[0]->generate();
		m_TargetSplines[1]->generate();
		m_TargetSplines[2]->generate();
		m_TargetSplines[3]->generate();
		m_TargetSplines[4]->generate();

		raytracer(*m_Geometry, 512, 512).render(
			vector3(480, 640, 80),   // Eye
			vector3(336, 528, -24),  // Upper left
			vector3(288, 0, 0),      // X direction
			vector3(0, 224, 0),      // Y direction
			m_RefractMap);           // Return image
	}

	void base::on_enter(void* p_Parameter)
	{
	}

	void base::on_leave()
	{
	}

	void base::on_display()
	{
		size_t l_Time = get_time();

		glClear(GL_DEPTH_BUFFER_BIT);

		vector3 l_Position, l_Direction;
		float l_Roll = 0, l_FOV = 80;

		if(l_Time < BEAT * 80)
		{
			l_Position = m_EyeSplines[0]->get_point(l_Time / (BEAT * 80));

			if(l_Time < BEAT * 48)
				l_Position += vector3(
					(double) rand() / RAND_MAX * 2.0 - 1.0,
					(double) rand() / RAND_MAX * 2.0 - 1.0,
					(double) rand() / RAND_MAX * 2.0 - 1.0) * (1 - fmod(l_Time, BEAT) / BEAT);

			l_Direction = m_TargetSplines[0]->get_point(l_Time / (BEAT * 80)) - l_Position;
		}
		else if(l_Time < BEAT * 112)
		{
			vector3 l_Offset(
				((fmod(l_Time, BEAT * 2) < BEAT)
				? (cos(l_Time / 600.0))
				: (cos(-l_Time / 600.0))),
				((fmod(l_Time, BEAT * 2) < BEAT)
				? sin(l_Time / 600.0)
				: sin(-l_Time / 600.0)),
				sin(l_Time / 200.0) * 0.4 - 0.3);
			l_Offset.normalize();
			l_Offset *= 190.0;

			l_Roll = (fmod(l_Time, BEAT * 2) < BEAT) ? sin(-l_Time / 800.0) : sin(l_Time / 800.0);
			l_Position = vector3(512, 1216, 180) + l_Offset;
			l_Direction = vector3(512, 1216, 180) - l_Position;
		}
		else if(l_Time < BEAT * 144)
		{
			vector3 l_Offset(
				cos(l_Time / 600.0),
				sin(l_Time / 595.0),
				sin(l_Time / 200.0) * 0.3 - 0.2);
			l_Offset.normalize();
			l_Offset *= 100.0;
			l_Roll = (l_Time - BEAT * 112) / (BEAT * 8) * M_PI;
			l_Position = vector3(512, 1216, 180) + l_Offset;
			l_Direction = vector3(512, 1216, 180) - l_Position;
		}
		else if(l_Time < BEAT * 176)
		{
			l_Position = m_EyeSplines[1]->get_point((l_Time - (BEAT * 144)) / (BEAT * 32));
			l_Direction = m_TargetSplines[1]->get_point((l_Time - (BEAT * 144)) / (BEAT * 32)) - l_Position;
		}
		else if(l_Time < BEAT * 208)
		{
			l_Position = m_EyeSplines[2]->get_point((l_Time - (BEAT * 176)) / (BEAT * 32));
			l_Direction = m_TargetSplines[2]->get_point((l_Time - (BEAT * 176)) / (BEAT * 32)) - l_Position;
		}
		else if(l_Time < BEAT * 240)
		{
			vector3 l_Offset(
				cos(l_Time / 1000.0),
				sin(l_Time / 980.0),
				sin(l_Time / 300.0) * 0.6);
			l_Offset.normalize();
			l_Offset *= 90.0;
			l_Roll = (l_Time - BEAT * 208) / (BEAT * 16) * M_PI;
			l_Position = vector3(-480, 648, 112) + l_Offset;
			l_Direction = vector3(-480, 648, 112) - l_Position;
		}
		else if(l_Time < BEAT * 260)
		{ }
		else if(l_Time < BEAT * 292)
		{
			l_Position = m_EyeSplines[3]->get_point((l_Time - (BEAT * 260)) / (BEAT * 32));
			l_Direction = m_TargetSplines[3]->get_point((l_Time - (BEAT * 260)) / (BEAT * 32)) - l_Position;
		}
		else if(l_Time < BEAT * 324)
		{
			l_Position = m_EyeSplines[4]->get_point((l_Time - (BEAT * 292)) / (BEAT * 32));
			l_Direction = m_TargetSplines[4]->get_point((l_Time - (BEAT * 292)) / (BEAT * 32)) - l_Position;
		}
		else if(l_Time < BEAT * 388)
		{
			vector3 l_Offset(cos(l_Time / 1000.0), sin(l_Time / 1000.0) * cos(l_Time / 1500.0), sin(l_Time / 1500.0));
			l_Offset.normalize();
			l_Offset *= 100 - 40 * pow(cos(l_Time / BEAT * M_PI), 2);
			l_Roll = pow(sin(l_Time / BEAT * M_PI), 3) * 0.5;
			l_Position = vector3(480, 320, 208) + l_Offset;
			l_Direction = vector3(480, 320, 208) - l_Position;
		}
		else if(l_Time < BEAT * 420)
		{
			vector3 l_Offset(cos(l_Time / 500.0), sin(l_Time / 500.0) * cos(l_Time / 800.0), sin(l_Time / 800.0));
			l_Offset.normalize();
			l_Offset *= 80.0;
			l_Position = vector3(480, 320, 208) + l_Offset;
			l_Direction = vector3(480, 320, 208) - l_Position;
		}
		else
		{
			state::leave();

			return;
		}

		m_Camera = camera(l_Position, l_Direction, l_Roll, l_FOV);

		m_Camera.activate();

		m_Geometry->set_state();
		m_Geometry->render(m_Camera);
		m_Geometry->set_state(false);

		if(l_Time > BEAT * 60 && l_Time < BEAT * 150)
			effect0(l_Time);

		if((l_Time > BEAT * 160 && l_Time < BEAT * 176)
		|| (l_Time > BEAT * 260 && l_Time < BEAT * 320))
			effect4(l_Time);

		if(l_Time > BEAT * 194 && l_Time < BEAT * 208)
			effect1(0);

		if(l_Time > BEAT * 208 && l_Time < BEAT * 240)
			effect1(l_Time);

		if(l_Time > BEAT * 292 && l_Time < BEAT * 388)
			effect2(l_Time);

		if(l_Time > BEAT * 388 && l_Time < BEAT * 420)
			effect3(l_Time);

		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			float l_LengthFactor = 1;
			const char* l_Text = "";
			float l_Scale = 1;

			if(l_Time < BEAT * 16)
			{
				l_Text = "roses are red";

				if(l_Time < BEAT * 8)
					l_LengthFactor = (l_Time / (BEAT * 8));

				if(l_Time > BEAT * 14)
					l_Scale = 1 - (l_Time - (BEAT * 14)) / (BEAT * 2);
			}
			else if(l_Time < BEAT * 32)
			{
				l_Text = "violets are blue";

				if(l_Time < BEAT * 24)
					l_LengthFactor = ((l_Time - (BEAT * 16)) / (BEAT * 8));

				if(l_Time > BEAT * 30)
					l_Scale = 1 - (l_Time - (BEAT * 30)) / (BEAT * 2);
			}
			else if(l_Time < BEAT * 48)
			{
				l_Text = "and so are you";

				if(l_Time < BEAT * 40)
					l_LengthFactor = ((l_Time - (BEAT * 32)) / (BEAT * 8));

				if(l_Time > BEAT * 46)
					l_Scale = 1 - (l_Time - (BEAT * 46)) / (BEAT * 2);
			}

			glColor3f(l_Scale * 0.9, l_Scale, l_Scale * 0.9);

			char* l_String = strdup(l_Text);
			l_String[size_t(strlen(l_Text) * l_LengthFactor)] = 0;
			font::put_text(
				vector2(strlen(l_String) * -0.025, -0.75),
				vector2(strlen(l_String) *  0.025, -0.8),
				l_String);
			free(l_String);

			if(l_Time > BEAT * 240 && l_Time < BEAT * 260)
			{
				glClear(GL_COLOR_BUFFER_BIT);

				glColor3f(1, 1, 1);

				font::put_text(
					vector2(9 * -0.1, 0.6),
					vector2(9 *  0.1, 0.3),
					"Greetings");

				const char l_Greets[9][41] =
				{
					"activator, neon, arcane, chroma, cypho,",
					"danner, daxxar, diggz, dropstar, edzes,",
					"dvalin, fuglemat, grimmy, grome, guard,",
					"guru, gus, ilmari, indh, irvin, raiden,",
					"kaktuskaffe, knus, plante, kusma, alge,",
					"lavos, longhair, lug00ber, turkey, tmk,",
					"norwolf, kravitz, snax, nelius, sirkew,",
					"silmaril, russlander, saftmelon, yannz,",
					"proteque, snillfisk, wolfram, titanstar",
				};

				for(size_t i = 0; i < 9; i++)
				{
					font::put_text(
						vector2(-0.9, 0.1 + i * -0.1),
						vector2(0.9,  i * -0.1),
						l_Greets[i]);
				}
				if(l_Time > BEAT * 256)
				{
					glColor4f(0, 0, 0, (l_Time - (BEAT * 256)) / (BEAT * 4));
					glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
					glBegin(GL_POLYGON);
					glVertex2f(-1, -1);
					glVertex2f( 1, -1);
					glVertex2f( 1,  1);
					glVertex2f(-1,  1);
					glEnd();
				}
			}

			glDisable(GL_BLEND);
		}
		float l_Brightness;
		if(l_Time < BEAT * 80)
			l_Brightness = 1 - l_Time / 1000.0;
		else if(l_Time < BEAT * 81)
			l_Brightness = 1 - (l_Time - BEAT * 80) / 1000.0;
		else if(l_Time < BEAT * 112)
			l_Brightness = 0.5 - (fmod(l_Time, BEAT) / (BEAT / 4));
		else if(l_Time < BEAT * 144)
			l_Brightness = 1 - (l_Time - BEAT * 112) / 1000.0;
		else if(l_Time < BEAT * 176)
			l_Brightness = 1 - (l_Time - BEAT * 144) / 1000.0;
		else if(l_Time < BEAT * 208)
			l_Brightness = 1 - (l_Time - BEAT * 176) / 1000.0;
		else if(l_Time < BEAT * 240)
			l_Brightness = 1 - (l_Time - BEAT * 208) / 1000.0;
		else if(l_Time < BEAT * 260)
			l_Brightness = 1 - (l_Time - BEAT * 240) / 1000.0;
		else if(l_Time < BEAT * 292)
			l_Brightness = 1 - (l_Time - BEAT * 260) / 1000.0;
		else if(l_Time < BEAT * 324)
			l_Brightness = 1 - (l_Time - BEAT * 292) / 1000.0;
		else if(l_Time < BEAT * 388)
			l_Brightness = 1 - (l_Time - BEAT * 324) / 1000.0;
		else if(l_Time < BEAT * 420)
			l_Brightness = 1 - (l_Time - BEAT * 388) / 1000.0;
		else
			l_Brightness = 1 - (l_Time - BEAT * 420) / 1000.0;


		if(l_Brightness > 0)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

			glDisable(GL_TEXTURE_2D);
			glColor3f(l_Brightness, l_Brightness, l_Brightness);
			glBegin(GL_POLYGON);
			glVertex2f(-1, -1);
			glVertex2f( 1, -1);
			glVertex2f( 1,  1);
			glVertex2f(-1,  1);
			glEnd();
			glDisable(GL_BLEND);
		}
	}

	void base::effect3(size_t p_Time)
	{
		for(size_t i_Vertex = 0; i_Vertex < m_DynamicBlob2->m_VertexCount; i_Vertex++)
		{
			vector3& l_Source = m_RefSphere->m_Vertices[i_Vertex];

			m_DynamicBlob2->m_Vertices[i_Vertex] = vector3(
				(sin(p_Time / 150.0 + l_Source(1) * M_PI * 3) * 0.3 + 0.7) * l_Source(0),
				(sin(p_Time / 200.0 + l_Source(2) * M_PI * 2) * 0.3 + 0.7) * l_Source(1),
				(sin(p_Time / 250.0 + l_Source(0) * M_PI * 3) * 0.3 + 0.7) * l_Source(2));
		}

		m_EnvMap.activate();
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(480, 320, 208);
		glScalef(50, 50, 50);
		m_DynamicBlob2->render(m_Camera);
		glPopMatrix();

		glDepthMask(GL_FALSE);

		particle l_Particle = particle(vector3(0, 0, 0), color(64, 64, 128), 10);

		l_Particle.set_state();

		for(size_t i_Vertex = 0; i_Vertex < m_DynamicBlob2->m_VertexCount; i_Vertex++)
		{
			l_Particle(0) = m_DynamicBlob2->m_Vertices[i_Vertex](0) * 50 + 480;
			l_Particle(1) = m_DynamicBlob2->m_Vertices[i_Vertex](1) * 50 + 320;
			l_Particle(2) = m_DynamicBlob2->m_Vertices[i_Vertex](2) * 50 + 208;

			l_Particle.render(m_Camera);
		}

		l_Particle.set_state(false);

		glDepthMask(GL_TRUE);
	}

	void base::effect2(size_t p_Time)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		m_Green.activate();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(480, 320, 208);
		glScalef(15, 15, 15);
		m_Sphere->render(m_Camera);
		glPopMatrix();

		particle l_Particle(vector3(480, 320, 208), color(255, 192, 128), 100);
		l_Particle.set_state();
		l_Particle.render(m_Camera);

		l_Particle.m_Color = color(32, 92, 128);

		for(size_t i_Vertex = 0; i_Vertex < m_Sphere->m_VertexCount; i_Vertex+=10)
		{
			for(size_t i_Hest = 0; i_Hest < 3; i_Hest++)
			{
				float l_Angle = p_Time / 500.0 + sin(p_Time / 800.0 + i_Hest * 0.3);
				float l_Scale = 25 + i_Hest * 6;
				l_Particle.m_Scale = 3.0 / (i_Hest + 1);
				l_Particle(0) = m_Sphere->m_Normals[i_Vertex](0) * l_Scale * cos(l_Angle)
 				              + m_Sphere->m_Normals[i_Vertex](1) * l_Scale * sin(l_Angle) + 480;
				l_Particle(1) = m_Sphere->m_Normals[i_Vertex](1) * l_Scale * cos(l_Angle)
 				              - m_Sphere->m_Normals[i_Vertex](0) * l_Scale * sin(l_Angle) + 320;
				l_Particle(2) = m_Sphere->m_Normals[i_Vertex](2) * l_Scale + 208;
				l_Particle.render(m_Camera);
			}
		}

		l_Particle.set_state(false);
		glDepthMask(GL_TRUE);
		glDisable(GL_DEPTH_TEST);
	}

	void base::effect1(size_t p_Time)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		float l_Angle0 = p_Time / 10.0;
		float l_Angle1 = p_Time / 9.0;
		float l_Angle2 = p_Time / 8.0;
		glMatrixMode(GL_MODELVIEW);
		m_EnvMap.activate();
		glColor3f(0.75, 1.0, 0.5);

		glPushMatrix();
		glTranslatef(-480, 648, 112);
		glRotatef(l_Angle0, 0, sin(p_Time / BEAT * M_PI_4), cos(p_Time / BEAT * M_PI_4));

		// *** Center spheres

			glPushMatrix();
			glTranslatef(32, 0, 0);
			glScalef(20, 20, 20);
			m_StaticBlob->render(m_Camera);
			glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-32, 0, 0);
			glScalef(20, 20, 20);
			m_StaticBlob->render(m_Camera);
			glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
			glPopMatrix();

			glPushMatrix();
			glRotatef(l_Angle1, 1, 0, 0);

				glPushMatrix();
				glTranslatef(0, 0, 32);
				glScalef(20, 20, 20);
				m_StaticBlob->render(m_Camera);
				glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(0, 0, -32);
				glScalef(20, 20, 20);
				m_StaticBlob->render(m_Camera);
				glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(0, 32, 0);
				glScalef(20, 20, 20);
				m_StaticBlob->render(m_Camera);
				glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(0, -32, 0);
				glScalef(20, 20, 20);
				m_StaticBlob->render(m_Camera);
				glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
				glPopMatrix();

				glPushMatrix();
				glRotatef(l_Angle2, 0, 1, 0);

					glPushMatrix();
					glTranslatef(40, -40, 0);
					glScalef(20, 20, 20);
					m_StaticBlob->render(m_Camera);
					glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(40,  40, 0);
					glScalef(20, 20, 20);
					m_StaticBlob->render(m_Camera);
					glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(-40, -40, 0);
					glScalef(20, 20, 20);
					m_StaticBlob->render(m_Camera);
					glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
					glPopMatrix();
					glPushMatrix();
					glTranslatef(-40,  40, 0);
					glScalef(20, 20, 20);
					m_StaticBlob->render(m_Camera);
					glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
					glPopMatrix();

					glPushMatrix();
					glRotatef(l_Angle2, 0, 0, 1);

						glPushMatrix();
						glTranslatef(0, -40, 40);
						glScalef(20, 20, 20);
						m_StaticBlob->render(m_Camera);
						glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(0,  40, 40);
						glScalef(20, 20, 20);
						m_StaticBlob->render(m_Camera);
						glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(0, -40, -40);
						glScalef(20, 20, 20);
						m_StaticBlob->render(m_Camera);
						glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(0,  40, -40);
						glScalef(20, 20, 20);
						m_StaticBlob->render(m_Camera);
						glScalef(0.9, 0.9, 0.9); m_StaticBlob->render(m_Camera);
						glPopMatrix();

					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

		glPopMatrix();
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}

	void base::effect0(size_t p_Time)
	{
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);

		for(size_t i_Vertex = 0; i_Vertex < m_DynamicBlob->m_VertexCount; i_Vertex++)
		{
			vector3& l_Source = m_RefSphere->m_Vertices[i_Vertex];

			m_DynamicBlob->m_Vertices[i_Vertex] = vector3(
				(sin(p_Time / 150.0 + l_Source(1) * M_PI * 2) * 0.3 + 0.7) * l_Source(0),
				(sin(p_Time / 200.0 + l_Source(2) * M_PI    ) * 0.3 + 0.7) * l_Source(1),
				(sin(p_Time / 250.0 + l_Source(0) * M_PI * 2) * 0.3 + 0.7) * l_Source(2));
		}

		m_DynamicBlob->calc_normals();

		for(size_t i_Vertex = 0; i_Vertex < m_DynamicBlob->m_VertexCount; i_Vertex++)
		{
			m_DynamicBlob->m_TexCoords[0][i_Vertex] = vector2(
				m_DynamicBlob->m_Normals[i_Vertex](1) * 0.15 + m_RefSphere->m_TexCoords[0][i_Vertex](0),
				m_DynamicBlob->m_Normals[i_Vertex](2) * 0.15 + m_RefSphere->m_TexCoords[0][i_Vertex](1));
			m_DynamicBlob->m_TexCoords[1][i_Vertex] = vector2(
				m_DynamicBlob->m_Normals[i_Vertex](0) * 0.5 + 0.5,
				m_DynamicBlob->m_Normals[i_Vertex](2) * 0.5 + 0.5);
		}

		m_Green.activate();
		m_EnvMap.activate(1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(512, 1216, 180);
		glScalef(50, 50, 50);
		m_DynamicBlob->render(m_Camera);
		glPopMatrix();

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
	}

	void base::effect4(size_t p_Time)
	{
		glEnable(GL_DEPTH_TEST);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);

		m_RefractMap.activate();

		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);

		float l_Elevation[21][16];
		vector3 l_Normals[21][16];

		for(size_t x = 0; x < 21; x++)
			for(size_t y = 0; y < 16; y++)
			{
				float l_Sinus = sin(p_Time * 0.005 + x / 1.5) * 0.8 + sin(p_Time * 0.008 + x * 1.5) * 0.2;
				float l_Cosinus = cos(p_Time * 0.006 + y / 1.4) * 0.8 + cos(p_Time * 0.009 + y * 2) * 0.2;
				float l_DeltaSinus = cos(p_Time * 0.005 + x / 1.5) * 0.8 + cos(p_Time * 0.008 + x * 1.5) * 0.2;
				float l_DeltaCosinus = -sin(p_Time * 0.006 + y / 1.4) * 0.8 - sin(p_Time * 0.009 + y * 2) * 0.2;

				l_Elevation[x][y] = -24 +
				                  + l_Sinus * 3.5
				                  + l_Cosinus * 3.5;

				l_Normals[x][y] = vector3(
					-l_DeltaSinus,
					-l_DeltaCosinus,
					2).normalize();
			}

		for(size_t x = 0; x < 20; x++)
		{
			for(size_t y = 0; y < 15; y++)
			{
				post_uv(x / 20.0, y / 15.0, l_Elevation[x][y], l_Normals[x][y]);
				glVertex3f( 336 + x * 288 / 20.0, 528 + y * 224 / 15.0, l_Elevation[x][y]);
				post_uv((x+1) / 20.0, y / 15.0, l_Elevation[x+1][y], l_Normals[x+1][y]);
				glVertex3f( 336 + (x+1) * 288 / 20.0, 528 + y * 224 / 15.0, l_Elevation[x+1][y]);
				post_uv((x+1) / 20.0, (y+1) / 15.0, l_Elevation[x+1][y+1], l_Normals[x+1][y+1]);
				glVertex3f( 336 + (x+1) * 288 / 20.0, 528 + (y+1) * 224 / 15.0, l_Elevation[x+1][y+1]);

				post_uv(x / 20.0, y / 15.0, l_Elevation[x][y], l_Normals[x][y]);
				glVertex3f( 336 + x * 288 / 20.0, 528 + y * 224 / 15.0, l_Elevation[x][y]);
				post_uv(x / 20.0, (y+1) / 15.0, l_Elevation[x][y+1], l_Normals[x][y+1]);
				glVertex3f( 336 + x * 288 / 20.0, 528 + (y+1) * 224 / 15.0, l_Elevation[x][y+1]);
				post_uv((x+1) / 20.0, (y+1) / 15.0, l_Elevation[x+1][y+1], l_Normals[x+1][y+1]);
				glVertex3f( 336 + (x+1) * 288 / 20.0, 528 + (y+1) * 224 / 15.0, l_Elevation[x+1][y+1]);
			}
		}
		glEnd();
	}

	void base::post_uv(float p_X, float p_Y, float p_Z, const vector3& p_Normal) const
	{
		vector3 l_Position(p_X * 368 - 152, p_Y * 232 - 24, p_Z);
		vector3 l_Direction = (l_Position - m_Camera.m_Position).normalize();

		float l_Angle = acos(-l_Direction * p_Normal);

		//l_Angle = asin(1.333 * sin(l_Angle));
		l_Angle = l_Angle / 1.333;

		vector3 l_Right = l_Direction.cross(p_Normal);
		l_Direction = -p_Normal * matrix().rotate(l_Right, l_Angle);

		float l_Distance[5];
		l_Distance[0] = (336 - vector3(1, 0, 0) * l_Position) / (vector3(1, 0, 0) * l_Direction);
		l_Distance[1] = (336+288 - vector3(-1, 0, 0) * l_Position) / (vector3(-1, 0, 0) * l_Direction);
		l_Distance[2] = (528 - vector3(0, 1, 0) * l_Position) / (vector3(0, 1, 0) * l_Direction);
		l_Distance[3] = (528+224 - vector3(0, -1, 0) * l_Position) / (vector3(0, -1, 0) * l_Direction);
		l_Distance[4] = (-128 - vector3(0, 0, 1) * l_Position) / (vector3(0, 0, 1) * l_Direction);
		int i_Shortest = -1;
		float l_Shortest = HUGE_VAL;

		for(size_t i = 0; i < 5; i++)
			if(l_Distance[i] < l_Shortest && l_Distance[i] > 0)
			{
				i_Shortest = i;
				l_Shortest = l_Distance[i_Shortest];
			}

		if(i_Shortest == -1)
			throw std::runtime_error("Under water");

		l_Position += l_Direction * l_Shortest;

		/*
			vector3(32, 96, 100),     // Eye
			vector3(-152, -24, -24),  // Upper left
			vector3(368, 0, 0),       // X direction
			vector3(0, 240, 0),       // Y direction
		*/

		vector3 l_Eye(480, 640, 80);

		l_Direction = l_Position - l_Eye;

		l_Direction.normalize();
		l_Direction *= -124.0 / l_Direction(2);

		l_Direction += l_Eye;

		float l_U = (l_Direction(0) + 152) / 368;
		float l_V = (l_Direction(1) + 24) / 240;

		glTexCoord2f(l_U, l_V);
	}

}

