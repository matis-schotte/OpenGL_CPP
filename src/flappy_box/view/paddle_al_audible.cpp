# include "../../../include/flappy_box/view/paddle_al_audible.hpp"

# include <thread>
# include <functional>

# include "../../../include/math.hpp"

# include "../../../include/sound_provider.hpp"

using namespace ::flappy_box::view;


PaddleAlAudible::PaddleAlAudible( std::shared_ptr< ::flappy_box::model::Paddle > const& b )
: _model( b )
{
    buffer = alutCreateBufferWaveform(ALUT_WAVEFORM_SINE, 100, 0, 1);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, true);
    alSourcef(source, AL_GAIN, 0.4f);
    alSourcef(source, AL_PITCH, 0.5f);
    
    alSourcePlay(source);
}

void PaddleAlAudible::auralize( ::view::AlRenderer& renderer )
{
    if(_model->isAlive())
    {
        // normalized x-coordinate of box
        x_pos = _model->position()[0] / _model->maxPosition()[0];
        alSource3f(source, AL_POSITION, static_cast<float>(x_pos), 0.0f, 0.0f);
        
        // pitch = speed
        vec3_type speed = _model->velocity();
        double p = ((speed(0)>0)?speed(0):-speed(0)) / 100. + 0.5;
        
		alSourcef(source, AL_PITCH, static_cast<float>(p));
    }
    else
    {
        alSourceStop(source);
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
    }
}
