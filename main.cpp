#include "gui/imagescaling.h"
#include <QApplication>
#include "gui/manipulationform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ManipulationForm imageView;
    imageView.show();

    return a.exec();
}
