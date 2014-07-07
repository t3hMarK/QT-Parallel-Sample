#ifndef IMAGE_H
#define IMAGE_H

class Image
{
    public:
        Image(int* aRgbBuffer, int width, int height);
        ~Image();
        const int* aRgbBuffer;
        int width;
        int height;
};

#endif // IMAGE_H
