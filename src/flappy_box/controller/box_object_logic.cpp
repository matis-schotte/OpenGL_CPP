# include "../../../include/flappy_box/controller/box_object_logic.hpp"

# include <thread>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
# include <GL/freeglut.h>

# include <AL/alut.h>

using namespace ::flappy_box::controller;

BoxObjectLogic::BoxObjectLogic(const std::shared_ptr< flappy_box::model::Box >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool BoxObjectLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    vec3_type palt = _model->position();
    vec3_type valt = _model->velocity();
    vec3_type aalt = _model->acceleration();
    //vec3_type agrav = flappy_box::model::Box::gravitation();
    vec3_type fext = _model->externalForce(); // IS THIS CORRECT ??
    const double d = flappy_box::model::Box::decay;
    const double dt = l.game_model()->timestep().count();
    const double m = _model->size()*_model->size()*_model->size(); // use volume of the cube
    vec3_type aext = fext / m; // umgestellt: fext = m * aext;
    
    // gesucht: a, v, p
    vec3_type aneu = aalt*d + aext + flappy_box::model::Box::gravitation();
    vec3_type vneu = valt + aneu*dt;
    vec3_type pneu = palt + vneu*dt;
    
    vec3_type max = _model->maxPosition();
    
    if(pneu(0) > max(0)) // IS THIS CORRECT ?? Betrag wählen?
        vneu(0) = -vneu(0)*d;
    //if(pneu(1) > max(1)) // IS THIS CORRECT ?? Betrag wählen? HIER: Y-WERT wird nicht gespiegelt
    //    vneu(1) = -vneu(1)*d;
    if(pneu(2) > max(2)) // IS THIS CORRECT ?? Betrag wählen?
        vneu(2) = -vneu(2)*d;
    
    _model->setAcceleration(aneu);
    _model->setVelocity(vneu);
    _model->setPosition(pneu);
    
    // Bonus-Aufgabe, gesucht: a, v, angle
    double abneu = _model->rotAcceleration()*d + aext(0) + flappy_box::model::Box::gravitation()(2);
    double vbneu = _model->rotVelocity() + abneu*dt;
    if(fext(0) < 0) // negativ / IS THIS CORRECT ?? welcher wert, 0, 1, oder 2 ? (x,y,z)
        vbneu = -vbneu;
    //else // positiv
    //    vbneu = vbneu;
    double angle = _model->angle() + vbneu*dt;
    
    _model->setAngle(angle);
    _model->setRotVelocity(vbneu);
    _model->setRotAcceleration(abneu);
    
    return false;
}
