# pragma once

# include "../../controller/logic.hpp"
# include "../../flappy_box/model/paddle.hpp"

namespace flappy_box
{
    namespace controller
    {
        class PaddleLogic : public ::controller::Logic::ObjectLogic
        {
        public:
            PaddleLogic() = delete;
            PaddleLogic( const std::shared_ptr< model::Paddle >& );
            
            virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;
            
        private:
            std::shared_ptr< model::Paddle > _model;
        };
    }
    
} // audiophile
