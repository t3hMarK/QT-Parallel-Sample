/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "imagescaling.h"
#include "math.h"

const int imageSize = 800;

/**
 * @brief Images::Images
 * @param parent
 */
Images::Images(QWidget *parent) : QWidget(parent) {

    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    imageScaling = new QFutureWatcher<QImage>(this);
    connect(imageScaling, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(imageScaling, SIGNAL(finished()), SLOT(finished()));

    openButton = new QPushButton(tr("Open Images"));
    connect(openButton, SIGNAL(clicked()), SLOT(open()));

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setEnabled(false);
    connect(cancelButton, SIGNAL(clicked()), imageScaling, SLOT(cancel()));

    pauseButton = new QPushButton(tr("Pause/Resume"));
    pauseButton->setEnabled(false);
    connect(pauseButton, SIGNAL(clicked()), imageScaling, SLOT(togglePaused()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addStretch();

    imagesLayout = new QGridLayout();

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

Images::~Images() {
    imageScaling->cancel();
    imageScaling->waitForFinished();
}

QImage scale(const QString &imageFileName)
{
    QImage image(imageFileName);

    /*
        public int[] resizeBilinear(int[] pixels, int w, int h, int w2, int h2) {
            int[] temp = new int[w2*h2] ;
            int a, b, c, d, x, y, index ;
            float x_ratio = ((float)(w-1))/w2 ;
            float y_ratio = ((float)(h-1))/h2 ;
            float x_diff, y_diff, blue, red, green ;
            int offset = 0 ;
            for (int i=0;i<h2;i++) {
                for (int j=0;j<w2;j++) {
                    x = (int)(x_ratio * j) ;
                    y = (int)(y_ratio * i) ;
                    x_diff = (x_ratio * j) - x ;
                    y_diff = (y_ratio * i) - y ;
                    index = (y*w+x) ;
                    a = pixels[index] ;
                    b = pixels[index+1] ;
                    c = pixels[index+w] ;
                    d = pixels[index+w+1] ;

                    // blue element
                    // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
                    blue = (a&0xff)*(1-x_diff)*(1-y_diff) + (b&0xff)*(x_diff)*(1-y_diff) +
                           (c&0xff)*(y_diff)*(1-x_diff)   + (d&0xff)*(x_diff*y_diff);

                    // green element
                    // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
                    green = ((a>>8)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>8)&0xff)*(x_diff)*(1-y_diff) +
                            ((c>>8)&0xff)*(y_diff)*(1-x_diff)   + ((d>>8)&0xff)*(x_diff*y_diff);

                    // red element
                    // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
                    red = ((a>>16)&0xff)*(1-x_diff)*(1-y_diff) + ((b>>16)&0xff)*(x_diff)*(1-y_diff) +
                          ((c>>16)&0xff)*(y_diff)*(1-x_diff)   + ((d>>16)&0xff)*(x_diff*y_diff);

                    temp[offset++] =
                            0xff000000 | // hardcode alpha
                            ((((int)red)<<16)&0xff0000) |
                            ((((int)green)<<8)&0xff00) |
                            ((int)blue) ;
                }
            }
            return temp ;
        }
    */

    QVector<QRgb> rgbVector = image.colorTable();
    QRgb test = rgbVector.at(0);

    return image.scaled(QSize(imageSize, imageSize), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void Images::open()
{
    // Cancel and wait if we are already loading images.
    if (imageScaling->isRunning()) {
        imageScaling->cancel();
        imageScaling->waitForFinished();
    }

    // Show a file open dialog at QStandardPaths::PicturesLocation.
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
                            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                            "*.jpg *.png");

    if (files.count() == 0)
        return;

    // Do a simple layout.
    qDeleteAll(labels);
    labels.clear();

    int dim = sqrt(qreal(files.count())) + 1;
    for (int i = 0; i < dim; ++i) {
        for (int j = 0; j < dim; ++j) {
            QLabel *imageLabel = new QLabel;
            imageLabel->setFixedSize(imageSize,imageSize);
            imagesLayout->addWidget(imageLabel,i,j);
            labels.append(imageLabel);
        }
    }

    // Use mapped to run the thread safe scale function on the files.
    imageScaling->setFuture(QtConcurrent::mapped(files, scale));

    openButton->setEnabled(false);
    cancelButton->setEnabled(true);
    pauseButton->setEnabled(true);
}

void Images::showImage(int num)
{
    labels[num]->setPixmap(QPixmap::fromImage(imageScaling->resultAt(num)));
}

void Images::finished()
{
    openButton->setEnabled(true);
    cancelButton->setEnabled(false);
    pauseButton->setEnabled(false);
}


