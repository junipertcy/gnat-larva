 /**********************************************************************************\
 * Vector2d:  a simple 2-dimensional vector class with all the necessary methods    *
 *            for the SPP simulation.                                               *
 *                                                                                  *
 * Author:    Enys Mones                                                            *
 * Date:      10.08.2014                                                            *
 \**********************************************************************************/


#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP


#include "stdlib.h"
#include "math.h"


class Vector2d
{
private:
    // Member variables
    double _x;
    double _y;
    const static double _EPSILON = 1e-4;

public:
    // Constructors
    Vector2d();
    Vector2d( const Vector2d &vec_ );
    Vector2d( double x_, double y_ );

    // Setters
    void set( const double x_, const double y_ );
    void set_length( double length_ );
    void set_angle( double angle_ );
    void normalize();

    // Getters
    double x() const;
    double y() const;
    double length() const;
    double angle() const;

    // Operators
    Vector2d& operator =( const Vector2d &vec_ );
    Vector2d& operator +=( const Vector2d &vec_ );
    const Vector2d operator +( const Vector2d &vec_ ) const;
    Vector2d& operator -=( const Vector2d &vec_ );
    const Vector2d operator -( const Vector2d &vec_ ) const;
    Vector2d& operator *=( const double &alpha_ );
    const Vector2d operator *( const double &alpha_ ) const;
    Vector2d& operator /=( const double &alpha_ );
    const Vector2d operator /( const double &alpha_ ) const;

    // Specific modifiers
    void shift_x( double dx_ );
    void shift_y( double dy_ );
};

#endif // VECTOR2D_HPP
