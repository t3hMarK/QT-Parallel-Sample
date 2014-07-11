#include "ImageQImageConverter.h"

ImageQImageConverter::ImageQImageConverter() {

}

QImage* ImageQImageConverter::QImageFromImage(Image* image){
    return new QImage((uchar*)image->aRgbBuffer, image->width,image->height, QImage::Format_ARGB32);
}

Image* ImageQImageConverter::ImageFromQImage(QImage *image) {
    unsigned int width = image->size().width();
    unsigned int height = image->size().height();

    //construct buffer from QImage
    unsigned int* argbBuffer = new unsigned int[width * height];
    for(unsigned int y = 0; y < height; y++){
        for(unsigned int x = 0; x < width; x++){
            argbBuffer[x + (width * y)] = image->pixel(x, y);
        }
    }

    return new Image(argbBuffer, width, height);
}
