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
#  this class is a camera (a position and 2 vectors)                                               #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "type.h"
#include "point.h"
#include "quaternion.h"


class CAMERA
{
	private:

	public:

		CAMERA();

		point	position;			// camera position
		point	direction;			// camera target (it's a position)
		point	orientation;		// camera orientation (unit vector, gives the vertical)

		void	update(const point& pos, float angle_x, float angle_y, float angle_z);
};


#endif

