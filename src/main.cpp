#include <iostream>
#include "App.h"

enum class test
{
    one = 1,
    two = 2,
    three
};
int main()
{
  std::cout << (int) test::three << std::endl;

  //create a window for the renderer
  Epsilon::App context(800, 600);

  //run the window
  context.Run();

  return 0;
}
