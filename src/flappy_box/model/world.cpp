# include "../../../include/flappy_box/model/world.hpp"

using namespace flappy_box::model;

World::World( const std::string& n )
: GameObject( true, n )
,           _angle( 0.0 )
,        _position( 0.0, 0.0, 0.0 )
{
}