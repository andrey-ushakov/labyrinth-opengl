                                                                                                     /*
/##################################################################################################\
#                                                                                                  #
#  Author    : Thomas Laquet - © INT Evry                                                          #
#  Date      : 07-31-2006                                                                          #
#  change #1 : date/author/what                                                                    #
#  change #2 : ../../..                                                                            #
#                                                                                                  #
####################################################################################################
#                                                                                                  #
#  this class is a vertex or a vector                                                              #
#                                                                                                  #
\##################################################################################################/
                                                                                                   */
#ifndef _POINT_3D_H_
#define _POINT_3D_H_




class point
{
  private:


  public:

    
    point() : x(0), y(0), z(0) { }
    point(const point& p) : x(p.x), y(p.y), z(p.z) { }
    point(const float X, const float Y, const float Z) : x(X), y(Y), z(Z) { }
    point(const float *tab) : x(tab[0]), y(tab[1]), z(tab[2]) { }

    point  operator+ (const point& p) { return point(x+p.x, y+p.y, z+p.z); }
    point  operator- (const point& p) { return point(x-p.x, y-p.y, z-p.z); }
    point  operator* (const float  k) { return point(x*k, y*k, z*k); }
    point  operator/ (const float  k) { return point(x/k, y/k, z/k); }
    point& operator= (const point& p) { x = p.x; y = p.y; z = p.z; return *this; }
    point& operator+=(const point& p);
    point& operator-=(const point& p);
    point& operator*=(const float  f);
    point& operator/=(const float  f);

    float x;
    float y;
    float z;
};


inline const point operator+(const point& a, const point& b)
{
  return point(a.x + b.x, a.y + b.y, a.z + b.z);
}


inline const point operator-(const point& a, const point& b)
{
  return point(a.x - b.x, a.y - b.y, a.z - b.z);
}


inline const point operator*(const float f, const point& p)
{
  return point(p.x * f, p.y * f, p.z * f);
}


inline const point operator*(const point& p, const float f)
{
  return point(p.x * f, p.y * f, p.z * f);
}


#endif


