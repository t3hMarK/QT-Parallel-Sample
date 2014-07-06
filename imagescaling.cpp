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
** DATA, OR PROFITS; OR BUSINESS unsigned intERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "imagescaling.h"
#include "math.h"

const float SCALING_FACTOR = 0.5f;

/**
 * @brief Images::Images
 * @param parent
 */
Images::Images(QWidget *parent) : QWidget(parent) {

    setWindowTitle(tr("Image loading and scaling example"));
    resize(800, 600);

    imageScaling = new QFutureWatcher<QVector<scalingDto> >(this);
    connect(imageScaling, SIGNAL(finished()), SLOT(showImage()));
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

QVector<scalingDto> map(const QImage &image, const float &scalingFactor) {

    QVector<scalingDto> dtoVector;

    unsigned int np = QThreadPool::globalInstance()->maxThreadCount();
    unsigned int extraWidthForLast = image.size().width() % np;

    unsigned int npCount = 0;
    while(npCount < np) {

        unsigned int h = image.size().height();
        unsigned int w = image.size().width()/np;
        unsigned int offset = npCount * w;

        if(npCount + 1 >= np) {
            w = image.size().width()/np + extraWidthForLast;
        }

        unsigned int* imgArray = new unsigned int[w * h];

        for(unsigned int i = 0; i < h; i++){
            for(unsigned int j = 0; j < w; j++){              
                imgArray[w * i + j] = image.pixel(j + offset, i);
            }
        }

        scalingDto dto;
        dto.imgArray = imgArray;
        dto.size = QSize(w,h);
        dto.scalingFactor = scalingFactor;

        dtoVector.push_back(dto);

        npCount++;
    }

    return dtoVector;
}

scalingDto scale(const scalingDto &dto) {
    /*
     *  Algorithm taken from java bilinear unsigned interpolation scaling
     *  http://tech-algorithm.com/articles/bilinear-image-scaling/
     */
    unsigned int w = dto.size.width();
    unsigned int w2 = dto.size.width() * dto.scalingFactor;
    unsigned int h = dto.size.height();
    unsigned int h2 = dto.size.height() * dto.scalingFactor;

    unsigned int* temp = new unsigned int[w2 * h2] ;

    unsigned int a, b, c, d, x, y, index ;
    float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff, blue, red, green ;

    unsigned int offset = 0 ;
    for (unsigned int i=0;i<h2;i++) {
        for (unsigned int j=0;j<w2;j++) {

            x = (unsigned int)(x_ratio * j) ;
            y = (unsigned int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;

            index = (y*w+x) ;
            a = dto.imgArray[index] ;
            b = dto.imgArray[index+1] ;
            c = dto.imgArray[index+w] ;
            d = dto.imgArray[index+w+1] ;

            // blue element
            // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
            blue = (a&0xff)*(1-x_diff)*(1-y_diff) +
                   (b&0xff)*(x_diff)*(1-y_diff) +
                   (c&0xff)*(y_diff)*(1-x_diff) +
                   (d&0xff)*(x_diff*y_diff);

            // green element
            // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
            green = ((a>>8)&0xff)*(1-x_diff)*(1-y_diff) +
                    ((b>>8)&0xff)*(x_diff)*(1-y_diff) +
                    ((c>>8)&0xff)*(y_diff)*(1-x_diff) +
                    ((d>>8)&0xff)*(x_diff*y_diff);

            // red element
            // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
            red = ((a>>16)&0xff)*(1-x_diff)*(1-y_diff) +
                  ((b>>16)&0xff)*(x_diff)*(1-y_diff) +
                  ((c>>16)&0xff)*(y_diff)*(1-x_diff) +
                  ((d>>16)&0xff)*(x_diff*y_diff);

            temp[offset++] = 0xff000000 | ((((unsigned int)red)<<16)&0xff0000) | ((((unsigned int)green)<<8)&0xff00) | ((unsigned int)blue) ;
        }
    }

    delete dto.imgArray;
    scalingDto scaledImageDto;
    scaledImageDto.imgArray = temp;
    scaledImageDto.size = QSize(w2, h2);
    scaledImageDto.scalingFactor = dto.scalingFactor;

    return scaledImageDto;
}

void assemble(QVector<scalingDto> &result, const scalingDto &imageFragment) {
    result.push_back(imageFragment);
}


void Images::open() {

    // Cancel and wait if we are already loading images.
    if (imageScaling->isRunning()) {
        imageScaling->cancel();
        imageScaling->waitForFinished();
    }

    // Show a file open dialog at QStandardPaths::PicturesLocation.
    QString file = QFileDialog::getOpenFileName(this, tr("Select Images"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "*.jpg *.png");
    if (file.size() == 0) {
        return;
    }
    QImage image = QImage(file);

    // Do a simple layout.
    qDeleteAll(labels);
    labels.clear();

    timer.start();
    unsigned int* imgArray = new unsigned int[image.size().width() * image.size().height()];
    for(int i = 0; i < image.size().height(); i++){
        for(int j = 0; j < image.size().width(); j++){
            imgArray[image.size().width() * i + j] = image.pixel(j , i);
        }
    }
    scalingDto dto;
    dto.imgArray = imgArray;
    dto.size = image.size();
    dto.scalingFactor = SCALING_FACTOR;
    scalingDto scaledDto = scale(dto);
    singleThreadTime = timer.elapsed();
    qDebug() << "SingleThread " << singleThreadTime;
    delete scaledDto.imgArray;

    // Use mapped to run the thread safe scale function on the files.
    timer.start();
    imageScaling->setFuture(QtConcurrent::mappedReduced(map(image, SCALING_FACTOR), scale, assemble, QtConcurrent::OrderedReduce));

    openButton->setEnabled(false);
    cancelButton->setEnabled(true);
    pauseButton->setEnabled(true);
}

void Images::showImage() {

    concurrentTime = timer.elapsed();
    qDebug() << "MapReduce " << concurrentTime;
    qDebug() << "speedup of " << (float)singleThreadTime / (float)concurrentTime;

    //construct QImage from results
    QVector<scalingDto> imageFragments = imageScaling->result();

    //find out size of combined image
    unsigned int width = 0, height = 0;
    foreach (scalingDto dto, imageFragments) {
        width += dto.size.width();
        height = dto.size.height();
    }

    //combine image by adding each individual width line to the combined array
    unsigned int* combinedImage = new unsigned int[width * height];
    for(unsigned int h = 0; h < height; h++) {
        unsigned int offset = 0;
        foreach(scalingDto dto, imageFragments) {
            for(int w = 0; w < dto.size.width(); w++) {
                combinedImage[h * width + offset + w] = dto.imgArray[h * dto.size.width() + w];
            }
            offset += dto.size.width();
        }
    }

    //clean up Qvector
    foreach(scalingDto dto, imageFragments) {
        delete dto.imgArray;
    }

    resize(width, height);
    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(width,height);
    imagesLayout->addWidget(imageLabel,0,0);
    imageLabel->setPixmap(QPixmap::fromImage(QImage((uchar*)combinedImage, width, height, QImage::Format_RGB32)));
    labels.append(imageLabel);
}

void Images::finished()
{
    openButton->setEnabled(true);
    cancelButton->setEnabled(false);
    pauseButton->setEnabled(false);
}


