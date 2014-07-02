# include "../../../include/flappy_box/model/game_over.hpp"

using namespace flappy_box::model;

GameOver::GameOver( const std::string& n )
: GameObject( true, n )
,           _angle( 0.0 )
,        _position( 0.0, 0.0, 0.0 )
{
}