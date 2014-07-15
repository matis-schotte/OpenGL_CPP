# include "../../../include/flappy_box/view/paddle_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b ) : _model( b )
{
	glGenBuffers(3, torus_buffers);
	glGenBuffers(3, rotor_buffers);

	vec3_type size = _model->size();
	r0 = std::max(size(0), size(1)) / 2; // die Hälfte des Maximums aus der Paddle-Breite und der –Tiefe
	r1 = std::min(size(2) / 2, r0); // die Hälfte der Paddle-Höhe, mindestens aber der große Radius

	updateVBOs();
}

PaddleGlDrawable::~PaddleGlDrawable()
{
	glDeleteBuffers(3, torus_buffers);
	glDeleteBuffers(3, rotor_buffers);
}

void PaddleGlDrawable::updateVBOs() {
	// torus
	double torus_vertices[3*40*15] = {0};
	double torus_normals[3*40*15] = {0};
	unsigned int torus_indices[2*3*40*15] = {0};

	for (unsigned int i = 0; i < u0; i++) {
		for (unsigned int j = 0; j < u1; j++) {
			double a = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(u0);
			double b = 2.0 * M_PI * static_cast<double>(j) / static_cast<double>(u1);

			unsigned int xIndex = j*3 + i*3*u1;
			unsigned int yIndex = xIndex + 1;
			unsigned int zIndex = xIndex + 2;

			// normals
			torus_normals[xIndex] = cos(b)*cos(a);
			torus_normals[yIndex] = cos(b)*sin(a);
			torus_normals[zIndex] = sin(b);

			// vertices
			torus_vertices[xIndex] = r1*torus_normals[xIndex] + r0*cos(a);
			torus_vertices[yIndex] = r1*torus_normals[yIndex] + r0*sin(a);
			torus_vertices[zIndex] = r1*torus_normals[zIndex];
		}
	}

	//std::fill<unsigned int[], unsigned int>(torus_indices, std::end(torus_indices), 0u);
	
	// indices
	for (unsigned int i = 0; i < u0; i++) {
		for (unsigned int j = 0; j < u1; j++) {
			unsigned int index = j * 6 + i * 6 * u1;

			if (i != u0 - 1) {
				if (j != u1 - 1) {
					torus_indices[index + 0] = (i * u1) + j;
					torus_indices[index + 1] = ((i + 1) * u1) + j;
					torus_indices[index + 2] = (i * u1) + j + 1;
					torus_indices[index + 3] = (i * u1) + j + 1;
					torus_indices[index + 4] = ((i + 1) * u1) + j;
					torus_indices[index + 5] = ((i + 1) * u1) + (j + 1);
				}
				else {
					torus_indices[index + 0] = (i * u1) + j;
					torus_indices[index + 1] = ((i + 1) * u1) + j;
					torus_indices[index + 2] = i * u1;
					torus_indices[index + 3] = i * u1;
					torus_indices[index + 4] = ((i + 1) * u1) + j;
					torus_indices[index + 5] = (i + 1) * u1;
				}
			}
			else {
				if (j != u1 - 1) {
					torus_indices[index + 0] = (i * u1) + j;
					torus_indices[index + 1] = j;
					torus_indices[index + 2] = (i * u1) + j + 1;
					torus_indices[index + 3] = (i * u1) + j + 1;
					torus_indices[index + 4] = j;
					torus_indices[index + 5] = j + 1;
				}
				else {
					torus_indices[index + 0] = (i * u1) + j;
					torus_indices[index + 1] = j;
					torus_indices[index + 2] = i * u1;
					torus_indices[index + 3] = i * u1;
					torus_indices[index + 4] = j;
					torus_indices[index + 5] = 0;
				}
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*1800, torus_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*1800, torus_normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, torus_buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*3600, torus_indices, GL_STATIC_DRAW);

	// rotor
	double rotor_vertices[3*3*9] = {0};
	double rotor_normals[3*3*9] = {0};
	unsigned int rotor_indices[2*3*9] = {0};

	for (unsigned int i = 0; i < blade_count; i ++) {
		const double a1 = 2 * M_PI * ((static_cast<double>(i) + 0.3) / static_cast<double>(blade_count));
		const double a2 = 2 * M_PI * ((static_cast<double>(i) - 0.3) / static_cast<double>(blade_count));

		vec3_type v0 = vec3_type(0, 0, 0);
		vec3_type v1 = vec3_type((r0 - r1) * cos(a1), (r0 - r1) * sin(a1), +r1);
		vec3_type v2 = vec3_type((r0 - r1) * cos(a2), (r0 - r1) * sin(a2), -r1);

		rotor_vertices[i*blade_count + 0] = v0(0);
		rotor_vertices[i*blade_count + 1] = v0(1);
		rotor_vertices[i*blade_count + 2] = v0(2);

		rotor_vertices[i*blade_count + 3] = v1(0);
		rotor_vertices[i*blade_count + 4] = v1(1);
		rotor_vertices[i*blade_count + 5] = v1(2);

		rotor_vertices[i*blade_count + 6] = v2(0);
		rotor_vertices[i*blade_count + 7] = v2(1);
		rotor_vertices[i*blade_count + 8] = v2(2);

		vec3_type n = (v0 - v2).cross(v0 - v1);
		//n(0) = abs(n(0)); // WARUM ABSOLUT? damit werden ja die vorzeichen gelšscht ! ;)
		//n(1) = abs(n(1));
		//n(2) = abs(n(2));

		rotor_normals[i*blade_count + 0] = n(0);
		rotor_normals[i*blade_count + 1] = n(1);
		rotor_normals[i*blade_count + 2] = n(2);

		rotor_normals[i*blade_count + 3] = n(0);
		rotor_normals[i*blade_count + 4] = n(1);
		rotor_normals[i*blade_count + 5] = n(2);

		rotor_normals[i*blade_count + 6] = n(0);
		rotor_normals[i*blade_count + 7] = n(1);
		rotor_normals[i*blade_count + 8] = n(2);
	}
	
	//std::fill<unsigned int[], unsigned int>(rotor_indices, std::end(rotor_indices), 0u);

	for (unsigned int i = 0; i < 3*9; i++) {
		rotor_indices[i] = i; 
		rotor_indices[i + 3*9] = 3*9 - 1 - i;
	}

	glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*81, rotor_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*81, rotor_normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, rotor_buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*2*3*9, rotor_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);
	//TODO delete CPU storage
}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    glPushMatrix();
    {
        const vec3_type& pos = _model->position();
        const vec3_type& size = _model->size();
        glTranslated(pos(0), pos(1), pos(2));
        
        // Enable LIGHTING
        glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		GLfloat lPos[4] = { 
			static_cast<float>(pos(0) - size(0)/2.),
			static_cast<float>(pos(1) - (r0*1.5) - 10.),
			static_cast<float>(pos(2) + (r1*3.0) + 100.),
			1.0f 
		};
		glLightfv(GL_LIGHT0, GL_POSITION, lPos);
        
        glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
        
        // draw rotor
		glColor3f(.4f, .8f, .4f);
		glRotated(_model->bladesAngle(), 0.0, 0.0, 1.0);
        
		glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[0]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[1]);
		glNormalPointer(GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, rotor_buffers[2]);
		glDrawElements(GL_TRIANGLES, 2*3*9, GL_UNSIGNED_INT, NULL);
        
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);
        
		// draw torus
        glColor3f(.6f, .9f, .9f);
		glRotated(0.0, 0.0, 0.0, 0.0);
        //glScaled(1.8, 1.8, 1.8);
        
		glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[0]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[1]);
		glNormalPointer(GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, torus_buffers[2]);
		glDrawElements(GL_TRIANGLES, u0*u1*6, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);
        
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
    }
    glPopMatrix();
}
