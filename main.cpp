#include <QtWidgets>
#include "PFAs.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  PFApage window;
  window.show();

  return app.exec();
}