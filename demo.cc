// *** Entry point for ``Norsk Forvaltningslov for Reindrift''

#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include <profiler.h>
#include <geom.h>
#include <state.h>
#include <font.h>
#include <particle.h>
#include <vector.h>
#include <files/quake3_bsp.h>
#include <files/vorbis.h>
#include <states/mushroom.h>
#include <states/intro.h>
#include <states/space0.h>
#include <states/base.h>

int main(int p_ArgumentCount, char** p_Arguments)
try
{
	cls::font::load(
		"data/textures/courier",
		" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~",
		13,
		cls::vector2(1.0 / 13.0, 1.0 / 8.0),
		cls::vector2(0, 0));

	cls::particle::init();

	try
	{
		cls::sound::init();
	}
	catch(std::runtime_error p_RuntimeError)
	{
		std::cerr << "Audio device inaccessible: "
		          << p_RuntimeError.what()
		          << std::endl;
                return EXIT_FAILURE;
	}

        vorbis::sound l_Music("data/music.ogg");

        cls::sound::stream_open(l_Music);

	bsp* l_Labyrinth = new bsp("data/maps/labyrinth.bsp");
	bsp* l_Station = new bsp("data/maps/station2.bsp");

	cls::space0* l_Space0 = new cls::space0("Space 0");
	cls::intro* l_Intro = new cls::intro("Intro", l_Labyrinth);
	cls::base* l_Base = new cls::base("Base", l_Station);
	cls::mushroom* l_Mushroom = new cls::mushroom("Mushroom");

	cls::state::init();

	std::cerr << "Running demo..." << std::endl;
	cls::state::enter(l_Space0, NULL, 0);
	cls::state::enter(l_Intro, NULL, 34390 - BEAT / 2);
	cls::state::enter(l_Base, NULL, 34390 - BEAT / 2 + 32 * BEAT);
	cls::state::enter(l_Mushroom,  NULL, 34390 - BEAT / 2 + 32 * BEAT + 420 * BEAT);

	cls::sound::stream_close();

	cls::profiler::print_stats();

	return EXIT_SUCCESS;
}
catch(std::logic_error p_LogicError)
{
	std::cerr << "Logic error: " << p_LogicError.what() << std::endl;
}
catch(std::runtime_error p_RuntimeError)
{
	std::cerr << "Runtime error: " << p_RuntimeError.what() << std::endl;
}
catch(std::exception p_Exception)
{
	std::cerr << "Exception: " << p_Exception.what() << std::endl;
}
catch(...)
{
	std::cerr << "Unhandled exception" << std::endl;

	return EXIT_FAILURE;
}

