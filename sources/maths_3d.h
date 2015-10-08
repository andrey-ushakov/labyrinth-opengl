                                                                                                     /*
/##################################################################################################\
#                                                                                                  #
#  Author    : Thomas Laquet - © INT Evry                                                          #
#  Date      : 09-06-2006                                                                          #
#  change #1 : date/author/what                                                                    #
#  change #2 : ../../..                                                                            #
#                                                                                                  #
####################################################################################################
#                                                                                                  #
#  Maths functions                                                                                 #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#ifndef _MATHS_3D_H_
#define _MATHS_3D_H_


#include "type.h"
#include <math.h>
#include "point.h"
//#include "plan.h"


//point  get_intersection(const point &a, const point &b, const plan &pl);
//plan   make_plan_from_points(const point &a, const point &b, const point &c);


/**************************************************************************\
*                                                                          *
*  Calcule le produit scalaire de 2 points                                 *
*                                                                          *
\**************************************************************************/
inline float produit_scalaire(const point &p1,const point &p2)
{
	return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}


/**************************************************************************\
*                                                                          *
*  Calcule le produit vectoriel de 2 points                                *
*                                                                          *
\**************************************************************************/
inline point produit_vectoriel(const point &p1,const point &p2)
{
	return point( p1.y*p2.z-p1.z*p2.y , p1.z*p2.x-p1.x*p2.z , p1.x*p2.y-p1.y*p2.x );
}


/**************************************************************************\
*                                                                          *
*  norme un vecteur                                                        *
*                                                                          *
\**************************************************************************/
inline void norme(point &p)
{
	float n = sqrtf( p.x*p.x + p.y*p.y + p.z*p.z );

	p.x /= n;
	p.y /= n;
	p.z /= n;
}


/**************************************************************************\
*                                                                          *
*  donne une longueur à un vecteur                                         *
*                                                                          *
\**************************************************************************/
inline void set_norme(point &p, float len)
{
	float n = len / sqrtf( p.x*p.x + p.y*p.y + p.z*p.z );

	p.x *= n;
	p.y *= n;
	p.z *= n;
}


/**************************************************************************\
*                                                                          *
*  norme au carré d'un vecteur                                             *
*                                                                          *
\**************************************************************************/
inline float get_norme_carree(const point &p)
{
	return p.x*p.x + p.y*p.y + p.z*p.z;
}


/**************************************************************************\
*                                                                          *
*  renvoie la norme d'un vecteur                                           *
*                                                                          *
\**************************************************************************/
inline float get_norme(const point &p)
{
	return sqrtf( p.x*p.x + p.y*p.y + p.z*p.z );
}


/**************************************************************************\
*                                                                          *
*  renvoie la distance d'un point à un plan                                *
*                                                                          *
\**************************************************************************
inline float distance_point_plan(const plan &pl, const point &po)
{
	return  pl.d + produit_scalaire(pl.normal,po);
}
*/






#endif


