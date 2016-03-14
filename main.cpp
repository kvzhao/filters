#include <iostream>

#include "include/image.hpp"
#include "include/io.hpp"
#include "include/algo.hpp"

using namespace std;

void get_imgtype (const Image &img) {
   if (img.type() == ImgType::GRAY)
      cout << "Gray Image\n";
   else if (img.type() == ImgType::RGB)
      cout << "Color Image\n";
   else if (img.type() == ImgType::EMPTY)
      cout << "Empty Image\n";
   else if (img.type() == ImgType::UNKNOWN)
      cout << "Unknown type";
      
}

int main(int argc, char *argv[]) {

   if (argc != 2) {
      cout << "Usage: new prog image_path\n";
      return -1;
   }
   
   Image img = imread(argv[1]);

   Image gray;

   convert(img, gray, RGB2GRAY);

   Image grad;
   gradient(gray, grad);

   Image blur;
   GaussianFilter2D(gray, blur);

   imwrite("gradient", grad);
   imwrite("gaussian", blur);


   cout << "Done.\n";
   return 0;
}
