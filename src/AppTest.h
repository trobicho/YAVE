#include "core/YAVE.h"

class AppTest
{
	public:
		AppTest(){};
		~AppTest(){};

		void	run();

	private:
		void	main_loop();

		GLFWwindow	*m_window;
}
