#ifndef IMAGE_H_
#define IMAGE_H_
#include <string>
#include "io.hpp"

enum RGB {R = 0, G = 1, B = 2};
enum class ImgType {UNKNOWN = -1, EMPTY = 0, GRAY = 1, RGB = 3};

class Image {
   public:
      Image();
      Image(const Image &img);
      Image(size_t w, size_t h, size_t ch, float **ptr);
      ~Image();

      // Support I/O functions
      friend Image imread(const std::string &path);
      friend void imwrite(const std::string &name, const Image& img);

      // Operator Overloaded
      float operator [] (const unsigned int &i); // heavy function
      Image& operator=(const Image &rhs);

      // Utility functions
      const inline size_t width() const {return _width;};
      const inline size_t height() const {return _height;};
      const inline size_t dim() const {return _width * _height;}
      const inline size_t len() const {return _width * _height * _channels;}
      const inline ImgType type() const {return _type;}
      inline bool empty() const 
         { return (_width ==0 || _height == 0 || _channels == 0 || pixels == NULL) ? true : false;};
      inline bool is_gray() const 
         { return _channels == 3 ? false : true;};
      // Dangerous
      inline float ** ptr() {return pixels;};

      // Increase or decrease the dim of Image
      // Used in ?
      void reshape(size_t w, size_t h, size_t ch);
      inline void clear() {this->~Image();};

   private: 
      size_t _width;
      size_t _height;
      size_t _channels;
      float **pixels;
      ImgType _type;
};

#endif // IMAGE_H_
