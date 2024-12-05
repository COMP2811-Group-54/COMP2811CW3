#include <iostream>
#include <QtWidgets>
#include "Dashboard.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QTranslator translator;

  if (translator.load("languages/en_GB.qm")) {
    std::cout << "loaded" << std::endl;
    QApplication::installTranslator(&translator);
  }

  Dashboard window;
  window.show();

  return QApplication::exec();
}