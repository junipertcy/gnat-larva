/***********************************************************************************\
* graphics:  This header defines all graphics-related functions. This is necessary  *
*            for the visualization and for exporting snapshots for creating video.  *
*            Visualization is implemented in OpenGL and images are saved using the  *
*            CImg.h library. For more informations:                                 *
*            OpenGL:  http://www.opengl.org/                                        *
*            CImg.h:  http://cimg.sourceforge.net/index.shtml                       *
*                                                                                   *
*            In order to compile (with g++, for instance), the following libraries  *
*            must be linked:                                                        *
*            -lglut -lGLU -lGL -lpthread -lX11 -lm                                  *
*                                                                                   *
* Author:    Enys Mones                                                             *
* Date:      10.08.2014                                                             *
\***********************************************************************************/


#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include "stdlib.h"
#include <string>
#include "boid.hpp"
#include "flock.hpp"
#include "CImg.h"
#include "GL/glut.h"


// Visualization constants
#define WINDOW_SIZE 700
#define BOID_WIDTH 0.1
#define BOID_HALF_HEIGHT 0.2
#define BOID_COLOR_RED 0.0
#define BOID_COLOR_GREEN 68.0
#define BOID_COLOR_BLUE 170.0
#define SAVE_IMAGE_DT 5


// Methods
void glut_init( int *argc_, char **argv_ );
void glut_display();
void glut_keyboard( unsigned char key_, int x_, int y_ );
void glut_timer( int var_ );
void draw_boid( const Vector2d &base1_,
                const Vector2d &base2_,
                const Vector2d &top_ );
void glut_draw_flock();
void save_image( const char *fileName_ );
void make_visualization( int *argc_, char **argv_, int saveSnapshots_ );


#endif // GRAPHICS_HPP
