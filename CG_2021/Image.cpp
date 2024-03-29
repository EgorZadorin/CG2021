#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height]{};
  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

Image& Image::operator=(const Image &image) 
{
  width = image.width;
  height = image.height;
  channels = image.channels;
  size = image.size;
  self_allocated = image.self_allocated;
  data = new Pixel[size];
  for (int i = 0; i < size; ++i) 
    data[i] = image.data[i];
  return *this;
}

int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

void Image::DrawPixel(int x, int y, const Pixel &pix)
{
  data[width * (height - y - 1) + x].r = data[width * (height - y - 1) + x].r * (1 - pix.a / 255) + pix.r * pix.a / 255;
  data[width * (height - y - 1) + x].g = data[width * (height - y - 1) + x].g * (1 - pix.a / 255) + pix.g * pix.a / 255;
  data[width * (height - y - 1) + x].b = data[width * (height - y - 1) + x].b * (1 - pix.a / 255) + pix.b * pix.a / 255;
}

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}
