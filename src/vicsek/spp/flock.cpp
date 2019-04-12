#include "flock.hpp"


/**
 * Empty constructor.
 * Sets default parameter values.
 */
Flock::Flock()
{
    _noise = _DEFAULT_NOISE;
}


/**
 * Destructor.
 * Siply calls destroy.
 */
Flock::~Flock()
{
    destroy();
}


/**
 * Returns an instance to the singleton Flock class.
 *
 * @return  static instance of the class
 */
Flock& Flock::instance()
{
    static Flock instance;
    return instance;
}


/**
 * Returns width of the underlying grid.
 *
 * @return  width of the grid
 */
int Flock::grid_width() const
{
    return _gridWidth;
}


/**
 * Returns number of boids in flock.
 *
 * @return  number of boids
 */
int Flock::number_of_boids() const
{
    return (int)_boids.size();
}


/**
 * Returns magnitude of noise.
 *
 * @return  magnitude of noise
 */
double Flock::noise() const
{
    return _noise;
}


/**
 * Returns linear size of the system.
 *
 * @return  linear size of the system
 */
double Flock::system_size() const
{
    return _gridWidth * _SITE_SIZE;
}


/**
 * Returns the position of a given boid.
 *
 * @param index_  index of the boid to query
 * @return        position
 */
const Vector2d &Flock::position( int index_ ) const
{
    return _boids[index_]->position();
}


/**
 * Returns the velocity of a given boid.
 *
 * @param index_  index of the boid to query
 * @return        velocity
 */
const Vector2d &Flock::velocity( int index_ ) const
{
    return _boids[index_]->velocity();
}


/**
 * Initializes flock by setting all adjustable parameters and allocating vectors.
 * After setting and checking the values of the parameters, the method allocates boids,
 * and sets their initial positions and velocities.
 *
 * @param boidsNumber_  number of boids
 * @param noise_        noise magnitude
 */
void Flock::init( int gridWidth_, int numberOfBoids_, double noise_ )
{
    printf( "\n" );
    // Set grid width
    _gridWidth = gridWidth_;
    if( _gridWidth < 10 )
    {
        printf( "init: _gridWidth is too small, set to %i\n", _DEFAULT_GRID_WIDTH );
        _gridWidth = _DEFAULT_GRID_WIDTH;
    }

    // Set number of boids
    int N = numberOfBoids_;
    if( N < 1 )
    {
        printf( "init: numberOfBoids_ is non-positive, set to %i\n", _DEFAULT_NUMBER_OF_BOIDS );
        N = _DEFAULT_NUMBER_OF_BOIDS;
    }

    // Set noise
    _noise = noise_;
    if( _noise < 0.0 )
    {
        printf( "init: _noise is negative, set to %lg\n", _DEFAULT_NOISE );
        _noise = _DEFAULT_NOISE;
    }

    // Set random generator seed
    srand( time(NULL) );
    _ran.init( rand()%100000 );

    // Allocate flock
    Vector2d pos, vel;
    for( int i=0; i<N; i++ )
    {
        // Set random initial values
        pos.set( _gridWidth*_SITE_SIZE*_ran.random(),
                 _gridWidth*_SITE_SIZE*_ran.random() );
        vel.set_length( _DRIFT );
        vel.set_angle( 2.0*M_PI*_ran.random() );

        // Allocate new boid on heap and add to the flock
        _boids.push_back( new Boid( pos, vel ) );
    }
    printf( "init: flock is generated with %i boids\n", number_of_boids() );

    // Allocate sites
    _sites.assign( _gridWidth * _gridWidth, std::vector<Boid*>() );
    printf( "init: system of %ix%i sites is generated\n", _gridWidth, _gridWidth );
}


/**
 * Frees allocated memory and destroys flock.
 */
void Flock::destroy()
{
    int N = number_of_boids();
    if( N > 0 )
    {
        for( int i=0; i<N; i++ )
        {
            delete _boids.back();
            _boids.pop_back();
        }
    }
}


