# pragma once

# include "../../view/gl_renderer.hpp"
# include "../../flappy_box/model/paddle.hpp"

#include "../../tiny_obj_loader.h"

namespace flappy_box
{
    namespace view
    {
        class PaddleGlDrawable : public ::view::GlRenderer::Drawable
        {
        public:
            
            PaddleGlDrawable( const std::shared_ptr< model::Paddle >& );
            virtual ~PaddleGlDrawable();
            virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;
			virtual void updateVBOs();
            
        private:
            std::shared_ptr< model::Paddle const > _model;
            std::vector<tinyobj::shape_t> shapes;

			double r0 = 0;
			double r1 = 0;
			const unsigned int u0 = 40;
			const unsigned int u1 = 15;
			const unsigned int blade_count = 9;
			const unsigned int vortex_length = 20;
			const unsigned int vortex_speed = 100;
			GLdouble vortex_width = 0.8;
			vec3_type vortex_dat[3][20][2];

			GLuint torus_buffers[3];
			GLuint rotor_buffers[3];
			GLuint vortex_buffers[4];            
        }; // BoxGlDrawable
    }
}
