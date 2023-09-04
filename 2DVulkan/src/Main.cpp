#include <memory>
#include <iostream>

#include "Engine.h"


//This should roughly be in the User Project not Engine, the Engine is the source of code not the start

int main() {
    EngineStartFlags flags = {};
    flags.windowSize = { 980 * 1.5, 720 * 1.5 };

    Engine engine{ flags };
    
    EngineData engineData = engine.GetEngineData();

   /* std::shared_ptr<EngineObjects> gameObjects = engineData.gameObjects;

    EngineObject square = {};
    square.position = { 0,0 };
    square.mesh = ObjectMesh({ {1, 1}, {1, -1}, {-1,-1}, {-1, 1} }, {0, 1, 2, 1, 2, 3});
    square.color = { 0.1f, 0.54f, 0.88f };*/

    int counter = 0;

    std::cout << "STARTING!" << std::endl;

    while (engine.DidEngineEnd())
    {
        engine.Update();
    }

    std::cout << "ENDING!" << std::endl;
}