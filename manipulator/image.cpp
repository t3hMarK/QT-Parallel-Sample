#include "image.h"

Image::Image(int* aRgbBuffer, const int width, const int height) {
    this->aRgbBuffer = aRgbBuffer;
    this->width = width;
    this->height = height;
}

Image::~Image() {
    delete aRgbBuffer;
}
