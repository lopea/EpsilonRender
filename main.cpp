#include <iostream>
#include "App.h"

int main()
{
  //create a window for the renderer
  Epsilon::App context(800, 600);

  //run the window
  context.Run();

  return 0;
}
