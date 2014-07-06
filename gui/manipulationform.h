#ifndef MANIPULATIONFORM_H
#define MANIPULATIONFORM_H

#include <QtWidgets>
#include <QImage>
#include <QLabel>

namespace Ui {
    class ManipulationForm;
}

class ManipulationForm : public QWidget
{
        Q_OBJECT

    public:
        explicit ManipulationForm(QWidget *parent = 0);
        ~ManipulationForm();

    private slots:
        void on_openButton_clicked();

    private:
        Ui::ManipulationForm *ui;
};

#endif // MANIPULATIONFORM_H
