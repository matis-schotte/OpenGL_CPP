# include "../../include/view/gl_renderer.hpp"
# include "../../include/view/glut_window.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
# include "GL/freeglut.h"

using namespace ::view;

GlRenderer::GlRenderer( std::shared_ptr< model::Game const > const& g )
: _game_model( g )
{}

std::shared_ptr< ::model::Game const > const& GlRenderer::game_model() const
{
    return _game_model;
}

GlRenderer::delegate_factory_type& GlRenderer::drawable_factory()
{
    return _drawable_factory;
}

GlRenderer::delegate_factory_type const& GlRenderer::drawable_factory() const
{
    return _drawable_factory;
}

void GlRenderer::visualize_model( GlutWindow& w )
{
#ifndef DEBUG_VISUALIZATION
    glClearColor( 0.f, 0.f, 0.f, 1.0f ); 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#endif
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0,-200,0, 0,0,0, 0,0,1 );
    
    
    // Enable global LIGHTNING
    ::glEnable(GL_CULL_FACE);
    ::glEnable(GL_NORMALIZE);
    ::glEnable(GL_LIGHTING);
    ::glEnable(GL_LIGHT0);
    ::glEnable(GL_COLOR_MATERIAL);
    ::glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    //::glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.9);
    GLfloat lPos[] = {40.0, -100.0, 50.0, .5};
    ::glLightfv(GL_LIGHT0, GL_POSITION, lPos);
    
    
    // render routines for game objects
    for( auto o : game_model()->objects() )
    {
        auto drawable = o->getData< Drawable >();
        if( ! drawable )
        {
            //std::clog << "::view::GlRenderer::visualize_model: Adding new Drawable for \"" << o->name() << "\"." << std::endl;
            drawable = _drawable_factory.create_for( o );
            o->registerData( drawable );
        }
        
        if( drawable ) drawable->visualize( *this, w );
    }
    
    glutSwapBuffers();
}

void GlRenderer::resize( GlutWindow& win )
{
    glViewport( 0,0, win.width(), win.height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity(); //Reset the camera
    gluPerspective( 45., win.width() / double( win.height() ), 1.0, 1000. );
}
