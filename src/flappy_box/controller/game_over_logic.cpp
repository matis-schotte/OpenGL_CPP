# include "../../../include/flappy_box/controller/game_over_logic.hpp"
# include "../../../include/flappy_box/model/world.hpp"

using namespace ::flappy_box::controller;

GameOverLogic::GameOverLogic(const std::shared_ptr< flappy_box::model::GameOver >& go )
: ::controller::Logic::ObjectLogic()
, _model( go )
{}

bool GameOverLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    // restart when r pressed
    if(ev.key == 'r' || ev.key == 'R')
    {
        // find our world and restart
        std::vector<std::shared_ptr<::model::GameObject>> allObjs = l.game_model()->objects();
        std::vector<std::shared_ptr<::model::GameObject>>::iterator it = std::find_if( allObjs.begin(), allObjs.end(), []( std::shared_ptr< ::model::GameObject > const& go )->bool{ return go->name().compare("World") == 0; } );
        std::shared_ptr< flappy_box::model::World > world = std::dynamic_pointer_cast< flappy_box::model::World >(*it); // assume first found object is right one, cuz there should only be one world
        
        world->setShouldRestart(true);
    }
    
    if(ev.key == 'q' || ev.key == 'q')
    {
        exit(0);
    }
    
    const double dt = l.game_model()->timestep().count();
    double d = _model->fadingColor()+v*dt;
    if(d > 1.) v = -1.;
    if(d < 0.) v = +1.;
    _model->setFadingColor(d);
    
    
    return false;
}
