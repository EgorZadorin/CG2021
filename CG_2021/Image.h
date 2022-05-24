#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int tileSize = 32;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

//Pixel mix(Pixel &prev, Pixel &next) {
//  next.r = next.a / 255.0 * (next.r - prev.r) + prev.r;
//  next.g = next.a / 255.0 * (next.g - prev.g) + prev.g;
//  next.b = next.a / 255.0 * (next.b - prev.b) + prev.b;
//}

constexpr Pixel backgroundColor{27, 38, 41, 255};

struct Image
{
  Image() {};
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);
  Image& operator=(const Image &image);
  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return  data; }

  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void PutPixel(int x, int y, const Pixel &pix) { data[width* y + x] = pix; }
  void DrawPixel(int x, int y, const Pixel &pix);
  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
