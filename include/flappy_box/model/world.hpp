# pragma once

# include "../../model/game_object.hpp"
# include "../../math.hpp"

// ATTENTION: NEW CODE!

namespace flappy_box
{
    namespace model
    {
        class World : public ::model::GameObject
        {
        public:
            
            World( const std::string& = "World" );
            
            const int& playerPoints() const { return _playerPoints; }
            void setPlayerPoints( int p ) { _playerPoints = p; }
            
            const int& remainingLives() const { return _remainingLives; }
            void setRemainingLives( const int& r ) { _remainingLives = r; }
            
            const bool& shouldRestart() const { return _shouldRestart; }
            void setShouldRestart( const bool& r ) { _shouldRestart = r; }
            
            const double getWorldHalfHeight() const { return 30.0; }; /* gibt die halbe Ausdehnung der Spielwelt in z-Richtung zurück */
            const double getWorldHalfWidth() const { return 42.0; }; /* gibt die halbe Ausdehnung der Spielwelt in x-Richtung zurück */
            
        private:
            
            int                  _playerPoints;
            int                _remainingLives;
            bool                _shouldRestart;
            
        }; // World
        
    } // model::
} // flappy_box::
