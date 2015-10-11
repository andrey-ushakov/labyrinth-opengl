
#include "load.h"




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
*                                                                          *
*                                                                          *
\**************************************************************************/
bool Model3D::load_3d_model(char *filename)
{
	int	i,dummy, nb_uv,nb_pts, nb_tri;
	char comment[1024];

	FILE *f = fopen(filename,"r");

	if (f == NULL)
	{
		debug("Fichier \"%s\" non trouvé !",filename);
		return false;
	}

	// lit l'entête du fichiers
	fgets(comment,1024,f);	// "OFF"
	fscanf(f,"%d %d %d\n", &nb_pts, &nb_tri, &dummy);
	float *pts = new float[nb_pts*3];
	float *nor = new float[nb_pts*3];
	int   *tri = new int  [nb_tri*3];

	// lecture des sommets, remplit le tableau
	for (i=0; i<nb_pts; i++)
	{
		fscanf(f,"%f %f %f\n", &(pts[i*3+0]), &(pts[i*3+1]), &(pts[i*3+2]) );
	}

	// lecture des faces, remplit le tableau
	for (i=0; i<nb_tri; i++)
	{
		fscanf(f,"%d %d %d %d\n", &dummy, &(tri[i*3+2]), &(tri[i*3+1]), &(tri[i*3+0])  );
	}


	float *uv    = NULL;
	int   *uvidx = NULL;
	fgets(comment,1024,f);	// "EXT"
	if (comment[0] == 'E')
	{
		nb_uv = atoi(comment+4);
		uv    = new float[nb_uv*2];
		uvidx = new int  [nb_tri*3];

		// lecture des coordonnées de textures, remplit le tableau
		for (i=0; i<nb_uv; i++)
		{
			fscanf(f,"%f %f\n", &(uv[i*2+0]), &(uv[i*2+1]) );
		}

		for (i=0; i<nb_tri; i++)
		{
			fscanf(f,"%d %d %d\n", &(uvidx[i*3+2]), &(uvidx[i*3+1]), &(uvidx[i*3+0]) );
		}

		// lecture des normales
		for (i=0; i<nb_pts; i++)
		{
			fscanf(f,"%f %f %f\n", &(nor[i*3+0]), &(nor[i*3+1]), &(nor[i*3+2]) );
		}
	}
	else
	{
		nb_uv = 1;
		uv    = new float[nb_uv*2];
		uvidx = new int  [nb_tri*3];
		uv[0*2+0] = 0.0f;
		uv[0*2+1] = 0.0f;
		for (i=0; i<nb_tri; i++)
		{
			uvidx[i*3+0] = 0;
			uvidx[i*3+1] = 0;
			uvidx[i*3+2] = 0;
		}
		for (i=0; i<nb_pts; i++)
		{
			nor[i*3+0] = 0.0;
			nor[i*3+1] = 0.0;
			nor[i*3+2] = 1.0;
		}
	}

	fclose(f);




	nb_triangles = nb_tri;
	faces        = new Triangle[nb_triangles];
	for (i=0; i<nb_tri; i++)
	{
		faces[i].a = tri[i*3+0];
		faces[i].b = tri[i*3+1];
		faces[i].c = tri[i*3+2];
	}
	
	nb_points = nb_tri * 3;
	points	  = new Sommet[nb_points];

	for (i=0; i<nb_tri; i++)
	{
		points[ i*3+0 ].x = pts[ faces[i].c*3+0 ];
		points[ i*3+0 ].y = pts[ faces[i].c*3+1 ];
		points[ i*3+0 ].z = pts[ faces[i].c*3+2 ];

		points[ i*3+1 ].x = pts[ faces[i].b*3+0 ];
		points[ i*3+1 ].y = pts[ faces[i].b*3+1 ];
		points[ i*3+1 ].z = pts[ faces[i].b*3+2 ];

		points[ i*3+2 ].x = pts[ faces[i].a*3+0 ];
		points[ i*3+2 ].y = pts[ faces[i].a*3+1 ];
		points[ i*3+2 ].z = pts[ faces[i].a*3+2 ];



		points[ i*3+0 ].nx = nor[ faces[i].c*3+0 ];
		points[ i*3+0 ].ny = nor[ faces[i].c*3+1 ];
		points[ i*3+0 ].nz = nor[ faces[i].c*3+2 ];

		points[ i*3+1 ].nx = nor[ faces[i].b*3+0 ];
		points[ i*3+1 ].ny = nor[ faces[i].b*3+1 ];
		points[ i*3+1 ].nz = nor[ faces[i].b*3+2 ];

		points[ i*3+2 ].nx = nor[ faces[i].a*3+0 ];
		points[ i*3+2 ].ny = nor[ faces[i].a*3+1 ];
		points[ i*3+2 ].nz = nor[ faces[i].a*3+2 ];



		points[ i*3+0 ].ucol =   uv[ uvidx[i*3+2]*2+0 ];
		points[ i*3+0 ].vcol =   uv[ uvidx[i*3+2]*2+1 ];

		points[ i*3+1 ].ucol =   uv[ uvidx[i*3+1]*2+0 ];
		points[ i*3+1 ].vcol =   uv[ uvidx[i*3+1]*2+1 ];

		points[ i*3+2 ].ucol =   uv[ uvidx[i*3+0]*2+0 ];
		points[ i*3+2 ].vcol =   uv[ uvidx[i*3+0]*2+1 ];


		faces[i].a = i*3+0;
		faces[i].b = i*3+1;
		faces[i].c = i*3+2;
	}




	delete[] pts;
	delete[] nor;
	delete[] tri;
	delete[] uv;
	delete[] uvidx;


	return true;
}




