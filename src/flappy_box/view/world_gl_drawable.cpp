# include "../../../include/flappy_box/view/world_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

std::vector<tinyobj::shape_t> WorldGlDrawable::shapes;

WorldGlDrawable::WorldGlDrawable(const std::shared_ptr< ::flappy_box::model::World >& b )
: _model( b )
{
    if(shapes.empty()) // no obj loading needed here
    {
        std::string err = tinyobj::LoadObj(shapes, "world.obj", NULL);
        
        if(!err.empty())
            std::cout << std::endl << "***ERROR: " << err << std::endl;
    }
}

WorldGlDrawable::~WorldGlDrawable()
{
}

void WorldGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    glPushMatrix();
    {
        // render points and remaining lives
        glDisable(GL_LIGHTING);
        char text[32];
        sprintf(text, "Points: %u Remaining Lives: %i", _model->playerPoints(), _model->remainingLives());
        glColor3f(1.0f, .0f, .0f);
        glRasterPos3d(-45., 0., 30.5);
        renderBitmapString(text);
        glEnable(GL_LIGHTING);
        
        //setup for the world
        const vec3_type& pos = _model->position();
        glTranslated( pos(0), pos(1), pos(2) );
        //glTranslated( 0., 200., 2. );
        glScaled( 50., 120., 30. );
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        // FOG
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, getFogColor());
        glFogf(GL_FOG_DENSITY, 1.0f);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, 0.0f);
        glFogf(GL_FOG_END, 500.0f);
        glEnable(GL_FOG);
        
        ::glBegin(GL_TRIANGLES); // GL_LINES GL_TRIANGLES
        
        for(size_t i = 0; i < shapes.size(); i++)
            for(size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
            {
                if((f%3) == 0) // berechnen der farben
                {
                    // b/w color
                    if(bw == 1)
                    {
                        glColor3d( 0., 0., 0. );
                        bw = 0;
                    }
                    else
                    {
                        glColor3d( 1., 1., 1. );
                        bw = 1;
                    }
                }
                
                ::glVertex3d(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0]);
            }
        
        ::glEnd();
        
        glDisable(GL_FOG);
    }
    glPopMatrix();
}

void WorldGlDrawable::renderBitmapString(const char *string)
{
    for(const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}