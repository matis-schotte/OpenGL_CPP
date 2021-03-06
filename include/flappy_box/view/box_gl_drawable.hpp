# pragma once

# include "../../view/gl_renderer.hpp"
# include "../../flappy_box/model/box.hpp"

#include "../../tiny_obj_loader.h"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

namespace flappy_box
{
    namespace view
    {
        class BoxGlDrawable : public ::view::GlRenderer::Drawable
        {
        public:
            
            BoxGlDrawable( const std::shared_ptr< model::Box >& );
            virtual ~BoxGlDrawable();
            virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;
            
        private:
            std::shared_ptr< model::Box const > _model;
            static std::vector<tinyobj::shape_t> shapes;
            
			GLfloat vertices[72];
			GLfloat *normals;
			GLfloat *texCoords;

			GLubyte pixels[128][128][3];
			GLuint texture;
        }; // BoxGlDrawable
    }
}
