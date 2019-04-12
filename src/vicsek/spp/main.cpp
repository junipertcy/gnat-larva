/***********************************************************************************\
* Flocking: a simulational framework for the standard Vicsek model.                 *
*                                                                                   *
* The model description can be found in:                                            *
* http://journals.aps.org/prl/abstract/10.1103/PhysRevLett.75.1226                  *
*                                                                                   *
* This program includes a simple implementation of the model that runs in linear    *
* time, and also comes with a visualization and snapshot exporting function.        *
* The latter functionalities are implemented in graphics.cpp and can be used by     *
* including graphics.hpp.                                                           *
* Note that in order to use the visualization functionality, OpenGL is needed to be *
* installed. For more details, see description in graphics.hpp.                     *
*                                                                                   *
*                                                                                   *
* Author:    Enys Mones                                                             *
* Date:      10.08.2014                                                             *
\***********************************************************************************/


// Standard and custom libraries
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "vector2d.hpp"
#include "random_generator.hpp"
#include "boid.hpp"
#include "flock.hpp"

// OpenGL and CImg libraries for real-time visualization and saving images
// FIXME: if no visualization is needed, comment out these two lines!
#include "GL/glut.h"
#include "graphics.hpp"


// Iteration constants
#define LAPS 100
#define MAX_ITERATIONS 1000


/**
 * Main function.
 *
 * @param argc_  number of arguments given to the program
 * @param argv_  list of arguments given to the program
 */
int main( int argc_, char **argv_ )
{
    // Read arguments
    int gridWidth = 20;
    int N = 1000;
    double noise = 0.0;
    bool visualize = false;
    int saveSnapshots = 0;
    for( int i=0; i<argc_; i++ )
    {
        if( strcmp(argv_[i], "--system-size") == 0 )
            sscanf( argv_[i+1], "%i", &gridWidth );
        if( strcmp(argv_[i], "--number-of-boids") == 0 )
            sscanf( argv_[i+1], "%i", &N );
        if( strcmp(argv_[i], "--noise") == 0 )
            sscanf( argv_[i+1], "%lg", &noise );
        if( strcmp(argv_[i], "--visualize") == 0 )
            visualize = true;
        if( strcmp(argv_[i], "--save-snapshots") == 0 )
            saveSnapshots = 1;
        if( strcmp(argv_[i], "--help") == 0 )
        {
            printf( "flock: a standard SPP simulation program based on the 1995 Phys. Rev. Lett."
                    " paper of Vicsek et al.\n" );
            printf( "Usage:\n  flock [options]\nOptions\n" );
            printf( " --help             Brings up help menu.\n" );
            printf( " --system-size      Width of the grid behind the system.\n" );
            printf( " --number-of-boids  Number of boids in flock.\n" );
            printf( " --noise            Magnitude of nise.\n" );
            printf( " --visualize        Turns on visualization.\n" );
            printf( " --save-snapshots   Turns on saving snapshots from visualization.\n" );
            printf( "\n" );
            exit( 0 );
        }
    }

    // Init flock
    Flock::instance().init( gridWidth, N, noise );

    // If visualize is on, show flock.
    // Note that the method make_visualization will exit the program properly.
    // FIXME: if no visualization is needed, comment out these two lines!
    if( visualize )
        make_visualization( &argc_, argv_, saveSnapshots );

    // Otherwise just make a simulation
    // Create output file
    char outputName[32] = { "" };
    sprintf( outputName, "spp_L-%i_N-%i_noise-%lg\n", gridWidth, N, noise );
    FILE* pFile = fopen( outputName, "w" );
    fclose( pFile );
    double param = 0.0;

    // Make measurement
    for( int i=0; i<MAX_ITERATIONS; i++ )
    {
        // Run iterations
        for( int lap=0; lap<LAPS; lap++ )
            Flock::instance().iterate();

        // Measure
        param = Flock::instance().order_parameter();
        printf( "t = %i, v = %lg\n", i, param );
        pFile = fopen( outputName, "a" );
        fprintf( pFile, "%i %lg\n", i, param );
        fclose( pFile );
    }
    return 0;
}

