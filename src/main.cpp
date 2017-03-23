#include "../inc/graphics/graphics.hpp"
#include "../inc/models/world.hpp"
#include "../inc/models/hares.hpp"
#include <iostream>
#include <cstdio>

int main()
{

    // create the window
    SFMLManager manager("Wolves, hares and grass");
    World world;
    world.readIslands();
    world.generate();
    world.spawnGrass();
    
    manager.drawWorld(world);

    // run the main loop
    while (manager.windowIsOpen())
    {
        // handle events
        if (!manager.checkCloseEvent()) {
            world.tick();
            
            manager.drawWorld(world);

            // draw it
            manager.render();
        }
    }

    return 0;
}