/**
 * Updates boid indices in sites.
 * These indices are further used to update neighbor lists of the boids.
 * We use an underlying grid in our system in order to boost running time. By applying the grid,
 * and checking surrounding boids of a single boid only in the neighboring sites, simulation can
 * run in linear time.
 * This method implies the following steps:
 * 1) Determine site index for all boids ( O(N) )
 * 2) Go through all sites and run calculations for all boids ( O(N) )
 * 3) For each boid, check boids in the surrounding sites ( ~O(N * const) )
 */
void Flock::update_sites()
{
    int N = number_of_boids();
    int sitesNum = _gridWidth * _gridWidth;
    int gridId = 0;
    std::vector<int> gridIndices( N, 0 );
    std::vector<int> boidsInSite( sitesNum, 0 );

    // Get grid indices
    for( int i=0; i<N; i++ )
        gridIndices[i] = _boids[i]->get_grid_index( _gridWidth, _SITE_SIZE );

    // Determine number of boids in sites
    for( int i=0; i<N; i++ )
        boidsInSite[ gridIndices[i] ]++;

    // Allocate sites
    for( int s=0; s<sitesNum; s++ )
    {
        _sites[s].resize( boidsInSite[s] );
        boidsInSite[s] = 0;
    }

    // Accumulate pointers
    for( int i=0; i<N; i++ )
    {
        gridId = gridIndices[i];
        _sites[gridId][ boidsInSite[gridId] ] = _boids[i];
        boidsInSite[gridId]++;
    }
}


/**
 * Updates neighbor lists for the boids.
 * These lists are further used to update the dynamics of the flock.
 */
void Flock::update_neighbors()
{
    // Shorthand notations
    int L = _gridWidth, L2 = _gridWidth * _gridWidth;
    int s2;
    int size1, size2;
    Boid *b1, *b2;
    std::vector<Boid*> neighbors;

    // Go through sites
    for( int s=0; s<L2; s++ )
    {
        // Go through boids in site
        size1 = (int)_sites[s].size();
        for( int i1=0; i1<size1; i1++ )
        {
            // Get current boid and neighbor list
            b1 = _sites[s][i1];
            neighbors.clear();

            // This site
            for( int i2=0; i2<size1; i2++ )
            {
                b2 = _sites[s][i2];
                if( b1 != b2 && b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Left site
            s2 = (s+L-1)%L + (s/L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Right site
            s2 = (s+L+1)%L + (s/L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Lower site
            s2 = (s+L2-L)%L2;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Upper site
            s2 = (s+L2+L)%L2;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Lower left site
            s2 = (s+L-1)%L + ((s/L+L-1)%L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Lower right site
            s2 = (s+1)%L + ((s/L+L-1)%L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Upper left site
            s2 = (s+L-1)%L + ((s/L+1)%L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }
            // Upper right site
            s2 = (s+1)%L + ((s/L+1)%L)*L;
            size2 = (int)_sites[s2].size();
            for( int i2=0; i2<size2; i2++ )
            {
                b2 = _sites[s2][i2];
                if( b1->in_range(b2, _ROI) )
                    neighbors.push_back( b2 );
            }

            // Set neighbor list
            b1->set_neighbors( neighbors );
        }
    }
}


/**
 * Updates dynamics of the flock.
 */
void Flock::update_dynamics()
{
    int N = number_of_boids();
    double systemSize = _gridWidth * _SITE_SIZE;

    // Update temporal velocities
    for( int i=0; i<N; i++ )
        _boids[i]->update_tmp_velocity_svm( _noise * (_ran.random()-0.5) );

    // Update velocities
    for( int i=0; i<N; i++ )
        _boids[i]->update_velocity( _DRIFT );

    // Update positions
    for( int i=0; i<N; i++ )
        _boids[i]->update_position( _DT, systemSize );
}


/**
 * Performs a single iteration.
 * An iteration consists of updating site indices, neighbor lists and dynamics.
 */
void Flock::iterate()
{
    update_sites();
    update_neighbors();
    update_dynamics();
}


/**
 * Calculates the order parameter of the model.
 *
 * @return  order parameter of the standard SPP model
 */
double Flock::order_parameter() const
{
    int N = number_of_boids();
    Vector2d v( 0.0, 0.0 );

    for( int i=0; i<N; i++ )
        v += _boids[i]->velocity();

    return v.length() / (_DRIFT * N);
}

