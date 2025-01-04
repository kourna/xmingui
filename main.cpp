
#include "components/gui.h"

int main() {

  // yep thats all thats needed to get your gui running :D

  x_gui *gui = new x_gui();
  gui->init_gui();

  // you can load a new layout for the gui at runtime using:
  // gui->load_from_file("layouts/main_layout");
  
  return 0;


}
