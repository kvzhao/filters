#include "include/image.hpp"
#include <string>
#include <cassert>

Image::Image() : _width(0), _height(0), _channels(0), pixels(NULL){
   _type = ImgType::EMPTY;
   // null ctor
}

Image::~Image() {
   if (!this->empty() && this->pixels != NULL) {
      for (int c = 0; c < this->_channels; ++c) {
         if (pixels[c] != NULL)
            delete [] pixels[c];
      }
      delete [] pixels;
   }
   _channels = _width = _height = 0;
   _type = ImgType::EMPTY;
}

Image::Image(size_t w, size_t h, size_t ch, float **ptr)
  : _width(w), _height(h), _channels(ch), pixels(NULL) {
     if (ptr != NULL) {
        pixels = new float *[_channels];
         for (int i = 0; i < _channels; i++) 
            pixels[i] = new float [_width * _height];
           // assign values
         for (int c = 0; _channels; c++) {
            for (int i = 0; i < this->dim(); i++) {
               pixels[c][i] = ptr[c][i];
            }
         }
           // set type
         switch (_channels) {
         case 1:
            _type = ImgType::GRAY;
            break;
         case 3:
            _type = ImgType::RGB;
            break;
         default:
            _type = ImgType::UNKNOWN;
            break;
         }
     }
}

Image::Image(const Image &img) : _width(img._width), _height(img._height), _channels(img._channels), pixels(NULL) {
   // check img is not empty
   if (!img.empty()) {
      pixels = new float* [this->_channels];
         for (int i = 0; i < this->_channels; ++i) 
            pixels[i] = new float [_width * _height];

      for (int c = 0; c < this->_channels; ++c) {
         for (int i = 0; i < this->dim(); ++i) {
            this->pixels[c][i] = img.pixels[c][i];
         }
      }
      switch (_channels) {
      case 1:
         _type = ImgType::GRAY;
         break;
      case 3:
         _type = ImgType::RGB;
         break;
      default:
         _type = ImgType::UNKNOWN;
         break;
      }
   } else {
         _type = ImgType::EMPTY;
   }
}

// Operator Overloadded: Deep Copy
Image& Image::operator=(const Image &rhs) {
   if (this == &rhs)
      return *this;
   if (!this->empty()) {
      if (this->pixels != NULL) {
         for (int c = 0; c < this->_channels; ++c) {
            if (pixels[c] != NULL)
               delete [] pixels[c];
         }
         delete [] pixels;
      }
   }
   _width = rhs._width;
   _height = rhs._height;
   _channels = rhs._channels;
   pixels = new float* [this->_channels];
      for (int i = 0; i < this->_channels; ++i) 
         pixels[i] = new float [_width * _height];
   for (int c = 0; c < _channels; ++c) {
      for (int i = 0; i < _width*_height; ++i) {
         this->pixels[c][i] = rhs.pixels[c][i];
      }
   }
   _type = rhs.type();
   return *this;
}

float Image::operator[] (const unsigned int &i) {
   switch (_type) {
   case ImgType::GRAY:
      return pixels[0][i];
      break;
   case ImgType::RGB:
            /*FIX*/
      return (0.33f*pixels[0][i] + .33f*pixels[1][i] + .33f*pixels[2][i]);
      break;
   }
}

// reshape (dim, ch) or reshape(w, h, ch)
void Image::reshape (size_t w, size_t h, size_t ch) {
   size_t dim = w * h;
   size_t len = dim * ch;
   
   // case1: != channel, delete or allocate two more, from ImgType
   if (ch != 0) {
      switch (ch) {
      case 1:
         if (_type == ImgType::RGB) {
            if (pixels[1] != NULL) delete [] pixels[1];
            if (pixels[2] != NULL) delete [] pixels[2];
         } else if (_type == ImgType::EMPTY) {
            /*TODO*/
         }
         _channels = ch;
         _type = ImgType::GRAY;
         break;
      case 3:
         if (_type == ImgType::GRAY) {
            /*TODO*/
         } else if (_type == ImgType::EMPTY) {
            /*TODO*/
         }
         _channels = ch;
         _type = ImgType::RGB;
         break;
      default:
         // illegal channel numbers, but i am fine with this.
            /*TODO*/
         _type = ImgType::UNKNOWN;
         break;
      }
   }

   // case2: != dim, totally reshape
   if (w && h) {
            /*TODO*/
      // NOT IMPLEMENT YET
   }
}
