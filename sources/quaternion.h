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

#ifndef _QUATERNION_H_
#define _QUATERNION_H_


#include <math.h>
#include "type.h"
#include "point.h"



class quaternion
{
	private:


	public:

		quaternion();
		quaternion(const quaternion&);
		quaternion(const point&, float);
		quaternion(float,float,float,float);

		quaternion&	operator=(const quaternion&);
		quaternion	operator+(const quaternion&);
		quaternion	operator-(const quaternion&);
		quaternion	operator*(const quaternion&);
		quaternion	operator*(const float f);
		quaternion	inv() const;
//		quaternion	slerp(float t, const quaternion& q);
		void		setup_rotation(const point& vecteur, float angle);

		float	s;
		point	v;

};



point quaternion_rotation(const point& p, const point& vectRef, float angle);
point quaternion_rotation(const point& p, const point& vectX, const point& vectY, const point& vectZ,
						  float x, float y, float z);




#endif


