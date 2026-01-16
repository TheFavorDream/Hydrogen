#include <iostream>
#include "Application/Application.h"

int main (int argc, char** arvg)
{
	Application* App = new Application;
	App->Run();
	delete App;
    return 0;
}