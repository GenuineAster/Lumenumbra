#include "Game/Game.hpp"
#include <SFML/Graphics.hpp>

/* Good morning Humans!
 * Most coding will be done in Game/Game.*
 * Thanks for watching!
*/

int main(int argc, char** argv)
{
    Game{argc, argv, {800,600}}.run();
    return 0; // <3
}