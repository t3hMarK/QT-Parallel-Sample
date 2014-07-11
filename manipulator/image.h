#ifndef IMAGE_H
#define IMAGE_H

class Image
{
    public:
        unsigned int width;          //width of the image
        unsigned int height;         //height of the image
        unsigned int* aRgbBuffer;    //32bit ARGB color buffer of width * height size

        /**
         * @brief Image, Create an instance of Image
         * @param aRgbBuffer, 32bit ARGB color buffer
         * @param width, width of the image
         * @param height, height of the image
         */
        Image(unsigned int* aRgbBuffer, unsigned int width, unsigned int height);
        ~Image();

        /**
         * @brief getAlpha, get the alpha value (FF000000) of specified pixel
         * @param position, position of the pixel in ARGB buffer
         * @return Alpha numerical value
         */
        unsigned int getAlpha(unsigned int position);

        /**
         * @brief getAlpha, get the alpha value (FF000000) of specified pixel
         * @param x, x position of the pixel in ARGB buffer
         * @param y, y position of the pixel in ARGB buffer
         * @return Alpha numerical value
         */
        unsigned int getAlpha(unsigned int x, unsigned int y);

        /**
         * @brief getRed, get the red value (00FF0000) of specified pixel
         * @param position, position of the pixel in ARGB buffer
         * @return Red numerical value
         */
        unsigned int getRed(unsigned int position);

        /**
         * @brief getRed, get the alpha value (00FF0000) of specified pixel
         * @param x, x position of the pixel in ARGB buffer
         * @param y, y position of the pixel in ARGB buffer
         * @return Red numerical value
         */
        unsigned int getRed(unsigned int x, unsigned int y);

        /**
         * @brief getGreen, get the green value (0000FF00) of specified pixel
         * @param position, position of the pixel in ARGB buffer
         * @return Green numerical value
         */
        unsigned int getGreen(unsigned int position);

        /**
         * @brief getGreen, get the alpha value (0000FF00) of specified pixel
         * @param x, x position of the pixel in ARGB buffer
         * @param y, y position of the pixel in ARGB buffer
         * @return Green numerical value
         */
        unsigned int getGreen(unsigned int x, unsigned int y);

        /**
         * @brief getBlue, get the blue value (000000FF) of specified pixel
         * @param position, position of the pixel in ARGB buffer
         * @return Blue numerical value
         */
        unsigned int getBlue(unsigned int position);

        /**
         * @brief getBlue, get the alpha value (000000FF) of specified pixel
         * @param x, x position of the pixel in ARGB buffer
         * @param y, y position of the pixel in ARGB buffer
         * @return Blue numerical value
         */
        unsigned int getBlue(unsigned int x, unsigned int y);
};

#endif // IMAGE_H
