# include "../../../include/flappy_box/view/paddle_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b ) : _model( b )
{
	glGenBuffers(3, glBuffers);

	vec3_type size = _model->size();
	r0 = std::max(size(0), size(1)) / 2; // die Hälfte des Maximums aus der Paddle-Breite und der –Tiefe
	r1 = std::max(size(2) / 2, r0); // die Hälfte der Paddle-Höhe, mindestens aber der große Radius

	updateVBOs();
}

PaddleGlDrawable::~PaddleGlDrawable()
{
	glDeleteBuffers(3, glBuffers);
}

void PaddleGlDrawable::updateVBOs() {
	double vertices[3*40*15];
	double normals[3*40*15];
	unsigned int indices[2*3*40*15];

	for (unsigned int i = 0; i < u0; i++) {
		for (unsigned int j = 0; j < u1; j++) {
			double a = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(u0);
			double b = 2.0 * M_PI * static_cast<double>(j) / static_cast<double>(u1);

			unsigned int xIndex = j*3 + i*3*u1;
			unsigned int yIndex = xIndex + 1;
			unsigned int zIndex = xIndex + 2;

			// normals
			normals[xIndex] = cos(b)*cos(a);
			normals[yIndex] = cos(b)*sin(a);
			normals[zIndex] = sin(b);
			
			// vertices
			vertices[xIndex] = r1*normals[xIndex] + r0*cos(a);
			vertices[yIndex] = r1*normals[yIndex] + r0*sin(a);
			vertices[zIndex] = r1*normals[zIndex];
		}
	}

	std::fill<unsigned int[], unsigned int>(indices, std::end(indices), 0u);
	indices[0] = 0;
	indices[1] = 15;
	indices[2] = 1;
	indices[3] = 1;
	indices[4] = 15;
	indices[5] = 16;
	
	glBindBuffer(GL_ARRAY_BUFFER, glBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, glBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, glBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    glPushMatrix();
    {
        const vec3_type& pos = _model->position();
        glTranslated(pos(0), pos(1), pos(2));
		glRotated(0.0, 0.0, 0.0, 0.0);
       
		// lighting
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		float r0 = 0.0f; float r1 = 0.0f;
		GLfloat lPos[4] = { static_cast<float>(pos(0)), static_cast<float>(pos(1)) - (r0*1.5f), static_cast<float>(pos(2)) + (r1*3.0f), 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lPos);
		
		glEnableClientState(GL_VERTEX_ARRAY); 
		glEnableClientState(GL_NORMAL_ARRAY); 
		
		glColor3f(.6f, .9f, .7f); 
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers[1]);
		glNormalPointer(GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, glBuffers[2]);
		glDrawElements(GL_TRIANGLES, u0 * u1 * 6, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
    }
    glPopMatrix();
}
