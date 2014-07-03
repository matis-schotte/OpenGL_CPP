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
    
    switch(ev.special_key)
    {
        case GLUT_KEY_LEFT:
            if(ev.key_state == ev.KEY_DOWN)
                _model->setPlayerControl(vec3_type(-1., 0., 0.));
            else if(ev.key_state == ev.KEY_UP)
                _model->setPlayerControl(vec3_type(0., 0., 0.));
            break;
        
        case GLUT_KEY_RIGHT:
            if(ev.key_state == ev.KEY_DOWN)
                _model->setPlayerControl(vec3_type(1., 0., 0.));
            else if(ev.key_state == ev.KEY_UP)
                _model->setPlayerControl(vec3_type(0., 0., 0.));
            break;
    }
    
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
    
    if(abs(pneu(0)) > max(0)) // IS THIS CORRECT ?? Betrag wählen?
        vneu(0) = -vneu(0);
    
    _model->setAcceleration(aneu);
    _model->setVelocity(vneu);
    _model->setPosition(pneu);
    
    return false;
}
