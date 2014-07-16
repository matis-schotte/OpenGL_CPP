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
    
    return false;
}
