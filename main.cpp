#include "Window.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window::instance()->showMaximized();

    return a.exec();
}

