# include "../../../include/flappy_box/model/game_over.hpp"

using namespace flappy_box::model;

GameOver::GameOver( const int p, const std::string& n )
: GameObject( true, n )
,           _playerPoints( p )
{
}
