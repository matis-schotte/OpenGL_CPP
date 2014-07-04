# pragma once

# include "../../view/al_renderer.hpp"
# include "../../flappy_box/model/game_over.hpp"

namespace flappy_box
{
    namespace view
    {
        class GameOverAlAudible : public ::view::AlRenderer::Audible
        {
        public:
            GameOverAlAudible( std::shared_ptr< model::GameOver > const& );
            
            virtual void auralize( ::view::AlRenderer& ) override;
            
        private:
            std::shared_ptr< model::GameOver const > _model;
            
        }; // BoxAlAudible
    }
}
