#include "engine.h"

#include <iostream>
#include <filesystem>


int main()
{
    Engine engine = Engine();
    engine.running = true;
    engine.Run();
}
