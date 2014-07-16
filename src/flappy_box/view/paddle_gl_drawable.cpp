# include "../../../include/flappy_box/view/paddle_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b ) : _model( b )
{
	glGenBuffers(3, torus_buffers);
	glGenBuffers(3, rotor_buffers);
	glGenBuffers(4, vortex_buffers);
    
    lastKnownPosition = _model->position();

	double vortex_colors[20 * 2 * 4];
	for (unsigned int i = 0; i < vortex_length; i++) {
		double alpha = 0.25*pow(sin(M_PI*static_cast<double>(i) / static_cast<double>(vortex_length-1)), 2);
		
		vortex_colors[i * 8 + 0] = 0.8;
		vortex_colors[i * 8 + 1] = 1.0;
		vortex_colors[i * 8 + 2] = 0.8;
		vortex_colors[i * 8 + 3] = alpha;

		vortex_colors[i * 8 + 4] = 0.8;
		vortex_colors[i * 8 + 5] = 1.0;
		vortex_colors[i * 8 + 6] = 0.8;
		vortex_colors[i * 8 + 7] = alpha;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 20; j++) {
			vortex_dat[i][j][0] = _model->position();
			vortex_dat[i][j][1] = _model->position();
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)* 160, vortex_colors, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)* 20 * 2 * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)* 20 * 2 * 3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)* 20 * 2 * 3, NULL, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	vec3_type size = _model->size();
	r0 = std::max(size(0), size(1)) / 2; // die Hälfte des Maximums aus der Paddle-Breite und der –Tiefe
	r1 = std::min(size(2) / 2, r0); // die Hälfte der Paddle-Höhe, mindestens aber der große Radius

	updateVBOs();
}

PaddleGlDrawable::~PaddleGlDrawable()
{
	glDeleteBuffers(3, torus_buffers);
	glDeleteBuffers(3, rotor_buffers);
	glDeleteBuffers(4, vortex_buffers);
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
        
        // Create light components
        GLfloat ambientLight[] = { 0.2f, .2f, 0.2f, 1.0f };
        GLfloat diffuseLight[] = { 0.5f, 1.f, 0.5f, 1.0f };
        GLfloat specularLight[] = { 1.f, 1.0f, 1.f, 1.0f };
        
        // Assign created components to GL_LIGHT0
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
        
        glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
		GLfloat lPos[4] = {
			static_cast<float>(pos(0) - size(0) / 2.),
			static_cast<float>(pos(1) - (r0*1.5) - 10.),
			static_cast<float>(pos(2) + (r1*3.0) + 30.),
			1.0f
		};
		glLightfv(GL_LIGHT0, GL_POSITION, lPos);
        
        float specReflection[] = { 1.f, 1.f, 1.f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
        float em[] = { .0f, .1f, .0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, em);
        glMateriali(GL_FRONT, GL_SHININESS, 96);
        
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
        
		// draw torus
		glColor3f(1.f, 1.f, 1.f);
		glRotated(0.0, 0.0, 0.0, 0.0);

		glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[0]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, torus_buffers[1]);
		glNormalPointer(GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, torus_buffers[2]);
		glDrawElements(GL_TRIANGLES, u0*u1 * 6, GL_UNSIGNED_INT, NULL);
        
        float specReflectionn[] = { 0.f, 0.f, 0.f, 1.0f };
        glMaterialfv(GL_FRONT, GL_SPECULAR, specReflectionn);
        float emn[] = { 0.f, 0.f, 0.f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, emn);
        glMateriali(GL_FRONT, GL_SHININESS, 96);
        glDisable(GL_LIGHTING);
        
		// draw vortex
		double timestep_sec = r.game_model()->timestep().count();
		for (unsigned int i = 0; i < 3; i++) {
			for (unsigned int t = vortex_length - 1; t > 0; --t) {
				vortex_dat[i][t][0] = vortex_dat[i][t - 1][0] + vec3_type(0, 0, timestep_sec * vortex_speed);
				vortex_dat[i][t][1] = vortex_dat[i][t][0] + (vortex_dat[i][t - 1][1] - vortex_dat[i][t - 1][0]) * 1.075;
			}
		}
        
        // set delta x movement
        if(lastKnownPosition(0) != pos(0) && abs(lastKnownPosition(0) - pos(0)) > 5)
        {
            deltaX = deltaX + (lastKnownPosition(0) - pos(0));
            lastKnownPosition = pos;
		}
        else
            deltaX = deltaX*0.9;
        vortex_dat[0][0][0] = vec3_type(-deltaX, 0, 0);
		vortex_dat[1][0][0] = vec3_type(-deltaX, 0, 0);
		vortex_dat[2][0][0] = vec3_type(-deltaX, 0, 0);

		// „Startpunkte“ gleichmäßig auf einem Kreis mit Radius (r0 – r1) an der Oberseite des Paddles
		double a1 = 2 * M_PI * (0 + _model->bladesAngle() / 180) / 3;
		vortex_dat[0][0][1] = vec3_type((r0 - r1) * cos(a1), (r0 - r1) * sin(a1), 0);
		double a2 = 2 * M_PI * (1 + _model->bladesAngle() / 180) / 3;
		vortex_dat[1][0][1] = vec3_type((r0 - r1) * cos(a2), (r0 - r1) * sin(a2), 0);
		double a3 = 2 * M_PI * (2 + _model->bladesAngle() / 180) / 3;
		vortex_dat[2][0][1] = vec3_type((r0 - r1) * cos(a3), (r0 - r1) * sin(a3), 0);

		for(unsigned int i = 0; i < 3; i++)
        {
			glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[i + 1]);
			double* dat = static_cast<double*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
            
			for(unsigned int t = vortex_length - 1; t > 0; --t)
            {
				vec3_type p = vortex_dat[i][t][1];
				p[0] -= 0.5 * vortex_width;
				*(dat++) = static_cast<double>(p[0]);
				*(dat++) = static_cast<double>(p[1]);
				*(dat++) = static_cast<double>(p[2]);
				p[0] += 0.5 * vortex_width;
				*(dat++) = static_cast<double>(p[0]);
				*(dat++) = static_cast<double>(p[1]);
				*(dat++) = static_cast<double>(p[2]);
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDisable(GL_CULL_FACE);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[0]);
		glColorPointer(4, GL_DOUBLE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[1]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 20*2);

		glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[2]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 20*2);

		glBindBuffer(GL_ARRAY_BUFFER, vortex_buffers[3]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 20*2);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);
        
		// draw rotor
		glColor3f(.4f, .8f, .4f);
		glRotated(_model->bladesAngle(), 0.0, 0.0, 1.0);

		glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[0]);
		glVertexPointer(3, GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, rotor_buffers[1]);
		glNormalPointer(GL_DOUBLE, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, rotor_buffers[2]);
		glDrawElements(GL_TRIANGLES, 2 * 3 * 9, GL_UNSIGNED_INT, NULL);

		// reset buffers and states
		glBindBuffer(GL_COLOR_ARRAY, NULL);
		glBindBuffer(GL_VERTEX_ARRAY, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, NULL);
        
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
        
        //glDisable(GL_LIGHT0);
        //glDisable(GL_LIGHTING);
    }
    glPopMatrix();
}
