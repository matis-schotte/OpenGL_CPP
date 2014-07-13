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

			GLuint glBuffers[3];
            
        }; // BoxGlDrawable
    }
}
