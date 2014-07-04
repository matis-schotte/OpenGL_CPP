# include "../../../include/flappy_box/view/paddle_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b )
: _model( b )
{
    std::string err = tinyobj::LoadObj(shapes, "cube.obj", NULL);
    
    if(!err.empty())
        std::cout << std::endl << "***ERROR: " << err << std::endl;
    else
        for (size_t i = 0; i < shapes.size(); i++)
            std::cout << "Loaded obj: " << shapes[i].name << std::endl;
}

PaddleGlDrawable::~PaddleGlDrawable()
{
}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    glPushMatrix();
    {
        const vec3_type& pos = _model->position();
        glTranslated( pos(0), pos(1), pos(2) );
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        glColor3d( .8, .4, .4 );
        glScaled(1., 1., 10.);
        glRotated(45., 0., 0., 1.);
        
        ::glBegin(GL_TRIANGLES); // GL_LINES GL_TRIANGLES
        
        for(size_t i = 0; i < shapes.size(); i++)
            for(size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
            {
                if((f%3) == 0) // berechnen der normalen
                {
                    vec3_type a(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2]), b(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+1]+2]), c(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f+2]+2]), d(0,0,0), e(0,0,0), final(0,0,0);
                    d = a-b; e = a-c;
                    final = d.cross(e); final.normalize();
                    ::glNormal3d(final(0), final(1), final(2));
                }
                ::glVertex3d(shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+0], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+1], shapes[i].mesh.positions[3*shapes[i].mesh.indices[f]+2]);
            }
        
        ::glEnd();
        
    }
    glPopMatrix();
}
