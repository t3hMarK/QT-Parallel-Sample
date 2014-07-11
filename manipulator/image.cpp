#include "image.h"

Image::Image(unsigned int* aRgbBuffer, const unsigned int width, const unsigned int height) {
    this->aRgbBuffer = aRgbBuffer;
    this->width = width;
    this->height = height;
}

Image::~Image() {
    delete aRgbBuffer;
}

unsigned int Image::getAlpha(unsigned int position) {
    if(position > width * height) {
        //TODO : Exception Handling for buffer overflow
    }
    return (aRgbBuffer[position] >> 24) & 0xFF;
}

unsigned int Image::getAlpha(unsigned int x, unsigned int y) {
    return getAlpha(x + (width * y));
}

unsigned int Image::getRed(unsigned int position) {
    if(position > width * height) {
        //TODO : Exception Handling for buffer overflow
    }
    return (aRgbBuffer[position] >> 16) & 0xFF;
}

unsigned int Image::getRed(unsigned int x, unsigned int y) {
    return getRed(x + (width * y));
}

unsigned int Image::getGreen(unsigned int position) {
    if(position > width * height) {
        //TODO : Exception Handling for buffer overflow
    }
    return (aRgbBuffer[position] >> 8) & 0xFF;
}

unsigned int Image::getGreen(unsigned int x, unsigned int y) {
    return getGreen(x + (width * y));
}

unsigned int Image::getBlue(unsigned int position) {
    if(position > width * height) {
        //TODO : Exception Handling for buffer overflow
    }
    return (aRgbBuffer[position]) & 0xFF;
}

unsigned int Image::getBlue(unsigned int x, unsigned int y) {
    return getBlue(x + (width * y));
}
