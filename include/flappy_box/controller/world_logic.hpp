# pragma once

# include "../../controller/logic.hpp"
# include "../../flappy_box/model/world.hpp"
# include "../../flappy_box/model/box.hpp"
# include "../../flappy_box/model/paddle.hpp"

# include <random>

namespace flappy_box
{
    namespace controller
    {
        class WorldLogic : public ::controller::Logic::ObjectLogic
        {
        public:
            WorldLogic() = delete;
            WorldLogic( const std::shared_ptr< model::World >&, const bool = true );
            
            virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;
            
        private:
            
            void travelWorld( ::controller::Logic& l );
            void addBoxToGame( ::controller::Logic& l );
            void setForce( std::shared_ptr< flappy_box::model::Box > & box, std::shared_ptr< flappy_box::model::Paddle > & paddle );
            void restartGame( ::controller::Logic& l );
            
            std::shared_ptr< model::World > _model;
            std::shared_ptr< flappy_box::model::Paddle > paddle;
            
            bool _shallRestartTheGame;
            bool _gameOverScreen;
            
            std::random_device rd;
            std::uniform_int_distribution<int> xKoord,boxSize,newBoxTime;
            double ticks, newBoxWhen;
        };
    }
    
} // audiophile
