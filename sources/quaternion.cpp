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
#  this class is a quaternion                                                                      #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */



#include "quaternion.h"


/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/






/**************************************************************************\
*                                                                          *
*  initialisation                                                          *
*                                                                          *
\**************************************************************************/
quaternion::quaternion()
{
  s   = 0.0f;
  v.x = 0.0f;
  v.y = 0.0f;
  v.z = 0.0f;
}


/**************************************************************************\
*                                                                          *
*  Crée un quaternion à partir d'un autre                                  *
*                                                                          *
\**************************************************************************/
quaternion::quaternion(const quaternion& q)
{
  s   = q.s;
  v.x = q.v.x;
  v.y = q.v.y;
  v.z = q.v.z;
}


/**************************************************************************\
*                                                                          *
*  Crée un quaternion à partir d'un point                                  *
*                                                                          *
\**************************************************************************/
quaternion::quaternion(const point& p, float S=0.0f)
{
  s   = S;
  v.x = p.x;
  v.y = p.y;
  v.z = p.z;
}


/**************************************************************************\
*                                                                          *
*  Crée un quaternion à partir de flottants                                *
*                                                                          *
\**************************************************************************/
quaternion::quaternion(float X, float Y, float Z, float S)
{
  s   = S;
  v.x = X;
  v.y = Y;
  v.z = Z;
}


/**************************************************************************\
*                                                                          *
*                                                                          *
*                                                                          *
\**************************************************************************/
void quaternion::setup_rotation(const point& vecteur, float angle)
{
  float t = (angle*3.1415927f)/360.0f;
  float n = sinf(t);

  s   = cosf(t);
  v.x = vecteur.x * n;
  v.y = vecteur.y * n;
  v.z = vecteur.z * n;
}


/**************************************************************************\
*                                                                          *
*  opérateur de recopie                                                    *
*                                                                          *
\**************************************************************************/
quaternion& quaternion::operator=(const quaternion& q)
{
  s   = q.s;
  v.x = q.v.x;
  v.y = q.v.y;
  v.z = q.v.z;

  return *this;
}


/**************************************************************************\
*                                                                          *
*  opérateur d'addition                                                    *
*                                                                          *
\**************************************************************************/
quaternion quaternion::operator+(const quaternion& q)
{
  return quaternion(v.x+q.v.x, v.y+q.v.y, v.z+q.v.z, s+q.s);
}


/**************************************************************************\
*                                                                          *
*  opérateur de soustraction                                               *
*                                                                          *
\**************************************************************************/
quaternion quaternion::operator-(const quaternion& q)
{
  return quaternion(v.x-q.v.x, v.y-q.v.y, v.z-q.v.z, s-q.s);
}


/**************************************************************************\
*                                                                          *
*  opérateur de multiplication                                             *
*                                                                          *
\**************************************************************************/
quaternion quaternion::operator*(const quaternion& q)
{
  float &S = s;
  float &X = v.x;
  float &Y = v.y;
  float &Z = v.z;
  
  float Qs = S * q.s - ( X * q.v.x + Y * q.v.y + Z * q.v.z );
  float Qx = (S * q.v.x + q.s * X ) + ( Y * q.v.z - Z * q.v.y );
  float Qy = (S * q.v.y + q.s * Y ) + ( Z * q.v.x - X * q.v.z );
  float Qz = (S * q.v.z + q.s * Z ) + ( X * q.v.y - Y * q.v.x );

  return quaternion(Qx,Qy,Qz,Qs);
}


/**************************************************************************\
*                                                                          *
*  opérateur de multiplication                                             *
*                                                                          *
\**************************************************************************/
quaternion quaternion::operator*(const float f)
{
  return quaternion(v.x*f, v.y*f, v.z*f, s*f);
}


/**************************************************************************\
*                                                                          *
*  renvoie le quaternion inversé (ne modifie le quaternion "appelant")     *
*                                                                          *
\**************************************************************************/
quaternion quaternion::inv() const
{
  return quaternion(-v.x, -v.y, -v.z, s);
}



/**************************************************************************\
*                                                                          *
*   rotate a point around 3 axis                                           *
*                                                                          *
\**************************************************************************/
point quaternion_rotation( const point& p,
                           const point& vectX,const point& vectY,const point& vectZ,
                           float x, float y, float z )
{
  quaternion q1,q2,q3,P(p);

  q1.setup_rotation(vectX,x);
  q2.setup_rotation(vectY,y);
  q3.setup_rotation(vectZ,z);

  quaternion tmp  = q1 * P    * q1.inv();
  quaternion tmp2 = q2 * tmp  * q2.inv();

  return (q3 * tmp2 * q3.inv()).v;
}


/**************************************************************************\
*                                                                          *
*   rotate a point around the axis VectRef                                 *
*                                                                          *
\**************************************************************************/
point quaternion_rotation(const point& p, const point& vectRef, float angle)
{
  quaternion q,P(p);

  q.setup_rotation(vectRef,angle);

  return (q * P * q.inv()).v;
}


/**************************************************************************\
*                                                                          *
*  Spherical Linear interpolation (les quaternions doivent être unités !)  *
*                                                                          *
*  Interpole linéairement 2 quaternions de *this à q, t appartient [0,1]   *
*                                                                          *
\**************************************************************************
quaternion quaternion::slerp(float t, const quaternion& q)
{
  quaternion &p = *this;

  float Cos = 2.0f * (v.x * q.v.x + v.y * q.v.y + v.z * q.v.z + s * q.s);
  
  if      (Cos < -1.0f) Cos = -1.0f;
  else if (Cos >  1.0f) Cos =  1.0f;

  float angle = acosf(Cos);
  float Sin   = sinf(angle);

  if (Sin < 1.e-4f) return p; // Sin proche de 0 (pb avec la division)
  else
    {
      float coeff0 = sinf( (1-t) * angle ) / Sin;
      float coeff1 = sinf(    t  * angle ) / Sin;
      
      return ((quaternion)p * coeff0) + ((quaternion)q * coeff1);
    }

}

*/

