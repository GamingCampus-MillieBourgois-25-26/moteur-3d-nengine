#include <Engine/Application.h>	

int main()
{
	Engine::Application app;

	app.Init();

	while (app.getIsRunning() == true) 	{
		app.Running();
	}

	app.Shutdown();
	return 0;
}