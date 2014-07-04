# include "../../../include/flappy_box/controller/flappy_engine.hpp"

# include "../../../include/flappy_box/model/box.hpp"
# include "../../../include/flappy_box/model/paddle.hpp"
# include "../../../include/flappy_box/model/world.hpp"
# include "../../../include/flappy_box/model/game_over.hpp"

# include "../../../include/flappy_box/controller/box_object_logic.hpp"
# include "../../../include/flappy_box/controller/paddle_logic.hpp"
# include "../../../include/flappy_box/controller/world_logic.hpp"
# include "../../../include/flappy_box/controller/game_over_logic.hpp"

# include "../../../include/flappy_box/view/box_gl_drawable.hpp"
# include "../../../include/flappy_box/view/paddle_gl_drawable.hpp"
# include "../../../include/flappy_box/view/world_gl_drawable.hpp"
# include "../../../include/flappy_box/view/game_over_gl_drawable.hpp"

# include "../../../include/flappy_box/view/box_al_audible.hpp"
# include "../../../include/flappy_box/view/paddle_al_audible.hpp"
# include "../../../include/flappy_box/view/world_al_audible.hpp"
# include "../../../include/flappy_box/view/game_over_al_audible.hpp"

# include "../../../include/view/glut_window.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
# include <GL/freeglut.h>
# include <AL/alut.h>

using namespace ::flappy_box::controller;

static std::function< void () > __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

FlappyEngine::FlappyEngine( const std::shared_ptr< ::controller::Logic >& l )
: ::controller::GlutEngine( l )
, _al_renderer( std::make_shared< ::view::AlRenderer >( game_model() ) )
, _gl_renderer( std::make_shared< ::view::GlRenderer >( game_model() ) )
{}

void FlappyEngine::init( int& argc, char** argv )
{
    GlutEngine::init( argc, argv );
    
    alutInit( &argc, argv );
    
    // register the delegate classes for Box, Paddle, World & GameOver
    game_logic()->logic_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< BoxObjectLogic > ( b ); } );
    game_logic()->logic_factory().register_module< flappy_box::model::Paddle >( []( std::shared_ptr< flappy_box::model::Paddle > const& b ) { return std::make_shared< PaddleLogic > ( b ); } );
    game_logic()->logic_factory().register_module< flappy_box::model::World >( []( std::shared_ptr< flappy_box::model::World > const& b ) { return std::make_shared< WorldLogic > ( b ); } );
    game_logic()->logic_factory().register_module< flappy_box::model::GameOver >( []( std::shared_ptr< flappy_box::model::GameOver > const& b ) { return std::make_shared< GameOverLogic > ( b ); } );
    
    al_renderer()->audible_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< view::BoxAlAudible > ( b ); } );
    al_renderer()->audible_factory().register_module< flappy_box::model::Paddle >( []( std::shared_ptr< flappy_box::model::Paddle > const& b ) { return std::make_shared< view::PaddleAlAudible > ( b ); } );
    al_renderer()->audible_factory().register_module< flappy_box::model::World >( []( std::shared_ptr< flappy_box::model::World > const& b ) { return std::make_shared< view::WorldAlAudible > ( b ); } );
    al_renderer()->audible_factory().register_module< flappy_box::model::GameOver >( []( std::shared_ptr< flappy_box::model::GameOver > const& b ) { return std::make_shared< view::GameOverAlAudible > ( b ); } );
    
    gl_renderer()->drawable_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< view::BoxGlDrawable >( b ); } );
    gl_renderer()->drawable_factory().register_module< flappy_box::model::Paddle >( []( std::shared_ptr< flappy_box::model::Paddle > const& b ) { return std::make_shared< view::PaddleGlDrawable >( b ); } );
    gl_renderer()->drawable_factory().register_module< flappy_box::model::World >( []( std::shared_ptr< flappy_box::model::World > const& b ) { return std::make_shared< view::WorldGlDrawable >( b ); } );
    gl_renderer()->drawable_factory().register_module< flappy_box::model::GameOver >( []( std::shared_ptr< flappy_box::model::GameOver > const& b ) { return std::make_shared< view::GameOverGlDrawable >( b ); } );
    
    // create our world
    game_model()->addGameObject( std::make_shared< flappy_box::model::World >() );
    
    /*
    // create one single cube (to be deleted later...)
    std::shared_ptr< flappy_box::model::Box > box = std::make_shared< flappy_box::model::Box >("Box");
    box->setSize(20.0);
    box->setAngle(22.5);
    game_model()->addGameObject( box );
    */
}


void FlappyEngine::run()
{
    // Create a window and connect it with a view::GlRenderer and an InputEventHandler.
    auto window = std::make_shared< ::view::GlutWindow >( "flappy_box", 1500, 1000, gl_renderer() , shared_from_this() );
    
    // run game
    GlutEngine::run();
    
    alutExit();
}

void FlappyEngine::step( ::controller::InputEventHandler::keyboard_event const& ev )
{
    ::controller::GlutEngine::step( ev );
    al_renderer()->auralize_model();
    glutPostRedisplay();
}

