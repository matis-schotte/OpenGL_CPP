# pragma once

# include "../../view/al_renderer.hpp"
# include "../../flappy_box/model/world.hpp"

namespace flappy_box
{
    namespace view
    {
        class WorldAlAudible : public ::view::AlRenderer::Audible
        {
        public:
            WorldAlAudible( std::shared_ptr< model::World > const& );
            
            virtual void auralize( ::view::AlRenderer& ) override;
            
        private:
            std::shared_ptr< model::World const > _model;
            
        }; // BoxAlAudible
    }
}
