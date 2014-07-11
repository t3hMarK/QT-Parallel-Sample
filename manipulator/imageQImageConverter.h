#ifndef IMAGEBUILDER_H
#define IMAGEBUILDER_H

#include <QImage>
#include <manipulator/image.h>

class ImageQImageConverter
{
    public:
        /**
         * @brief ImageBuilder, Create a new instance of ImageBuilder
         */
        ImageQImageConverter();
        ~ImageQImageConverter();

        /**
         * @brief QImageFromImage, Create an instance of QImage from an Image
         * @param image, Image instance used to build QImage
         * @return valid ARGB QImage
         */
        QImage* QImageFromImage(Image* image);

        /**
         * @brief ImageFromQImage, Create an instance of Image from a QImage
         * @param Image, QImage instance used to build Image
         * @return Image instance
         */
        Image* ImageFromQImage(QImage* image);
};

#endif // IMAGEBUILDER_H
