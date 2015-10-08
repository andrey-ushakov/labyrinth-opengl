


#ifndef _LOAD_H_
#define _LOAD_H_

#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include "ogl.h"



class Sommet
{
	public:

		float x;		// position
		float y;
		float z;

		float ucol;		// coordonnées U,V de texture couleur
		float vcol;

		float nx;		// normales (normalisée)
		float ny;
		float nz;

		float r;		// couleurs [0..1]
		float v;
		float b;
		float a;

		// constructeur
		Sommet()
		{
			x = y = z = 0.0f;				// les valeurs par défaut
			ucol = vcol = 0.0f;
			nx = nz = 0.0f; ny = 1.0f;
			r = v = b = a = 1.0f;
		}

};


class Triangle
{
	public:

		int a,b,c;		// 3 index de Sommet dans un tableau de Sommet

		// constructeur
		Triangle()
		{
			a = b = c = -1;		// initialisation
		};
};


class Model3D
{
	public:


		Sommet		*points;
		Triangle	*faces;
		int			nb_points;
		int			nb_triangles;


		// constructeur
		Model3D()
		{ 
			nb_points		= 0;
			nb_triangles	= 0;
			points			= NULL;
			faces			= NULL;
		};


		// destructeur (fait le ménage)
		~Model3D()
		{ 
			if (points != NULL) 
				delete[] points;

			if (faces  != NULL) 
				delete[] faces;
		};

		bool load_3d_model(char *filename);	// défini dans "load.cpp"


};


class Image
{
	public:

		dword  lenx;           // largeur de l'image
		dword  leny;           // hauteur de l'image
		dword  nbbits;         // vaut 16, 24 ou 32     (32 => couche alpha pour les textures)
		byte   *data;          // pointeur sur les données

		Image()               // créé une image vide
		{
			lenx = leny = nbbits = 0;
			data = NULL;
		}

		~Image()
		{
			if (data != NULL)
				delete[] data;
		}

		bool load_tga(char *filename);	// défini dans "load.cpp"

};



class Texture
{

	public:

		Image	*img_color;
		Image	*img_alpha;			// contient la texture sous forme d'un tableau a une dimension
		char	*img_all;
		dword	OpenGL_ID[1];	// "nom" opengl de la texture
		bool	isRGBA;			// à true si RGBA (4 octets par pixel), sinon RGB (3 octets)


		Texture()
		{
			img_color	= NULL;
			img_alpha	= NULL;
			img_all		= NULL;
			OpenGL_ID[0]= (unsigned)-1;
			isRGBA		= false;
		}

		~Texture()
		{
			if (img_color != NULL)
				delete img_color;								// vide la mémoire standard de l'image
			if (img_alpha != NULL)
				delete img_alpha;								// vide la mémoire standard de l'image
			if (img_all != NULL)
				delete img_all;								// vide la mémoire standard de l'image

			glDeleteTextures(1, OpenGL_ID);				// vide la mémoire vidéo de la texture
		}

		bool load_texture(char *filename, char *filename2);				// défini dans "load.cpp"

};






#endif

