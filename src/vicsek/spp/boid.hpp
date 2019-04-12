/***********************************************************************************\
* Boid:      Class of a single boid, which is the unit of the SPP model. All        *
*            properties of the particles and corresponding methods are encapsulated *
*            in this class.                                                         *
*                                                                                   *
* Author:    Enys Mones                                                             *
* Date:      10.08.2014                                                             *
\***********************************************************************************/


#ifndef BOID_HPP
#define BOID_HPP


#include "stdlib.h"
#include "math.h"
#include <vector>
#include "vector2d.hpp"
#include "random_generator.hpp"
#include "GL/glut.h"


class Boid
{
private:
    Vector2d _position;
    Vector2d _velocity;
    Vector2d _tmpVelocity;
    std::vector<Boid*> _neighbors;

public:
    Boid( Vector2d pos_, Vector2d vel_ );

    // Getters
    const Vector2d position() const;
    const Vector2d velocity() const;

    // Operations
    int get_grid_index( const int gridWidth_, const double siteSize_ ) const;
    bool in_range( const Boid* boid_, const double roi_ ) const;
    void set_neighbors( std::vector<Boid*> neighbors_ );
    void update_tmp_velocity_svm( double noise_ );
    void update_velocity( const double drift_ );
    void update_position( const double dt_, double systemSize_ );
};

#endif // BOID_HPP
