#include <iostream>
#include "Context.h"

int main()
{
  //create a window for the renderer
  Epsilon::Context context(800, 600);
  Epsilon::Context context2(400, 600);

  //run the window
  context.Run();

  return 0;
}
