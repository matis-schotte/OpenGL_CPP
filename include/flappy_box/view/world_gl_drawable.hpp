# pragma once

# include "../../view/gl_renderer.hpp"
# include "../../flappy_box/model/world.hpp"

#include "../../tiny_obj_loader.h"

namespace flappy_box
{
    namespace view
    {
        class WorldGlDrawable : public ::view::GlRenderer::Drawable
        {
        public:
            
            WorldGlDrawable( const std::shared_ptr< model::World >& );
            virtual ~WorldGlDrawable();
            virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;
            
        private:
            void renderBitmapString(const char *string);
            
            std::shared_ptr< model::World const > _model;
            static std::vector<tinyobj::shape_t> shapes;
            int bw = 0;

			GLfloat fogColor[4];
			GLfloat* getFogColor() { 
				fogColor[0] = 1.f;  
				fogColor[1] = 1.f; 
				fogColor[2] = 1.f; 
				fogColor[3] = 1.f; 
				return fogColor;
		};
            
        }; // BoxGlDrawable
    }
}
