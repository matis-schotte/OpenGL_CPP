# include "../../../include/flappy_box/controller/world_logic.hpp"

# include "../../../include/model/game_object.hpp"
# include "../../../include/flappy_box/model/game_over.hpp"

# include <memory>

using namespace ::flappy_box::controller;

WorldLogic::WorldLogic( const std::shared_ptr< flappy_box::model::World >& b, const bool r )
: ::controller::Logic::ObjectLogic()
, _model( b )
, _shallRestartTheGame( r )
{
    xKoord = std::uniform_int_distribution<int>(-_model->getWorldHalfWidth(), _model->getWorldHalfWidth());
    boxSize = std::uniform_int_distribution<int>(3, 10); // random box size
}

bool WorldLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    if(_shallRestartTheGame)
        restartGame(l);
    
    if(l.game_model()->timestep().count() > 10.) // CORRECT? 10 seconds?
        addBoxToGame(l);
    
    // find our paddle
    std::vector<std::shared_ptr<::model::GameObject>> allObjs = l.game_model()->objects();
    std::vector<std::shared_ptr<::model::GameObject>>::iterator it = std::find_if( allObjs.begin(), allObjs.end(), []( std::shared_ptr< ::model::GameObject > const& go )->bool{ return go->name().compare("World") == 0; } );
    std::shared_ptr< flappy_box::model::Paddle > paddle = std::dynamic_pointer_cast< flappy_box::model::Paddle >(*it);
    
    if(l.game_model()->timestep().count() > 1.) // CORRECT? 10 seconds?
        _model->setPlayerPoints(_model->playerPoints()+1);
    
    if(_model->remainingLives() < 1)
    {
        // invalidate game model
        for(std::shared_ptr<::model::GameObject> go : l.game_model()->objects())
            go->setAlive(false);
        
        // except world
        _model->setAlive( true );
        
        // add game over gameobject
        l.game_model()->addGameObject( std::shared_ptr< flappy_box::model::GameOver >( new flappy_box::model::GameOver( _model->playerPoints() ) ) );
    }
    
    for(std::shared_ptr<::model::GameObject> go : l.game_model()->objects())
    {
        std::shared_ptr< flappy_box::model::Box > box = std::dynamic_pointer_cast< flappy_box::model::Box >(go);
        if(box && box->isAlive())
        {
            if(box->position()(3) < paddle->position()(3))
            {
                box->setAlive(false);
                _model->setRemainingLives(_model->remainingLives()-1);
            }
            else
            {
                setForce(box, paddle);
                
                for(std::shared_ptr<::model::GameObject> boxColl : l.game_model()->objects())
                {
                    std::shared_ptr< flappy_box::model::Box > boxCollision = std::dynamic_pointer_cast< flappy_box::model::Box >(boxColl);
                    
                    // check for collisions
                    if(boxCollision && boxCollision->isAlive() && box != boxCollision)
                    {
                        // umhüllende kugel mit center=position und radius=size
                        // schnitt-test: theoretisch abstand beider center berechnen und dieser abstandd muss größer sein als beide radien
                        // see http://www.ina-de-brabandt.de/vektoren/a/abstand-2p-in-r3.html
                        
                        if((boxCollision->size() + box->size()) > sqrt((boxCollision->position() - box->position()).cwiseAbs2().sum()))
                        { // collision detected
                            boxCollision->setAlive(false);
                            box->setAlive(false);
                            
                            // bonus points
                            _model->setPlayerPoints(_model->playerPoints()+10);
                            
                            // box is not alive anymore, exit inner loop
                            break;
                        }
                    }
                } // inner loop
            }
        }
    } // outer loop
    
    return false;
}

void WorldLogic::addBoxToGame( ::controller::Logic& l ) // add boxes to l
{
    // create new box
    std::shared_ptr< flappy_box::model::Box > newBox;
    
    // set random size
    newBox->setSize((double)boxSize(rd));
    
    // set max size (world size)
    vec3_type maxP = newBox->maxPosition();
    maxP(0) = _model->getWorldHalfWidth() - newBox->size()/2.0;
    maxP(1) = 400;
    maxP(2) = _model->getWorldHalfHeight() - newBox->size()/2.0;
    newBox->setMaxPosition(maxP);
    
    // set random x value
    vec3_type pos = newBox->position();
    pos(0) = xKoord(rd);
    if(pos(0) > maxP(0)) pos(0) = maxP(0)-newBox->size(); else if(pos(0) < -maxP(0)) pos(0) = maxP(0)+newBox->size();
    pos(1) = 200;
    pos(2) = maxP(0) * 0.75;
    newBox->setPosition(pos);
    
    l.game_model()->addGameObject( newBox );
}

void WorldLogic::setForce( std::shared_ptr< flappy_box::model::Box > & box, std::shared_ptr< flappy_box::model::Paddle > & paddle )
{
    // apply force to boxes
    
}

void WorldLogic::restartGame( ::controller::Logic& l )
{
    // invalidate all game objects
    for ( auto o : l.game_model()->objects() )
    {
        o->setAlive( false );
    }
    
    // reject invalidation for world object
    _model->setAlive( true );
    _model->setPlayerPoints( 0 );
    _model->setRemainingLives( 9 );
    
    // create and configure new paddle object
    std::shared_ptr< flappy_box::model::Paddle > user_paddle = std::make_shared< flappy_box::model::Paddle >("Paddle");
    user_paddle->setSize( vec3_type( 10.0, 1.0, 2.5 ));
    user_paddle->setPosition( vec3_type( 0.0, 0.0, - _model->getWorldHalfHeight()
                                        + user_paddle->size()[2] * 2.0 ));
    user_paddle->setMaxPosition( vec3_type( _model->getWorldHalfWidth()
                                           - user_paddle->size()[0] * 0.5, 0.0, _model->getWorldHalfHeight() ) );
    // add paddle object
    l.game_model()->addGameObject( user_paddle );
    
    // unset restart flag
    _shallRestartTheGame = false;
}
