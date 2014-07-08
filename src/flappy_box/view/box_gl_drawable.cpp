# include "../../../include/flappy_box/view/box_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

// all points of the cube
#define POINT_A -1,+1,-1
#define POINT_B +1,+1,-1
#define POINT_C +1,-1,-1
#define POINT_D -1,-1,-1
#define POINT_E -1,+1,+1
#define POINT_F +1,+1,+1
#define POINT_G +1,-1,+1
#define POINT_H -1,-1,+1

// all normals of the cube
#define NORMAL_ABCD 0,0,-1
#define NORMAL_AEFB 0,+1,0
#define NORMAL_ADHE -1,0,0
#define NORMAL_FGCB +1,0,0
#define NORMAL_HGFE 0,0,+1
#define NORMAL_GHDC 0,-1,0

// some constants
#define VERTEX_SIZE 3

BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
: _model( b )
{
    std::string err = tinyobj::LoadObj(shapes, "cube.obj", NULL);
    
    if(!err.empty())
        std::cout << std::endl << "***ERROR: " << err << std::endl;
    //else
    //    for(size_t i = 0; i < shapes.size(); i++)
    //        std::cout << "Loaded obj: " << shapes[i].name << std::endl;
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
        glTranslated( pos(0), pos(1), pos(2) );
        glRotated( angle, 1., 1., 0. );
        glScaled( size, size, size );
		glColor3d(.4, .8, .4);

		const GLfloat vertices[] = {
			POINT_A, POINT_B, POINT_C, POINT_D,
			POINT_A, POINT_E, POINT_F, POINT_B,
			POINT_A, POINT_D, POINT_H, POINT_E,
			POINT_F, POINT_G, POINT_C, POINT_B,
			POINT_H, POINT_G, POINT_F, POINT_E,
			POINT_G, POINT_H, POINT_D, POINT_C
		};

		float normals[] = {
			NORMAL_ABCD,
			NORMAL_AEFB,
			NORMAL_ADHE,
			NORMAL_FGCB,
			NORMAL_HGFE,
			NORMAL_GHDC
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		
		glVertexPointer(VERTEX_SIZE, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
		
		glDrawArrays(GL_QUADS, 0, sizeof(vertices) / VERTEX_SIZE);

		glColor3d(0.2, 1.0, 0.2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_QUADS, 0, sizeof(vertices) / VERTEX_SIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*
        // modell besser platzieren
        glRotated( 90., 1., 1., 0. );
        glScaled( .2, .2, .2 );        
        
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
        */

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
        
        //glEnd();
        
    }
    glPopMatrix();
}
