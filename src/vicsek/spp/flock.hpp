/***********************************************************************************\
* Flock:     The flock class that contains the vector of boids and handles all      *
*            corresponding operations (initialization, dynamics, measurement, etc). *
*            No visualization is implemented, this class is responsible merely for  *
*            the simulation.                                                        *
*            Note that in order to use OpenGL function with much more ease, this    *
*            class is designed to be a singleton.                                   *
*                                                                                   *
* Author:    Enys Mones                                                             *
* Date:      10.08.2014                                                             *
\***********************************************************************************/


#ifndef FLOCK_HPP
#define FLOCK_HPP


#include "stdio.h"
#include "time.h"
#include "math.h"
#include <vector>
#include "random_generator.hpp"
#include "boid.hpp"


class Flock
{
private:
    // Boids
    std::vector<Boid*> _boids;
    std::vector< std::vector<Boid*> > _sites;

    // Random generator
    RandomGenerator _ran;

    // Adjustable parameters
    int _gridWidth;
    double _noise;

    // Constant parameters
    const static double _DT = 1.0;
    const static double _ROI = 1.0;
    const static double _SITE_SIZE = 1.0;
    const static double _DRIFT = 0.03;

    // Default parameter values
    const static int _DEFAULT_GRID_WIDTH = 10;
    const static int _DEFAULT_NUMBER_OF_BOIDS = 100;
    const static double _DEFAULT_NOISE = 1.0;

    // Make Flock a singleton
    Flock();
    Flock( Flock const& );
    void operator= ( Flock const& );

public:
    ~Flock();

    // Get instance
    static Flock& instance();

    // Queries
    int grid_width() const;
    int number_of_boids() const;
    double noise() const;
    double system_size() const;
    const Vector2d &position( int index_ ) const;
    const Vector2d &velocity( int index_ ) const;

    // Allocation, destruction
    void init( int gridWidth_, int numberOfBoids_, double noise_ );
    void destroy();

    // Dynamics
    void update_sites();
    void update_neighbors();
    void update_dynamics();
    void iterate();

    // Measurement
    double order_parameter() const;
};

#endif // FLOCK_HPP