/*************************************************************************\
*                                                                         *
*  Charge une image TGA de "bits" bits et la convertit en nbbits bits     *
*                                                                         *
\*************************************************************************/
bool Image::load_tga(char *filename)
{
	FILE  *f;
	dword taille;
	dword nb_byte_2_load;
	dword bloc;
	byte  tmp;

	// Format TGA : le header + un bloc de idlength octets + les donnees
	struct
	{
		byte idlength;                           // header dans l'ordre
		byte colourmaptype;
		byte datatypecode;
		word colourmaporigin;
		word colourmaplength;
		byte colourmapdepth;
		word x_origin;
		word y_origin;
		word width;
		word height;
		byte bitsperpixel;
		byte imagedescriptor;
	} TGA_HEADER;


	// LECTURE
	f = fopen(filename, "rb");
	if (f == NULL)
	{
		debug("Fichier \"%s\" non trouvé !",filename);
		return false;
	}
	fseek(f,0,SEEK_END); 
	nb_byte_2_load = ftell(f);                    // lecture taille du fichier

	fseek(f, 0,SEEK_SET); fread(&(TGA_HEADER.idlength     ),1,1,f);
	fseek(f, 1,SEEK_SET); fread(&(TGA_HEADER.colourmaptype),1,1,f);
	fseek(f, 2,SEEK_SET); fread(&(TGA_HEADER.datatypecode ),1,1,f);
	fseek(f,12,SEEK_SET); fread(&(TGA_HEADER.width        ),1,2,f);
	fseek(f,14,SEEK_SET); fread(&(TGA_HEADER.height       ),1,2,f);
	fseek(f,16,SEEK_SET); fread(&(TGA_HEADER.bitsperpixel ),1,1,f);

	fseek(f,18+TGA_HEADER.idlength,SEEK_SET);    // on se place au debut des donnees


	lenx   = TGA_HEADER.width;
	leny   = TGA_HEADER.height;
	nbbits = TGA_HEADER.bitsperpixel;

	taille			= lenx * leny * (nbbits>>3);
	nb_byte_2_load	= nb_byte_2_load - (TGA_HEADER.idlength + 18);
  
	// Tri les algorithmes de decompression supportes
	switch (TGA_HEADER.datatypecode)
	{
		// formats supportes
		case  2: // Uncompressed, RGB images.      
			   break;
       
		// formats non supportes
		case  0: // No image data included.
		case  1: // Uncompressed, color-mapped images.
		case  3: // Uncompressed, black and white images.
		case  9: // Runlength encoded color-mapped images.
		case 10: // Runlength encoded RGB images.
		case 11: // Compressed, black and white images.
		case 32: // Compressed color-mapped data, using Huffman, Delta, and runlength encoding.
		case 33: // Compressed color-mapped data, using Huffman, Delta, and runlength encoding. 4-pass quadtree-type process.
		default: // Erreur        
				return false;
       
	}

	if (TGA_HEADER.bitsperpixel != 24 && TGA_HEADER.bitsperpixel != 32)
	{
		fclose(f);
		return false;
	}

	data = new byte[nb_byte_2_load];       // on y va bourrin
	if (data == NULL)                      // probleme allocation memoire
	{
		fclose(f);
		return false;
	}  

	fread(data ,1, nb_byte_2_load, f);
	fclose(f);

	bloc = (nbbits == 24 ? 3 : 4);
	for (dword i=0; i<lenx*leny; i++)		// permutte le rouge et bleu (BGR -> RGB)
	{										// ou  BGRA -> RGBA
		tmp			   = data[i*bloc+0];
		data[i*bloc+0] = data[i*bloc+2];
		data[i*bloc+2] = tmp;
	} 

	return true;
}



/*************************************************************************\
*                                                                         *
*  Crée une texture à partir d'un fichier TGA non compressé, renvoie      *
*  true si tout s'est bien passé.                                         *
*                                                                         *
\*************************************************************************/
bool Texture::load_texture(char *filename,char *filename2)
{


	img_color = new Image();

	if (img_color->load_tga(filename) == false)
	{
		debug("échec à la création de la texture !");
		return false;
	}

	if(filename2!=NULL){

		img_alpha = new Image();

	if (img_alpha->load_tga(filename2) == false)
	{
		debug("échec à la création de la texture !");
		return false;
	}

	img_all=new char[img_color->lenx*img_color->leny*4];

	int i=0;
	int k=0;

	for(i=0;i<(int)(img_color->lenx*img_color->leny);i++){
		img_all[k]=img_color->data[3*i+0];
		k++;
		img_all[k]=img_color->data[3*i+1];
		k++;
		img_all[k]=img_color->data[3*i+2];
		k++;
		img_all[k]=img_alpha->data[3*i+0];
		k++;
	}
	}

	if (filename2 != NULL)
		isRGBA = true;
	else
		isRGBA = false;

	return true;
}
