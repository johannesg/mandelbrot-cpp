#pragma once
#include <complex>
typedef std::complex<double> Point;

class Mandlebrot {
   public:
    Mandlebrot(Point topLeft, Point bottomRight, int bufferWidth,
             int bufferHeight);

    Point getCoord(int x, int y);
    int calcDot(Point c); 

   private:
    Point topLeft;
    Point bottomRight;
    Point scale;
//    int width;
//    int height;
};
