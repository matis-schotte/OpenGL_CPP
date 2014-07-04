# include "../../../include/flappy_box/model/box.hpp"

using namespace flappy_box::model;

Box::Box( const std::string& n )
: GameObject( true, n )
,           _angle( 0.0 )
,            _size( 1.0 )
,        _position( 0.0, 200.0, 0.0 )
,     _maxPosition( 42.0, 10000.0, 30.0 ) // see world.hpp's getWorldHalfWidth() & getWorldHalfHeight()
,        _velocity( 0.0, -10.0, 5.0 )
,    _acceleration( 0.0, -300.0, 0.0 )
,   _externalForce( 0.0, 0.0, 0.0 )
, _rotAcceleration( 0.0 )
,     _rotVelocity( 0.0 )
{
}
