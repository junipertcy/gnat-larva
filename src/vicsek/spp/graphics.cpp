#include "graphics.hpp"


/**
 * Initializes OpenGL.
 *
 * @param argc_   number of arguments given to the program
 * @param argv_   list of arguments given to the program
 * @param flock_  Flock object of the simulation
 */
void glut_init( int *argc_, char **argv_ )
{
    // Initialize OpenGL window
    glutInit( argc_, argv_ );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( WINDOW_SIZE, WINDOW_SIZE );
    glutInitWindowPosition( 0, 0 );
    glutCreateWindow( "SPP simulation" );

    // Set clearing color and matrices
    glClearColor( 01.0, 1.0, 1.0, 0.0 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, Flock::instance().system_size(), 0.0, Flock::instance().system_size() );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}


/**
 * Display function for the OpenGL visualization.
 */
void glut_display()
{
    // Clear color buffers and render flock
    glClear( GL_COLOR_BUFFER_BIT );
    glut_draw_flock();

    // Swap buffers and force rendering
    glutSwapBuffers();
    glFlush();
}


/**
 * Keyboard function for OpenGL.
 *
 * @param key_  key pressed
 * @param x_    mouse x-coordinate
 * @param y_    mouse y-coordinate
 */
void glut_keyboard( unsigned char key_, int x_, int y_ )
{
    switch( key_ )
    {
    // If 'ESC', just exit
    case 27:
    {
        Flock::instance().destroy();
        exit(0);
        break;
    }
    }
}


/**
 * Timer function for OpenGL.
 * The mandatory argument var_ is used to decode whether we want to save snapshots or not.
 *
 * @param var_  variable passed to the timer function
 */
void glut_timer( int var_ )
{
    // Iterate some
    for( int i=0; i<1; i++ )
        Flock::instance().iterate();

    // Display flock
    glut_display();

    // Save image if var is 1
    static int t = 0;
    t = (t + 1) % 100;
    if( t%SAVE_IMAGE_DT == 0 && var_ == 1 )
    {
        static int imgTime = 1;
        char imgName[128] = { "" };
        sprintf( imgName, "spp_L-%i_N-%i_noise-%lg/img_%i%i%i%i.jpg",
                 Flock::instance().grid_width(),
                 Flock::instance().number_of_boids(),
                 Flock::instance().noise(),
                 imgTime/1000, (imgTime%1000)/100, (imgTime%100)/10, imgTime%10 );

        printf( "saving image %i\n", imgTime );
        save_image( imgName );

        imgTime++;
    }

    // Call timer again
    glutTimerFunc( 10, glut_timer, var_ );
}


/**
 * Draws a single boid.
 * Each boid is drawn as a triangle-shaped object pointing towards its velocity.
 */
void draw_boid( const Vector2d &base1_,
                const Vector2d &base2_,
                const Vector2d &top_ )
{
    // Draw body
    glBegin( GL_POLYGON );
    glVertex2d( top_.x(), top_.y() );
    glVertex2d( base1_.x(), base1_.y() );
    glVertex2d( base2_.x(), base2_.y() );
    glEnd();
}


/**
 * Draws the flock, i.e., all boids.
 * If a boid is on the boundary of the system, a mirror boid is also drawn (periodic boundaries).
 */
