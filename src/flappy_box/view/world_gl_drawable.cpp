# include "../../../include/flappy_box/view/world_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

WorldGlDrawable::WorldGlDrawable(const std::shared_ptr< ::flappy_box::model::World >& b )
: _model( b )
{
}

WorldGlDrawable::~WorldGlDrawable()
{
}

void WorldGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
}
