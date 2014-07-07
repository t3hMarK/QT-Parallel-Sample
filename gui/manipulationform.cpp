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
    //clean scroll area content if any
    if(ui->imageScrollAreaWidgetContents) {
        qDeleteAll(ui->imageScrollAreaWidgetContents->children());
    }

    //open image file
    QDesktopWidget desktop;
    QString file = QFileDialog::getOpenFileName(this, tr("Select Images"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "*.jpg *.png");
    if (file.size() == 0) {
        return;
    }

    //create image
    QImage image = QImage(file);
    int width = image.size().width();
    int height = image.size().height();

    //show image
    QLabel* imageLabel = new QLabel();
    imageLabel->setFixedSize(width,height);
    imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageScrollArea->setWidget(imageLabel);

    //resize widget to image or available geometry
    resize((width < desktop.availableGeometry().width() - 10) ? width : desktop.availableGeometry().width(),
           (height <desktop.availableGeometry().height() - 10) ? height : desktop.availableGeometry().height());
}

void ManipulationForm::on_CancelButton_clicked()
{

}

void ManipulationForm::on_startButton_clicked()
{

}

void ManipulationForm::on_pauseResumeButton_clicked()
{

}
