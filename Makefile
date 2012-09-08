CPPFLAGS = -I. -Iinclude -DX11 # -DHAS_MAGICK
CXXFLAGS = -O3 -g -Wall
LDFLAGS = -lX11 -lGL -lstdc++ # `Magick++-config --libs`
OBJECTS = \
	states/mushroom.o states/intro.o states/space0.o states/base.o \
	misc/profiler.o misc/image.o misc/sound.o misc/common.o misc/state.o \
	misc/font.o \
	geometry/geom.o geometry/sphere.o geometry/aabox.o geometry/vertex_geom.o \
	geometry/camera.o geometry/particle.o \
	raytracer/ray.o raytracer/raytracer.o \
	files/quake3_bsp.o files/tga.o files/png.o files/jpeg.o \
	files/vorbis.o

OBJECTS_DEMO = $(OBJECTS) demo.o
LDFLAGS_DEMO = $(LDFLAGS) \
	-lvorbis \
	-lvorbisfile \
        -lasound \
        -ljpeg \
        -lpng \
        -lz \
        -lX11

all: $(OBJECTS) demo

demo: $(OBJECTS_DEMO)
	@echo Linking demo...
	@$(CXX) $(OBJECTS_DEMO) $(LDFLAGS_DEMO) $(OUTPUT_OPTION)

clean:
	@-rm $(OBJECTS) demo demo.o

.cc.o:
	@echo Compiling $<...
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

demo.o: ./demo.cc \
 include/profiler.h \
 include/geom.h include/common.h include/vector.h \
 include/matrix.h \
 include/state.h \
 include/font.h \
 include/image.h include/particle.h include/color.h files/quake3_bsp.h \
 files/vorbis.h \
 include/sound.h states/mushroom.h \
 states/intro.h states/space0.h states/base.h include/spline.h
files/quake1_bsp.o: ./files/quake1_bsp.cc \
 files/quake1_bsp.h \
 include/geom.h include/common.h include/vector.h \
 include/matrix.h \
 include/image.h files/swap.h
files/png.o: ./files/png.cc \
 include/common.h \
 files/png.h include/image.h
files/tga.o: ./files/tga.cc \
 include/common.h \
 files/swap.h files/tga.h include/image.h
files/jpeg.o: ./files/jpeg.cc \
 include/common.h files/jpeg.h include/image.h
files/quake3_bsp.o: ./files/quake3_bsp.cc \
 include/bezier.h \
 include/profiler.h \
 include/raytracer.h include/common.h include/vector.h \
 include/image.h include/geom.h include/matrix.h \
 include/color.h files/quake3_bsp.h \
 files/swap.h
files/vorbis.o: ./files/vorbis.cc \
 include/common.h files/vorbis.h \
 include/sound.h
geometry/geom.o: ./geometry/geom.cc \
 include/geom.h include/common.h include/vector.h \
 include/matrix.h \
 include/raytracer.h include/image.h \
 include/color.h
geometry/sphere.o: ./geometry/sphere.cc \
 include/vector.h \
 include/geom.h include/common.h \
 include/matrix.h include/raytracer.h include/image.h \
 include/color.h
geometry/vertex_geom.o: ./geometry/vertex_geom.cc \
 include/vector.h \
 include/geom.h include/common.h include/matrix.h \
 include/raytracer.h include/image.h \
 include/color.h
geometry/aabox.o: ./geometry/aabox.cc \
 include/geom.h \
 include/common.h \
 include/vector.h \
 include/matrix.h
geometry/camera.o: ./geometry/camera.cc \
 include/geom.h \
 include/common.h include/vector.h \
 include/matrix.h
geometry/particle.o: ./geometry/particle.cc \
 include/particle.h include/vector.h \
 include/color.h include/common.h \
 include/image.h \
 include/geom.h include/matrix.h \
 include/primitives.h
misc/profiler.o: ./misc/profiler.cc \
 include/profiler.h
misc/image.o: ./misc/image.cc \
 include/image.h \
 include/common.h files/jpeg.h files/tga.h files/png.h
misc/common.o: ./misc/common.cc \
 include/common.h
misc/sound.o: ./misc/sound.cc \
 include/common.h \
 include/sound.h
misc/state.o: ./misc/state.cc \
 include/state.h \
 include/common.h include/sound.h
misc/font.o: ./misc/font.cc \
 include/font.h include/image.h \
 include/common.h \
 include/vector.h \
 include/primitives.h
raytracer/ray.o: ./raytracer/ray.cc \
 include/common.h \
 include/vector.h include/raytracer.h include/image.h \
 include/geom.h include/matrix.h
raytracer/raytracer.o: ./raytracer/raytracer.cc \
 include/raytracer.h include/common.h \
 include/vector.h \
 include/image.h include/geom.h include/matrix.h \
 include/color.h
states/intro.o: ./states/intro.cc \
 include/state.h \
 include/common.h \
 include/matrix.h \
 include/image.h include/font.h include/vector.h states/intro.h \
 include/geom.h
states/mushroom.o: ./states/mushroom.cc \
 include/state.h \
 include/common.h \
 include/matrix.h \
 include/font.h include/image.h include/vector.h states/mushroom.h
states/space0.o: ./states/space0.cc \
 include/vector.h \
 include/state.h \
 include/common.h \
 include/matrix.h include/image.h include/font.h include/primitives.h \
 states/space0.h include/geom.h
states/base.o: ./states/base.cc \
 include/state.h \
 include/common.h \
 include/matrix.h \
 include/image.h include/font.h include/vector.h include/particle.h \
 include/color.h include/raytracer.h include/geom.h states/base.h \
 include/spline.h
