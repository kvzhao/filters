#include "include/io.hpp"
#include "include/image.hpp"
#include <fstream>
#include <iostream>

Image imread(const std::string &path) {
   std::ifstream ifs;
   ifs.open(path.c_str(), std::ios::binary);

   try {
      if (ifs.fail()) 
         throw("File can not be opened!");
      std::string header;
      size_t w, h, b; // or int
      size_t ch;

      ifs >> header;
      if (header == "P6")
         ch = 3; // color map
      else if (header == "P5")
         ch = 1; // gray scale
      else 
         throw("P5 or P6 cannot be found!");

      ifs >> w >> h >> b;
      // allocate image memory and assign
      ifs.ignore(256, '\n');
      Image img;
      img._channels = ch;
      img._width = w;
      img._height = h;

      img.pixels = new float* [ch];
      for (int c = 0; c < ch; ++c)
         img.pixels[c] = new float [w * h];

      unsigned char p[3];
      for (int i = 0; i < img.dim() ; ++i) {
         ifs.read(reinterpret_cast<char *>(p), ch);
         for (int c = 0; c < ch; ++c) {
            img.pixels[c][i] = p[c] / 255.f;
         }
      }
      ifs.close();
      return img;

   } catch (const char *err) {
      std::cout << err << std::endl;
      ifs.close();
   }

};

void imwrite(const std::string &name, const Image &img) {
   if (img.empty()) 
      throw ("Empty image can not be saved!");
   std::ofstream ofs;
   std::string postfix;
   try {
      unsigned char px[3];
      if (img._channels == 3) {
         postfix = ".ppm";
      }
      else if (img._channels == 1) {
         postfix = ".pbm";
      }

      std::string filename = name + postfix;
      ofs.open(filename.c_str(), std::ios::binary);
      if (ofs.fail())
         throw("Can not open output file");

      if (img._channels == 3) 
         ofs << "P6\n";
      else if (img._channels == 1)
         ofs << "P5\n";
      ofs << img._width << " " << img._height << "\n255\n";

      for (int i =0; i < img._width * img._height; ++i) {
         if (img._channels == 3) {
            px[0] = static_cast<unsigned char> (std::min(1.f, img.pixels[0][i]) *255);
            px[1] = static_cast<unsigned char> (std::min(1.f, img.pixels[1][i]) *255);
            px[2] = static_cast<unsigned char> (std::min(1.f, img.pixels[2][i]) *255);
            ofs << px[0] << px[1] << px[2];
         } else if (img._channels == 1) {
            px[0] = static_cast<unsigned char> (std::min(1.f, img.pixels[0][i]) *255);
            ofs << px[0];
         }
      }
      ofs.close();
   } catch (const char *err) {
      std::cout << err << std::endl;
      ofs.close();
   }
}
