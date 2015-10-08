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

#include "maths_3d.h"



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




/************************************************************************\
*                                                                        *
*  Renvoie la position de l'intersection de la ligne et du plan          *
*                                                                        *
\************************************************************************
point get_intersection(const point &a, const point &b, const plan &pl)
{
  point p(a.x - b.x, a.y - b.y, a.z - b.z);

  float d = produit_scalaire(pl.normal,p);

  if (fabsf(d)<0.00002f) // à revoir les limites des float
    {
      //Beep(1000,200);
      return p;
    }

  float t = -( pl.d + produit_scalaire(pl.normal,b) ) / d;

  return point(b.x+t*p.x, b.y+t*p.y, b.z+t*p.z);
}


/************************************************************************\
*                                                                        *
*  construit un plan à partir de 3 points, les points doivent être       *
*  donnés dans le sens horaire pour avoir une normale qui pointe vers    *
*  vous [ qui ne fuit pas donc ;-) ]                                     *
*                                                                        *
\************************************************************************
plan make_plan_from_points(const point &a, const point &b, const point &c)
{
  plan  pl;
  const point& v1 = a - b;
  const point& v2 = c - b;

  // la normale
  pl.normal = produit_vectoriel(v1, v2);
  norme(pl.normal);

  // le décalage
  pl.d = - produit_scalaire(pl.normal, a);

  return pl;
}
*/

