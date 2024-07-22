// AdventureEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Application.h"

int main()
{
    Application MyApp;
    
    if (MyApp.startup())
    {
        while (!MyApp.shutdownState())
        {
            MyApp.update();
            MyApp.draw();
        }
        MyApp.shutdown();
    }
}

