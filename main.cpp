#include "gui/imagescaling.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Images imageView;
    imageView.show();

    return a.exec();
}
