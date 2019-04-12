#include "vector2d.hpp"


/**
 * Empty constructor.
 * Creates a unit vector in the direction of the x-axis.
 */
Vector2d::Vector2d()
{
    set( 1.0, 0.0 );
}


/**
 * Copy constructor.
 *
 * @param vec_  vector to copy
 */
Vector2d::Vector2d( const Vector2d &vec_ )
{
    _x = vec_._x;
    _y = vec_._y;
}


/**
 * Complete constructor.
 *
 * @param x_  x-component
 * @param y_  y-component
 */
Vector2d::Vector2d( double x_, double y_ )
{
    set( x_, y_ );
}


/**
 * Componentwise setter.
 *
 * @param x_  x-component
 * @param y_  y-component
 */
void Vector2d::set( const double x_, const double y_ )
{
    _x = x_;
    _y = y_;
}


/**
 * Sets length of the vector without changing its angle.
 *
 * @param length_  new length of the vector
 */
void Vector2d::set_length( double length_ )
{
    double l = length();
    if( l > _EPSILON )
        *this *= length_ / l;
}


/**
 * Sets angle of the vector without changing its length.
 *
 * @param angle_  new angle of the vector in radian
 */
void Vector2d::set_angle( double angle_ )
{
    double l = length();
    _x = l * cos( angle_ );
    _y = l * sin( angle_ );
}


/**
 * Normalizes vector length to unit.
 */
void Vector2d::normalize()
{
    set_length( 1.0 );
}


/**
 * Getter for the x-component.
 *
 * @return  x-component
 */
double Vector2d::x() const
{
    return _x;
}


/**
 * Getter for the y-component.
 *
 * @return  y-component
 */
double Vector2d::y() const
{
    return _y;
}


/**
 * Calculates length of the vector.
 *
 * @return  length of the vector
 */
double Vector2d::length() const
{
    return sqrt( _x*_x + _y*_y );
}


/**
 * Calculates angle of the vector.
 *
 * @return  angle of the vector in radian
 */
double Vector2d::angle() const
{
    return atan2( _y, _x );
}


/**
 * Assignment operator.
 *
 * @param vec_  vector to copy
 * @return      reference to the assigned vector
 */
Vector2d& Vector2d::operator =( const Vector2d &vec_ )
{
    this->_x = vec_._x;
    this->_y = vec_._y;
    return *this;
}


/**
 * Increment operator.
 *
 * @param vec_  vector of the incrementation
 * @return      reference to the incremented vector
 */
Vector2d& Vector2d::operator +=( const Vector2d &vec_ )
{
    _x += vec_._x;
    _y += vec_._y;
    return *this;
}


/**
 * Addition operator.
 *
 * @param vec_  vector to add
 * @return      sum of the two vectors
 */
const Vector2d Vector2d::operator +( const Vector2d &vec_ ) const
{
    return Vector2d(*this) += vec_;
}


/**
 * Decrement operator.
 *
 * @param vec_  vector of the decrementation
 * @return      reference to the decremented vector
 */
Vector2d& Vector2d::operator -=( const Vector2d &vec_ )
{
    _x -= vec_._x;
    _y -= vec_._y;
    return *this;
}


/**
 * Subtraction operator.
 *
 * @param vec_  vector to subtract
 * @return      the difference vector
 */
const Vector2d Vector2d::operator -( const Vector2d &vec_ ) const
{
    return Vector2d(*this) -= vec_;
}


/**
 * Multiplication assignment operator.
 *
 * @param alpha_  multiplier
 * @return        reference to the multiplied vector
 */
Vector2d& Vector2d::operator *=( const double &alpha_ )
{
    _x *= alpha_;
    _y *= alpha_;
    return *this;
}


/**
 * Multiplication operator.
 *
 * @param vec_  multiplier
 * @return      multiplied vector
 */
const Vector2d Vector2d::operator *( const double &alpha_ ) const
{
    return Vector2d(*this) *= alpha_;
}


/**
 * Division assignment operator.
 *
 * @param alpha_  divider
 * @return        reference to the divided vector
 */
Vector2d& Vector2d::operator /=( const double &alpha_ )
{
    _x /= alpha_;
    _y /= alpha_;
    return *this;
}


/**
 * Division operator.
 *
 * @param vec_  divider
 * @return      divided vector
 */
const Vector2d Vector2d::operator /( const double &alpha_ ) const
{
    return Vector2d(*this) /= alpha_;
}


/**
 * Shifts x-component by the given value.
 *
 * @param dx_  size of the shift
 */
void Vector2d::shift_x( double dx_ )
{
    _x += dx_;
}


/**
 * Shifts y-component by the given value.
 *
 * @param dx_  size of the shift
 */
void Vector2d::shift_y( double dy_ )
{
    _y += dy_;
}
