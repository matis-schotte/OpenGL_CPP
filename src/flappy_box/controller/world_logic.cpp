# include "../../../include/flappy_box/controller/world_logic.hpp"

# include "../../../include/model/game_object.hpp"
# include "../../../include/flappy_box/model/game_over.hpp"

# include <memory>

using namespace ::flappy_box::controller;

WorldLogic::WorldLogic( const std::shared_ptr< flappy_box::model::World >& b, const bool r )
: ::controller::Logic::ObjectLogic()
, _model( b )
, _shallRestartTheGame( r )
, _gameOverScreen( false )
{
    xKoord = std::uniform_int_distribution<int>(static_cast<int>(-_model->getWorldHalfWidth()), static_cast<int>(_model->getWorldHalfWidth()));
    boxSize = std::uniform_int_distribution<int>(1, 3); // random box size
    newBoxTime = std::uniform_int_distribution<int>(1, 2); // random box creation time
    
    newBoxWhen = 0.0;
    ticks = 0.0;
}

bool WorldLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
    if(_shallRestartTheGame)
        restartGame(l);
    
    ticks += l.game_model()->timestep().count();
    if(ticks > newBoxWhen && !_gameOverScreen)
    {
        newBoxWhen = (double)newBoxTime(rd);
        ticks = 0.0;
        addBoxToGame(l);
    }
    
    /* // stupid - in every loop we are searching for our paddle??? bad performance
    // find our paddle
    std::vector<std::shared_ptr<::model::GameObject>> allObjs = l.game_model()->objects();
    std::vector<std::shared_ptr<::model::GameObject>>::iterator it = std::find_if( allObjs.begin(), allObjs.end(), []( std::shared_ptr< ::model::GameObject > const& go )->bool{ return go->name().compare("Paddle") == 0; } );
    std::shared_ptr< flappy_box::model::Paddle > paddle = std::dynamic_pointer_cast< flappy_box::model::Paddle >(*it); // assume first found object is right one, cuz there should only be one paddle
    */
    
    if(!paddle || !paddle->isAlive()) // no paddle exists or already invalidated
    {
        // create and configure new paddle object
        paddle = std::make_shared< flappy_box::model::Paddle >("Paddle");
        paddle->setSize( vec3_type( 10.0, 1.0, 2.5 ));
        paddle->setPosition( vec3_type( -30.0, 0.0, - _model->getWorldHalfHeight()
                                       + paddle->size()[2] * 2.0 ));
        paddle->setMaxPosition( vec3_type( _model->getWorldHalfWidth()
                                          - paddle->size()[0] * 0.5, 0.0, _model->getWorldHalfHeight() ) );
        // add paddle object
        l.game_model()->addGameObject( paddle );
    }
    
    if(!_gameOverScreen) _model->setPlayerPoints(_model->playerPoints()+1); // add points over time
    
    if(_model->remainingLives() <= 0 && !_gameOverScreen)
    {
        // invalidate game model
        for(auto go : l.game_model()->objects())
            go->setAlive(false);
        
        // except world
        _model->setAlive( true );
        
        // add game over gameobject
        l.game_model()->addGameObject( std::make_shared< flappy_box::model::GameOver >( _model->playerPoints() ) );
        _gameOverScreen = true;
    }
    else if(_model->remainingLives() <= 0 && _gameOverScreen && _model->shouldRestart())
    {
        _gameOverScreen = false;
        _shallRestartTheGame = true;
    }
    
    if(!_gameOverScreen)
        for(auto go : l.game_model()->objects())
        {
            std::shared_ptr< flappy_box::model::Box > box = std::dynamic_pointer_cast< flappy_box::model::Box >(go);
            if(box && box->isAlive())
            {
                if(box->position()(2) <= paddle->position()(2))
                {
                    box->setAlive(false);
                    _model->setRemainingLives(_model->remainingLives()-1);
                }
                else
                {
                    setForce(box, paddle);
                    
                    for(auto boxColl : l.game_model()->objects())
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
    std::shared_ptr< flappy_box::model::Box > newBox( new flappy_box::model::Box );
    
    // set random size
    newBox->setSize((double)boxSize(rd));
    
    // set max size (world size)
    vec3_type maxP = newBox->maxPosition();
    maxP(0) = _model->getWorldHalfWidth() - newBox->size()/2.0;
	maxP(1) = 0;//400 - newBox->size() / 2.0;
    maxP(2) = _model->getWorldHalfHeight() - newBox->size()/2.0;
    newBox->setMaxPosition(maxP);
    
    // set random x value
    vec3_type pos = newBox->position();
    pos(0) = xKoord(rd);
    if(pos(0) > maxP(0)) pos(0) = maxP(0)-newBox->size(); else if(pos(0) < -maxP(0)) pos(0) = maxP(0)+newBox->size();
    pos(1) = 0;
    pos(2) = maxP(2) * 0.75;
    newBox->setPosition(pos);
    
    l.game_model()->addGameObject( newBox );
}

void WorldLogic::setForce( std::shared_ptr< flappy_box::model::Box > & box, std::shared_ptr< flappy_box::model::Paddle > & paddle )
{
    // apply force to boxes
    double s = 5 * box->size()*box->size();
    
    // box muss vollständig im paddle-bereich liegen; also muss paddle immer kleiner im -bereich oder größer im +bereich sein
    if(paddle->position()(0)-0.5*paddle->size()(0) < box->position()(0)-0.5*box->size() &&
       paddle->position()(0)+0.5*paddle->size()(0) > box->position()(0)+0.5*box->size())
    {
        vec3_type f(0., 0., 1.);
        f = f * s;
        box->setExternalForce(f);
    }
    else
    {
        vec3_type padPos = paddle->position();
        vec3_type boxPos = box->position();
        
        if(abs(paddle->position()(0)-0.5*paddle->size()(0) - box->position()(0)-0.5*box->size()) > // near left side of paddle
           abs(paddle->position()(0)+0.5*paddle->size()(0) - box->position()(0)+0.5*box->size()))
            padPos(0) = padPos(0)-0.5*paddle->size()(0);
        else // near right side of paddle
            padPos(0) = padPos(0)+0.5*paddle->size()(0);
        
        vec3_type f = (boxPos-padPos);
        f.normalize();
        
        double k = f.dot(vec3_type(0., 0., 1.)); // mit skalar multiplizieren?
        
        f = f * s * k;
        box->setExternalForce(f);
    }
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
    _model->setRemainingLives( 1 );
    
    // create and configure new paddle object
    paddle = std::make_shared< flappy_box::model::Paddle >("Paddle");
    paddle->setSize( vec3_type( 10.0, 1.0, 2.5 ));
    paddle->setPosition( vec3_type( -30.0, 0.0, - _model->getWorldHalfHeight()
                                   + paddle->size()[2] * 2.0 ));
    paddle->setMaxPosition( vec3_type( _model->getWorldHalfWidth()
                                      - paddle->size()[0] * 0.5, 0.0, _model->getWorldHalfHeight() ) );
    // add paddle object
    l.game_model()->addGameObject( paddle );
    
    // unset restart flag
    _shallRestartTheGame = false;
}