void glut_draw_flock()
{
    // Get instance
    Flock *flock = &Flock::instance();

    // Get properties
    int N = flock->number_of_boids();
    double systemSize = flock->system_size();

    // Necessary variables
    double minCrit = BOID_HALF_HEIGHT*1.2;
    double maxCrit = systemSize - BOID_HALF_HEIGHT*1.2;
    Vector2d pos, vn, n, base1, base2, top;

    // Set polygon mode and color
    glColor3f( BOID_COLOR_RED/255., BOID_COLOR_GREEN/255., BOID_COLOR_BLUE/255. );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Draw boids
    for( int i=0; i<N; i++ )
    {
        // Get vertices
        pos = flock->position(i);
        vn = flock->velocity(i);
        vn.set_length( BOID_HALF_HEIGHT );
        n.set( vn.y(), -vn.x() );
        n.set_length( BOID_WIDTH );
        base1 = pos - vn - n;
        base2 = pos - vn + n;
        top = pos + vn;

        // Draw original boid
        draw_boid( base1, base2, top );

        // Boid is on the left side
        if( pos.x() < minCrit
                && pos.y() > minCrit && pos.y() < maxCrit )
        {
            base1.shift_x( systemSize );
            base2.shift_x( systemSize );
            top.shift_x( systemSize );
            draw_boid( base1, base2, top );
        }
        // On the right side
        if( pos.x() > maxCrit
                && pos.y() > minCrit && pos.y() < maxCrit )
        {
            base1.shift_x( -systemSize );
            base2.shift_x( -systemSize );
            top.shift_x( -systemSize );
            draw_boid( base1, base2, top );
        }
        // On the bottom side
        if( pos.x() > minCrit && pos.x() < maxCrit
                && pos.y() < minCrit )
        {
            base1.shift_y( systemSize );
            base2.shift_y( systemSize );
            top.shift_y( systemSize );
            draw_boid( base1, base2, top );
        }
        // On the top side
        if( pos.x() > minCrit && pos.x() < maxCrit
                && pos.y() > maxCrit )
        {
            base1.shift_y( -systemSize );
            base2.shift_y( -systemSize );
            top.shift_y( -systemSize );
            draw_boid( base1, base2, top );
        }

        // Bottom left corner
        if( pos.x() < minCrit && pos.y() < minCrit )
        {
            base1.shift_x( systemSize );
            base2.shift_x( systemSize );
            top.shift_x( systemSize );
            draw_boid( base1, base2, top );
            base1.shift_y( systemSize );
            base2.shift_y( systemSize );
            top.shift_y( systemSize );
            draw_boid( base1, base2, top );
            base1.shift_x( -systemSize );
            base2.shift_x( -systemSize );
            top.shift_x( -systemSize );
            draw_boid( base1, base2, top );
        }
        // Bottom right corner
        if( pos.x() > maxCrit && pos.y() < minCrit )
        {
            base1.shift_x( -systemSize );
            base2.shift_x( -systemSize );
            top.shift_x( -systemSize );
            draw_boid( base1, base2, top );
            base1.shift_y( systemSize );
            base2.shift_y( systemSize );
            top.shift_y( systemSize );
            draw_boid( base1, base2, top );
            base1.shift_x( systemSize );
            base2.shift_x( systemSize );
            top.shift_x( systemSize );
            draw_boid( base1, base2, top );
        }
        // Top left corner
        if( pos.x() < minCrit && pos.y() > maxCrit )
        {
            base1.shift_x( systemSize );
            base2.shift_x( systemSize );
            top.shift_x( systemSize );
            draw_boid( base1, base2, top );
            base1.shift_y( -systemSize );
            base2.shift_y( -systemSize );
            top.shift_y( -systemSize );
            draw_boid( base1, base2, top );
            base1.shift_x( -systemSize );
            base2.shift_x( -systemSize );
            top.shift_x( -systemSize );
            draw_boid( base1, base2, top );
        }
        // Top right corner
        if( pos.x() > maxCrit && pos.y() > maxCrit )
        {
            base1.shift_x( -systemSize );
            base2.shift_x( -systemSize );
            top.shift_x( -systemSize );
            draw_boid( base1, base2, top );
            base1.shift_y( -systemSize );
            base2.shift_y( -systemSize );
            top.shift_y( -systemSize );
            draw_boid( base1, base2, top );
            base1.shift_x( systemSize );
            base2.shift_x( systemSize );
            top.shift_x( systemSize );
            draw_boid( base1, base2, top );
        }
    }
}


/**
 * Saves the image rendered by OpenGL.
 * The rendered viewport is copied to an array and then to a CImg image object.
 *
 * @param fileName_  name of the output image file
 */
void save_image( const char *fileName_ )
{
    // Get viewport
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );

    // Allocate image bits
    int w = viewport[2];
    int h = viewport[3];
    GLubyte *bits = new GLubyte[ 3 * w * h ];

    // Read pixels from framebuffer
    glFinish();
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
    glPixelStorei( GL_PACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_PACK_SKIP_PIXELS, 0 );
    glReadPixels( 0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, bits );

    // Make CImg image anc copy data
    cimg_library::CImg<unsigned char> image( w, h, 1, 3 );
    for( int x=0; x<w; x++ )
    {
        for( int y=0; y<h; y++ )
        {
            image( x, y, 0, 0 ) = (unsigned char)(bits[3*(x + w*(h-1-y)) + 2]);
            image( x, y, 0, 1 ) = (unsigned char)(bits[3*(x + w*(h-1-y)) + 1]);
            image( x, y, 0, 2 ) = (unsigned char)(bits[3*(x + w*(h-1-y)) + 0]);
        }
    }

    // Save image
    image.save_jpeg( fileName_ );

    // Clean up
    delete[] bits;
}


/**
 * Initializes GLUT and enters main loop.
 *
 * @param argc_           number of arguments taken from the main function
 * @param argv_           list of arguments taken from the main function
 * @param saveSnapshots_  1 if we want snapshots to be exported
 */
void make_visualization( int *argc_, char **argv_, int saveSnapshots_ )
{
    // If want images to save, create directory
    if( saveSnapshots_ == 1 )
    {
        char dirName[128] = { "" };
        sprintf( dirName, "spp_L-%i_N-%i_noise-%lg",
                 Flock::instance().grid_width(),
                 Flock::instance().number_of_boids(),
                 Flock::instance().noise() );
        std::string command = "mkdir " + std::string( dirName );
        system( command.c_str() );
    }

    glut_init( argc_, argv_ );
    glutDisplayFunc( glut_display );
    glutKeyboardFunc( glut_keyboard );
    glutTimerFunc( 100, glut_timer, saveSnapshots_ );

    glutMainLoop();
}
