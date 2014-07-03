# include "../../../include/flappy_box/controller/game_over_logic.hpp"

using namespace ::flappy_box::controller;

GameOverLogic::GameOverLogic(const std::shared_ptr< flappy_box::model::GameOver >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool GameOverLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    return false;
}
