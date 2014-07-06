#include "manipulationform.h"
#include "ui_manipulationform.h"

ManipulationForm::ManipulationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManipulationForm)
{
    ui->setupUi(this);
}

ManipulationForm::~ManipulationForm()
{
    delete ui;
}

void ManipulationForm::on_openButton_clicked()
{
    QDesktopWidget desktop;
    QString file = QFileDialog::getOpenFileName(this, tr("Select Images"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "*.jpg *.png");
    if (file.size() == 0) {
        return;
    }

    QImage image = QImage(file);
    int width = image.size().width();
    int height = image.size().height();

    QLabel* imageLabel = new QLabel();
    imageLabel->setFixedSize(width,height);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageScrollArea->setWidget(imageLabel);

    resize((width < desktop.availableGeometry().width() - 10) ? width : desktop.availableGeometry().width(),
           (height <desktop.availableGeometry().height() - 10) ? height : desktop.availableGeometry().height());
}
