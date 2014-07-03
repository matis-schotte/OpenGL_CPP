# include "../../../include/flappy_box/controller/paddle_logic.hpp"

# include <thread>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
# include <GL/freeglut.h>

# include <AL/alut.h>

using namespace ::flappy_box::controller;

PaddleLogic::PaddleLogic(const std::shared_ptr< flappy_box::model::Paddle >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool PaddleLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
	if (ev.key_state == ev.KEY_UP) _model->setPlayerControl(vec3_type(0.0, 0.0, 0.0));
	else if (ev.key_state == ev.KEY_DOWN && ev.special_key == GLUT_KEY_LEFT) _model->setPlayerControl(vec3_type(+1.0, 0.0, 0.0));
	else if (ev.key_state == ev.KEY_DOWN && ev.special_key == GLUT_KEY_RIGHT) _model->setPlayerControl(vec3_type(-1.0, 0.0, 0.0));

	const double dt = l.game_model()->timestep().count();
	vec3_type aneu = _model->playerControl() * 1000; // Als Beschleunigung aneu dient _model->playerControl() mit einer geeigneten Skalierung
	vec3_type valt = _model->velocity() * 0.8; // valt aus der Multiplikation von _model->velocity() mit einem skalaren Dämpfungskoeffizient
	vec3_type palt = _model->position();
	vec3_type dv = aneu * dt;
	
	vec3_type vneu = valt + dv;
	vec3_type pneu = palt + vneu*dt;

	// check and set max or min position
	vec3_type pmax = _model->maxPosition();
	if (pneu(0) > pmax(0)) pneu(0) = pmax(0);
	else if (pneu(0) < -pmax(0)) pneu(0) = -pmax(0);
	if (pneu(1) > pmax(1)) pneu(1) = pmax(1);
	else if (pneu(1) < -pmax(1)) pneu(1) = -pmax(1);
	if (pneu(2) > pmax(2)) pneu(2) = pmax(2);
	else if (pneu(2) < -pmax(2)) pneu(2) = -pmax(2);

	_model->setAcceleration(aneu);
	_model->setVelocity(vneu);
	_model->setPosition(pneu);

    return false;
}
