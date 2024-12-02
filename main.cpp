#include <QtWidgets>
#include "POPs.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  PersistentOrganicPollutants window;
  window.show();

  return app.exec();
}