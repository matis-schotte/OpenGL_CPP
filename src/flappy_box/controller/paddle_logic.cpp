# include "../../../include/flappy_box/controller/paddle_logic.hpp"

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
# include <GL/freeglut.h>

using namespace ::flappy_box::controller;

PaddleLogic::PaddleLogic(const std::shared_ptr< flappy_box::model::Paddle >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool PaddleLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    // key, special_key, modifier_mask, key_state, mouse_pos
    
    if(ev.key_state == ev.KEY_UP) _model->setPlayerControl(vec3_type(0., 0., 0.));
    else if(ev.key_state == ev.KEY_DOWN && ev.special_key == GLUT_KEY_LEFT) _model->setPlayerControl(vec3_type(-1., 0., 0.));
    else if(ev.key_state == ev.KEY_DOWN && ev.special_key == GLUT_KEY_RIGHT) _model->setPlayerControl(vec3_type(1., 0., 0.));
    
    vec3_type palt = _model->position();
    vec3_type valt = _model->velocity();
    vec3_type aalt = _model->acceleration();
    const double dt = l.game_model()->timestep().count();
    
    // gesucht: a, v, p
    vec3_type aneu = _model->playerControl()*1000; // Skalierung=1000
    vec3_type vneu = valt*0.8 + aneu*dt; // decay=0.8
    if(vneu(0) > 100.) vneu(0) = 100.; // limit speed to 100.
    vec3_type pneu = palt + vneu*dt;
    
    //_model->size();
    vec3_type max = _model->maxPosition();
    
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
    
	double t = l.game_model()->timestep().count();
	double bladesAngle = _model->bladesAngle();
	bladesAngle += 720 * t;
	bladesAngle = bladesAngle - ((static_cast<int>(bladesAngle) / 360) * 360); // avoid overflow
	_model->setBladesAngle(bladesAngle);

    return false;
}
