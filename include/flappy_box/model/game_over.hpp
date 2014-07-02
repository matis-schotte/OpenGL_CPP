# pragma once

# include "../../model/game_object.hpp"
# include "../../math.hpp"

// ATTENTION: NEW CODE!

namespace flappy_box
{
    namespace model
    {
        class GameOver : public ::model::GameObject
        {
        public:
            
            GameOver( const std::string& = "GameOver" );
            
            const double& angle() const { return _angle; }
            void setAngle( double a ) { _angle = a; }
            
            const vec3_type& position() const { return _position; }
            void setPosition( const vec3_type& p ) { _position = p; }
            
        private:
            
            double               _angle; // comment
            vec3_type         _position;
            
        }; // GameOver
        
    } // model::
} // flappy_box::
