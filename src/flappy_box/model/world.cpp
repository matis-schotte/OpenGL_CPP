# include "../../../include/flappy_box/model/world.hpp"

using namespace flappy_box::model;

World::World( const std::string& n )
: GameObject( true, n )
,           _playerPoints( 0 )
,         _remainingLives( 0 )
,          _shouldRestart( false )
,               _position( 0., 200., 2. )
{
}
