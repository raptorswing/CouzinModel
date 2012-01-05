#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow * w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

    return a.exec();
}
