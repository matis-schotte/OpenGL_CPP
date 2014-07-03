# include "../../../include/flappy_box/controller/box_object_logic.hpp"

# include <cstdlib>

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
    
    if(abs(pneu(0)) > max(0)) // IS THIS CORRECT ?? Betrag wählen?
        vneu(0) = -(vneu(0)*d);
    if(abs(pneu(1)) > max(1)) // IS THIS CORRECT ?? Betrag wählen? HIER: Y-WERT wird nicht gespiegelt
        vneu(1) = -(vneu(1)*d);
    //if(abs(pneu(2)) > max(2)) // IS THIS CORRECT ?? Betrag wählen?
    //    vneu(2) = -(vneu(2)*d);
    
    _model->setAcceleration(aneu);
    _model->setVelocity(vneu);
    _model->setPosition(pneu);
    
    // Bonus-Aufgabe, gesucht: a, v, angle
    double abneu = 1.2*(_model->rotAcceleration()*d*0.6 + aext(1) + flappy_box::model::Box::gravitation()(1));
    double vbneu = _model->rotVelocity() + abneu*dt;
    double angle = _model->angle() + vbneu*dt;
    
    _model->setAngle(angle);
    _model->setRotVelocity(vbneu);
    _model->setRotAcceleration(abneu);
    
    /* // test: stopping the external force, and testing decay of rotation
    std::cout << "ANGLE: " << angle << std::endl;
    if(abs(angle) > 28.)
    {
        fext(0) = 0.;fext(1) = 0.;fext(2) = 0.;
        _model->setExternalForce(fext);
        _model->setSize(1.);
    }
     */
    
    return false;
}
