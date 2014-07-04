# include "../../../include/flappy_box/view/game_over_al_audible.hpp"

# include <thread>
# include <functional>

# include "../../../include/math.hpp"

# include "../../../include/sound_provider.hpp"

using namespace ::flappy_box::view;


GameOverAlAudible::GameOverAlAudible( std::shared_ptr< ::flappy_box::model::GameOver > const& b )
: _model( b )
{
    // play the sound
    SoundProvider::getInstance()->playSound("laugh.wav", 0.0, 0.2, 1.0, false);
}

void GameOverAlAudible::auralize( ::view::AlRenderer& renderer )
{
}
