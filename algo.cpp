#include "include/algo.hpp"
#include <cassert>
#include <cstring>
#include <cmath>


void convert(const Image &src, Image &dest, Code c) {
   switch (c) {
   case RGB2GRAY:
      // check type
      if (src.type() == ImgType::GRAY) {
         // trival case, deep copy
         dest = src;
      } else if (src.type() == ImgType::RGB) {
         // check size
         if (dest.empty()) {
            // empty or not same
            Image tmp = src; // Are duplications necessary?
            float **buf = tmp.ptr();
            for (int i = 0; i < tmp.dim(); ++i) {
               buf[0][i] *= 0.299f;
               buf[0][i] += buf[1][i] * .587f;
               buf[0][i] += buf[2][i] * .114f;
            }
            tmp.reshape(0, 0, 1);
            dest = tmp;
         } else {
            /* dest is not empty */
         }
      }
      break;
   case GRAY2RGB:
      /* TODO */
      break;
   }
}

/* 
 * SUPPOSE INPUT IS COLER MAP, OUTPUT IS GRAY
 * */ 
void gradient(const Image &in, Image &out) {
   // Check input is not empty
   assert(!in.empty() && "Can not process empty image!");

   Image img;
   // Check input type, only GRAY available 
   if (in.type() != ImgType::GRAY) {
      convert(in, img, RGB2GRAY);
   } else {
      img = in;
   }

   const size_t width = img.width();
   const size_t height = img.height();
   const size_t dim = img.dim();

   // Gradient Filter Kernel
   const int K = 3; // Kernel size
   const float SobelKernelKx[K][K] = {{-1.f, 0.f, 1.f}, {-2.f, 0.f, 2.f}, {-1.f, 0.f, 1.f}};
   const float SobelKernelKy[K][K] = {{-1.f, -2.f, -1.f}, {0.f, 0.f, 0.f}, {1.f, 2.f, 1.f}};

   float *buf = img.ptr()[0];
   float *gx = new float [dim];
   float *gy = new float [dim];
   // algorithm of gradient filter

   // iterate through the image
   for (int i = K/2; i < width -K/2; ++i) {
      for (int j = K/2; j < height -K/2; ++j) {

         float ptx = 0.f, pty = 0.f;
         // iterate through the kernel
         for (int ii = -K/2; ii <= K/2; ++ii) {
            for (int jj = -K/2; jj <= K/2; ++jj) {
               float data = buf[(i + ii) * height + j + jj];
               float coeff_x = SobelKernelKx[ii + K/2][jj + K/2];
               float coeff_y = SobelKernelKy[ii + K/2][jj + K/2];
               ptx += data * coeff_x;
               pty += data * coeff_y;
            }
         }
         // Saturation Condition
         if (ptx >= 1.f) ptx = 1.f;
         if (pty >= 1.f) pty = 1.f;
         if (ptx <= 0.f) ptx = 0.f;
         if (pty <= 0.f) pty = 0.f;
         // Write results back to output buffer
         gx[i * height +j] = ptx;
         gy[i * height +j] = pty;
      }
   }
   // Find magtitude
   for (int i = 0; i < dim; ++i) {
      gx[i] *= gx[i];
      gx[i] += pow(gy[i], 2);
      gx[i] = sqrt(gx[i]);
   }
   std::memcpy(buf, gx, dim * sizeof(float));
   delete [] gx; 
   delete [] gy;
   out = img;
};

