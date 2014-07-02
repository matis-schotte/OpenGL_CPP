# include "../../../include/flappy_box/view/box_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
: _model( b )
{
    std::string err = tinyobj::LoadObj(shapes, "cppcube.obj", NULL);
    
    if(!err.empty())
        std::cout << std::endl << "***ERROR: " << err << std::endl;
    else
        for (size_t i = 0; i < shapes.size(); i++)
            std::cout << "Loaded obj: " << shapes[i].name << std::endl;
}

BoxGlDrawable::~BoxGlDrawable()
{
}

void BoxGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    // TODO: Replace old rendering code with new and improved rendering - Aufgabe 5.3
    
    glPushMatrix();
    {
        double angle = _model->angle();
        const vec3_type& pos = _model->position();
        const double size = _model->size();
        glTranslated( pos[0], pos[1], pos[2] );
        glRotated( angle, 0., 1., 0. );
        glScaled( size, size, size );
        
        glColor3d( 0.25, 0.25, 0.25 );
        
        // modell richtig drehen
        glRotated( 100., 1., 0., 0. );
        glRotated( -20, 0., 1., 0. );
        glRotated( 22.5, 0., 0., 1. );
        
        ::glBegin(GL_LINES);
        
        for (size_t i = 0; i < shapes.size(); i++)
            for(size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
                ::glVertex3d(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2]);
        
        /*
        ::glVertex3d(-0.5, -0.5, -0.5);
        ::glVertex3d(-0.5, -0.5,  0.5);
        ::glVertex3d(-0.5,  0.5, -0.5);
        ::glVertex3d(-0.5,  0.5,  0.5);
        ::glVertex3d( 0.5, -0.5, -0.5);
        ::glVertex3d( 0.5, -0.5,  0.5);
        ::glVertex3d( 0.5,  0.5, -0.5);
        ::glVertex3d( 0.5,  0.5,  0.5);
        
        ::glVertex3d(-0.5, -0.5, -0.5);
        ::glVertex3d(-0.5,  0.5, -0.5);
        ::glVertex3d( 0.5, -0.5, -0.5);
        ::glVertex3d( 0.5,  0.5, -0.5);
        ::glVertex3d(-0.5, -0.5,  0.5);
        ::glVertex3d(-0.5,  0.5,  0.5);
        ::glVertex3d( 0.5, -0.5,  0.5);
        ::glVertex3d( 0.5,  0.5,  0.5);
        
        ::glVertex3d(-0.5, -0.5, -0.5);
        ::glVertex3d( 0.5, -0.5, -0.5);
        ::glVertex3d(-0.5, -0.5,  0.5);
        ::glVertex3d( 0.5, -0.5,  0.5);
        ::glVertex3d(-0.5,  0.5, -0.5);
        ::glVertex3d( 0.5,  0.5, -0.5);
        ::glVertex3d(-0.5,  0.5,  0.5);
        ::glVertex3d( 0.5,  0.5,  0.5);
         */
        
        ::glEnd();
        
    }
    glPopMatrix();
}
