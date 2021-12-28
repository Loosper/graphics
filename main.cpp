#include <QApplication>
#include "Scene.h"

#include <QShortcut>
#include <QObject>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Scene *window = new Scene();

    // this gives me ^Q to quit
    QShortcut *shortcut = new QShortcut(QKeySequence::Quit, window);
    QObject::connect(shortcut, &QShortcut::activated, &app, &QApplication::quit);

    window->resize(1280, 720);
    window->show();

    app.exec();
    return 0;
}
