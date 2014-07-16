# include "../../../include/flappy_box/view/game_over_gl_drawable.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;

GameOverGlDrawable::GameOverGlDrawable(const std::shared_ptr< ::flappy_box::model::GameOver >& b )
: _model( b )
{
}

GameOverGlDrawable::~GameOverGlDrawable()
{
}

void GameOverGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
    glPushMatrix();
    {
        // render game over message
        glDisable(GL_LIGHTING);
        char text[64];
        sprintf(text, "Game Over - Score: %u", _model->playerPoints());
        glColor3f(1.0f, .0f, .0f);
        glRasterPos3d(-7., 0., 3.);
        renderBitmapString(text);
        glRasterPos3d(-6., 0., 0.);
        renderBitmapString("Restart by pressing r");
        glRasterPos3d(-5., 0., -3.);
        renderBitmapString("Quit by pressing q");
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();
}

void GameOverGlDrawable::renderBitmapString(const char *string)
{
    for(const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}
