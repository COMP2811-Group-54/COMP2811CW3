#include <QtWidgets>
#include <QFile>
#include <stdio.h>
#include "views/Dashboard.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load the stylesheet
    if (QFile file("styles/main.qss"); file.open(QFile::ReadOnly)) {
        const QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    }

    QTranslator translator;

    if (translator.load("languages/en_GB.qm")) {
        std::cout << "loaded" << std::endl;
        QApplication::installTranslator(&translator);
    }

    Dashboard window;

    window.show();

    return QApplication::exec();
}
