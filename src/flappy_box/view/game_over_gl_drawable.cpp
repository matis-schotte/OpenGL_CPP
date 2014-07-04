# include "../../../include/flappy_box/view/game_over_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

GameOverGlDrawable::GameOverGlDrawable(const std::shared_ptr< ::flappy_box::model::GameOver >& b )
: _model( b )
{
    std::cout << "Game Over - Score: " << _model->playerPoints() << std::endl;
}

GameOverGlDrawable::~GameOverGlDrawable()
{
}

void GameOverGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
}
