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
    glClearColor( 1.f, 1.f, 1.f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt( 0,-80,1, 0,0,0, 0,0,1 );
    
    // Enable standard stuff
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    
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
