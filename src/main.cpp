#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "core/Logger.h"
#include "core/Application.h"
#include <iostream>

// cmake --build build --config Release

int main() {
	Logger::getInstance().openFile("engine.log");

	Application app;
	if (!app.init(800, 600, "Uzlezz Engine"))
		return -1;

	app.run();
	app.shutdown();

    return 0;
}