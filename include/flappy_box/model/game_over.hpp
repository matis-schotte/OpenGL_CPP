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
            
            GameOver( const int p, const std::string& = "GameOver" );
            
            const int& playerPoints() const { return _playerPoints; }
            // set-methode schlägt fehl, da const int -> unveränderbarer wert
            
            const double& fadingColor() const { return _fadingColor; }
            void setFadingColor( const double& r ) { _fadingColor = r; }
            
        private:
            
            const int                  _playerPoints;
            double                      _fadingColor;
            
        }; // GameOver
        
    } // model::
} // flappy_box::
