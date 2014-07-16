# pragma once

# include "../../controller/logic.hpp"
# include "../../flappy_box/model/game_over.hpp"

namespace flappy_box
{
    namespace controller
    {
        class GameOverLogic : public ::controller::Logic::ObjectLogic
        {
        public:
            GameOverLogic() = delete;
            GameOverLogic( const std::shared_ptr< model::GameOver >& );
            
            virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;
            
        private:
            std::shared_ptr< model::GameOver > _model;
            double v = 1.; // text flashing speed
        };
    }
    
} // audiophile
