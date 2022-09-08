#include "AppTest.h"
//#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int ac, char **av)
{
  AppTest app(800, 600);

  app.run();
  //return (EXIT_SUCCESS);
  return (0);
}
