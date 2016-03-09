#ifndef IO_H_
#define IO_H_

#include <iostream>
#include <string>
#include "image.hpp"

class Image; // predeclare class name
Image imread(const std::string &path);
void imwrite(const std::string &path, const Image &img);

#endif // IO_H_
