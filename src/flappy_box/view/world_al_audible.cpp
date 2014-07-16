# include "../../../include/flappy_box/view/world_al_audible.hpp"

# include <thread>
# include <functional>

# include "../../../include/math.hpp"

# include "../../../include/sound_provider.hpp"

using namespace ::flappy_box::view;


WorldAlAudible::WorldAlAudible( std::shared_ptr< ::flappy_box::model::World > const& b )
: _model( b )
{
    // play the sound
    SoundProvider::getInstance()->playSound("beat.wav", 0.0, 0.2, 0.8, true);
    SoundProvider::getInstance()->playSound("high.wav", 0.0, 0.2, 0.8, true);
    SoundProvider::getInstance()->playSound("synth.wav", 0.0, 0.2, 1.0, true);
}

void WorldAlAudible::auralize( ::view::AlRenderer& renderer )
{
}
