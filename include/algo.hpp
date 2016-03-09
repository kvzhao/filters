#ifndef ALGO_H_
#define ALGO_H_

#include "image.hpp"

enum Code {RGB2GRAY, GRAY2RGB};
void convert(const Image &src, Image &dest, Code c);

void gradient(const Image &in, Image &out);

// 2D Gaussian Filter with 
void GaussainFilter(const Image &in, Image &out);

#endif // ALGO_H_
