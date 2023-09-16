#include <iostream>

#include "Game.h"

int WinMain() {

    //Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Init Game engine
    Game game;

    //Game loop
    while (game.running()) {

        //Update
        game.update();

        //Render
        game.render();
    }

    //End of application
    return EXIT_SUCCESS;
}