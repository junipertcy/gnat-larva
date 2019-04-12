#include "boid.hpp"


/**
 * Constructor with position and velocity.
 *
 * @param pos_   initial position
 * @param vel_   initial velocity
 */
Boid::Boid( Vector2d pos_, Vector2d vel_ )
{
    _position = pos_;
    _velocity = vel_;
}


/**
 * Getter for the position.
 *
 * @return  a constant position vector
 */
const Vector2d Boid::position() const
{
    return _position;
}


/**
 * Getter for the velocity.
 *
 * @return  a constant velocity vector
 */
const Vector2d Boid::velocity() const
{
    return _velocity;
}


/**
 * Determines the boid's grid index on a rectangle-shape grid.
 * Grid index is calculated by the following:
 *   id = floor(x/L) + floor(y/L)*N
 * where N is the number of sites in the horizontal direction, L is the linear length of a single
 * square-shaped site.
 *
 * @param gridWidth_  number of sites on the grid in the horizontal direction
 * @param siteSize_   linear length of a single site
 * @return            index of the site in the grid
 */
int Boid::get_grid_index( const int gridWidth_, const double siteSize_ ) const
{
    return int( _position.x() / siteSize_ )
            + gridWidth_ * int( _position.y() / siteSize_ );
}


/**
 * Checks if boid is in range, i.e., it is inside the range of interaction of this boid.
 *
 * @param boid_  boid to check for distance
 * @param roi_   range of interaction
 * @return       true if boid is in range, false otherwise
 */
bool Boid::in_range( const Boid *boid_, const double roi_ ) const
{
    Vector2d dist = _position - boid_->_position;
    if( dist.length() < roi_ )
        return true;
    else
        return false;
}


/**
 * Sets neighbor set of the boid.
 *
 * @param neighbors_  new neighbor set
 */
void Boid::set_neighbors( std::vector<Boid*> neighbors_ )
{
    _neighbors = neighbors_;
}


/**
 * Updates temporal velocity of the boid according to the standard Vicsek model.
 *
 * @param noise_  magnitude of the noise (eta)
 */
void Boid::update_tmp_velocity_svm( double noise_ )
{
    int n = (int)_neighbors.size();

    // Calculate average velocity
    _tmpVelocity = _velocity;
    for( int i=0; i<n; i++ )
        _tmpVelocity += _neighbors[i]->_velocity;
    _tmpVelocity /= (double)(n+1);

    // Add noise
    double angle = _tmpVelocity.angle() + noise_;
    _tmpVelocity.set_angle( angle );
}


/**
 * Updates velocity by copying the temporal velocity.
 * After update, velocity is normalized to the drift velocity.
 *
 * @param drift_  drift magnitude of the boid
 */
void Boid::update_velocity( double drift_ )
{
    _velocity = _tmpVelocity;
    _velocity.set_length( drift_ );
}


/**
 * Updates position according to the velocity.
 * Position is updated by:
 *   pos = pos + vel * dt
 * where dt is the temporal difference of one iteration step.
 * After position is updated, it is checked against system size and updated according to
 * periodic boundary conditions.
 *
 * @param dt_  temporal difference

 */
void Boid::update_position( const double dt_, double systemSize_ )
{
    _position += _velocity * dt_;
    double corrX = 0.0, corrY = 0.0;

    if( _position.x() < 0.0 )
        corrX = systemSize_;
    if( _position.x() > systemSize_ )
        corrX = -systemSize_;
    if( _position.y() < 0.0 )
        corrY = systemSize_;
    if( _position.y() > systemSize_ )
        corrY = -systemSize_;
    Vector2d corr( corrX, corrY );

    _position += corr;
}


