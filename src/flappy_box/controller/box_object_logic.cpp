# include "../../../include/flappy_box/controller/box_object_logic.hpp"
# include "../../../include/flappy_box/model/world.hpp"

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
    const vec3_type agrav = flappy_box::model::Box::gravitation();
    vec3_type fext = _model->externalForce();
    const double d = flappy_box::model::Box::decay();
    const double dt = l.game_model()->timestep().count();
    const double m = _model->size()*_model->size()*_model->size(); // use volume of the cube
    vec3_type aext = fext / m; // umgestellt: fext = m * aext;
    
    // gesucht: a, v, p
    vec3_type aneu = aalt*d + aext + agrav;
    vec3_type vneu = valt + aneu*dt;
    vec3_type pneu = palt + vneu*dt;
    
    vec3_type max = _model->maxPosition();

    //TODO Begrenzung auf Minimalwerte -> min = max * -1 ?
    // Anmerkung an Franz: die abs() müssen schon da bleiben, denn pneu kann +/- sein
	// An Matis: Die "TODO" Markierung war da, weil keine Fallunterscheidungen für den "negativen Rand" behandelt wurden
    
	if(abs(pneu(0)) > max(0))
    {
		// keine elegante Lösung, aber müsste passen
		if (pneu(0) >= 0) pneu(0) = max(0);
		else pneu(0) = -max(0);
		vneu(0) = -vneu(0)*d; // unelastischer Stoss
	}
    
	if(abs(pneu(1)) > max(1))
    {
		// keine elegante Lösung, aber müsste passen
		if (pneu(1) >= 0) pneu(1) = max(1);
		else pneu(1) = -max(1);
		// HIER: Y-WERT wird nicht gespiegelt
	}
    
	if(abs(pneu(2)) > max(2))
    {
		// keine elegante Lösung, aber müsste passen
		if (pneu(2) >= 0) pneu(2) = max(2);
		else pneu(2) = -max(2);
		vneu(2) = -vneu(2)*d; // unelastischer Stoss
    }
    /*
     if(pneu(2) < -max(2))
     {
     // wenn kiste auf den boden fällt, wird ein leben abgezogen und kiste ist kaputt
     // achtung: gleiche logik bereits in der world implementiert, hier nicht notwendig ?!
     
     _model->setAlive(false);
     
     // find our world and decrease lives
     std::vector<std::shared_ptr<::model::GameObject>> allObjs = l.game_model()->objects();
     std::vector<std::shared_ptr<::model::GameObject>>::iterator it = std::find_if( allObjs.begin(), allObjs.end(), []( std::shared_ptr< ::model::GameObject > const& go )->bool{ return go->name().compare("World") == 0; } );
     std::shared_ptr< flappy_box::model::World > world = std::dynamic_pointer_cast< flappy_box::model::World >(*it); // assume first found object is right one, cuz there should only be one world
     
     world->setRemainingLives(world->remainingLives()-1);
     }
     */
    
    _model->setAcceleration(aneu);
    _model->setVelocity(vneu);
    _model->setPosition(pneu);
    
    // Bonus-Aufgabe, gesucht: a, v, angle
    double abneu = _model->rotAcceleration()*0.3 + aext(0)*60;
    double vbneu = _model->rotVelocity() + abneu*dt;
    double angle = _model->angle() + vbneu*dt;
    
	// avoid double overflow by subtraction
	if(angle > 360.)
        angle -= 360.;
    
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
