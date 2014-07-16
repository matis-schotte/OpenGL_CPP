# pragma once

# include "../../view/al_renderer.hpp"
# include "../../flappy_box/model/paddle.hpp"

namespace flappy_box
{
    namespace view
    {
        class PaddleAlAudible : public ::view::AlRenderer::Audible
        {
        public:
            PaddleAlAudible( std::shared_ptr< model::Paddle > const& );
            
            virtual void auralize( ::view::AlRenderer& ) override;
            
        private:
            std::shared_ptr< model::Paddle const > _model;
            
            ALuint buffer, source;
            double x_pos;
            
        }; // BoxAlAudible
    }
}
