#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H

#include <manipulator/image.h>
#include <QVector>
#include <QFutureWatcher>

class ImageManipulator
{
    public:
        ImageManipulator();

    protected:
        /**
         * @brief mapManipulation, Generic template method used to divide and map it to processing units
         * @param image, Image mapped
         * @return QVector with each divided image
         */
        QVector<Image> mapManipulation(const Image &image);

        /**
         * @brief manipulateImage, Pure virtual method to be implemented by subclasses, specific work may vary
         * @param image
         * @return
         */
        Image manipulateImage(const Image &image) = 0;

        /**
         * @brief reduceManipulation, reduce the result of each manipulation into a single image
         * @param result, resulting Image
         * @param resultFragment, fragment of the result to be added to the result
         */
        void reduceManipulation(Image &result,const Image &resultFragment);

    private:
        QFutureWatcher<Image> *manipulator;
};

#endif // IMAGEMANIPULATOR_H
