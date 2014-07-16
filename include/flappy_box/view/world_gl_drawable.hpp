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
            std::shared_ptr< model::World const > _model;
            static std::vector<tinyobj::shape_t> shapes;
            int bw = 0;
            GLfloat fogColor[4]= {1.f, 1.f, 1.f, 1.0f};
            
        }; // BoxGlDrawable
    }
}
