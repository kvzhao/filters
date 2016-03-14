#include "include/algo.hpp"
#include <cassert>
#include <cstring>
#include <cmath>

#define PI 3.1415926

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


// Calculates a 1d gaussian bell shaped kernel
float* ComputeGaussianKernel(const int inRadius, const float inWeight)
{
    int mem_amount = (inRadius*2)+1;
    float* gaussian_kernel = (float*)malloc(mem_amount*sizeof(float));

    float twoRadiusSquaredRecip = 1.0 / (2.0 * inRadius * inRadius);
    float sqrtTwoPiTimesRadiusRecip = 1.0 / (sqrt(2.0 * PI) * inRadius);
    float radiusModifier = inWeight;

    // Create Gaussian Kernel
    int r = -inRadius;
    float sum = 0.0f;
    for (int i = 0; i < mem_amount; i++)
    {
        float x = r * radiusModifier;
        x *= x;
        float v = sqrtTwoPiTimesRadiusRecip * exp(-x * twoRadiusSquaredRecip);
        gaussian_kernel[i] = v;
            
        sum+=v;
        r++;
    }

    // Normalize distribution
    float div = sum;
    for (int i = 0; i < mem_amount; i++)
        gaussian_kernel[i] /= div;

    return gaussian_kernel;
}

int climp(int x, int min, int max) {
   if (x < min)
      x = min;
   else if (x > max)
      x = max;
   return x;
}

float climp(float x, float min, float max) {
   if (x < min)
      x = min;
   else if (x > max)
      x = max;
   return x;
}

void GaussianFilter2D(const Image &in, Image &out) {
   const int kernel_size = 9;
   float *gaussian_kernel = ComputeGaussianKernel(kernel_size, 1);

   Image img;

   if (in.type() != ImgType::GRAY) {
      convert(in, img, RGB2GRAY);
   } else {
      img = in;
   }

   const unsigned int mem_amount = img.dim();
   const unsigned int height = img.height();
   const unsigned int width = img.width();
   const unsigned int width_climp = img.width()-1;
   const unsigned int kernel_amount = kernel_size * 2 +1;

   float *buf = img.ptr()[0];

   out = img;
   float *dest = out.ptr()[0];

   for (int i = kernel_size ; i < width-kernel_size; ++i) {

      int col = i * height;

      for (int j = kernel_size; j < height-kernel_size; ++j) {
         float blur_val = 0.f;

         /* algorithm here */
         for (int k = 0; k < kernel_amount; ++k) {
            int s = climp((j-kernel_size) + k, 0, width_climp);
            blur_val += buf[col+s] * gaussian_kernel[k];
         }

         // Saturation condition
         blur_val = climp(blur_val, 0.f, 1.f);
         // write back to output
         dest[i * height + j] = blur_val;
      }
   }

   delete [] gaussian_kernel;
}

