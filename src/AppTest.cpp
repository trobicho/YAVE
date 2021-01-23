#include "AppTest.h"

AppTest::AppTest(width, height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZEABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(width, height, "Yave test", nullptr, nullptr);
}

AppTest::~AppTest()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void	AppTest::run()
{
	main_loop();
}

void	AppTest::main_loop()
{
	while(!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
	}
}
