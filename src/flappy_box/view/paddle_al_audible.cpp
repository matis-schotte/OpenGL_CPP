# include "../../../include/flappy_box/view/paddle_al_audible.hpp"

# include <thread>
# include <functional>

# include "../../../include/math.hpp"

# include "../../../include/sound_provider.hpp"

using namespace ::flappy_box::view;


PaddleAlAudible::PaddleAlAudible( std::shared_ptr< ::flappy_box::model::Paddle > const& b )
: _model( b )
{
    // normalized x-coordinate of box
    double x_pos = b->position()[0] / b->maxPosition()[0];
    
    // play the sound
    SoundProvider::getInstance()->playSound("synth.wav", x_pos, 0.2, 1.0, true);
}

void PaddleAlAudible::auralize( ::view::AlRenderer& renderer )
{
}
