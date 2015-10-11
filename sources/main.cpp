                                                                                                     /*
/##################################################################################################\
# Description : #                                                                                  #
#################                                                                                  #
#                                                                                                  #
#  Ce fichier est le point d'entrée du moteur ( le "main()" ).                                     #
#                                                                                                  #
#                                                                                                  #
\##################################################################################################/
                                                                                                     */

#include "window.h"
#include "input.h"
#include "timer.h"
#include "ogl.h"
#include "load.h"
#include "camera.h"
#include "point.h"
#include "maths_3d.h"
#include <iostream>

#define cubeList 1


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
WINDOW		*win = NULL;
MY_INPUT	*inp = NULL;
TIMER		*tim = NULL;

// Textures, images
Texture *texWall = NULL;
Texture *texGrass = NULL;
Texture *texMinimap = NULL;
Texture *texPlayer = NULL;
Image	*heightmapLabyrinth = NULL;
Image	*heightmapLabyrinthH = NULL;

float size1 = 5;		// resolution
float size2 = 5;	// step

// Camera
CAMERA *cam = NULL;
float	angleX = 0;
float	angleY = 0;
float	angleZ = 0;
float oldAngleX = 0;
float oldAngleY = 0;
point posStart;
float posX = 35;		float prevPosX = 35;
float posY = 4.9;			float prevPosY = 4.9;
float posZ = 10;		float prevPosZ = 10;
float speed = 0.15;
bool b_canMoveAtX = true;
bool b_canMoveAtZ = true;
bool **bPtr_bricks;

/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/


void cube(int i, int j) {
	// back
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1 + size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1, 0, (j / 4)*size1 + size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1, size2, (j / 4)*size1 + size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1 + size1);
	glEnd();

	// front
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1, 0, (j / 4)*size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1, size2, (j / 4)*size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1);
	glEnd();

	// left
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1 + size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1 + size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1);
	glEnd();

	// right
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + 0, 0, (j / 4)*size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1 + 0, 0, (j / 4)*size1 + size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1 + 0, size2, (j / 4)*size1 + size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + 0, size2, (j / 4)*size1);
	glEnd();

	// top
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1, size2, (j / 4)*size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1, size2, (j / 4)*size1 + size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + size1, size2, (j / 4)*size1 + size1);
	glEnd();

	// bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1);
	glTexCoord2f(1, 0);
	glVertex3f((i / 4)*size1, 0, (j / 4)*size1);
	glTexCoord2f(1, 1);
	glVertex3f((i / 4)*size1, 0, (j / 4)*size1 + size1);
	glTexCoord2f(0, 1);
	glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1 + size1);
	glEnd();
}

void generateLabyrinth() {
	// WALL
	glGenTextures(1, texWall->OpenGL_ID);                // crée un "nom" de texture (un identifiant associé a la texture)
	glBindTexture(GL_TEXTURE_2D, texWall->OpenGL_ID[0]);    // et on active ce "nom" comme texture courante (définie plus bas)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    // ou < 0.0

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap
	if (texWall->isRGBA)// with alpha
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texWall->img_color->lenx, texWall->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texWall->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texWall->img_color->lenx, texWall->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texWall->img_color->data);// no alpha

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	for (int i = 0; i < (int)heightmapLabyrinth->lenx - 1; i++) {
		if (i % 4 == 1) {
			for (int j = 0; j < (int)heightmapLabyrinth->leny - 1; j++) {
				if (j % 4 == 0) {
					if ((float)heightmapLabyrinth->data[3 * (i + heightmapLabyrinth->lenx*j)] > 125) {
						cube(i, j);
						/*glPushMatrix();
						glTranslatef(i, 0, j);
						glCallList(cubeList);
						glPopMatrix();*/
					}
				}
			}
		}
	}
	glPopMatrix();
	
	
	// GRASS
	glGenTextures(1, texGrass->OpenGL_ID);                // crée un "nom" de texture (un identifiant associ??la texture)
	glBindTexture(GL_TEXTURE_2D, texGrass->OpenGL_ID[0]);    // et on active ce "nom" comme texture courante (définie plus bas)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    // ou < 0.0

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // indique qu'il faut mélanger la texture avec la couleur courante

	// charge le tableau de la texture en mémoire vidéo et crée une texture mipmap
	if (texGrass->isRGBA)// with alpha
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, texGrass->img_color->lenx, texGrass->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, texGrass->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, texGrass->img_color->lenx, texGrass->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, texGrass->img_color->data);// no alpha

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	for (int i = 0; i < (int)heightmapLabyrinth->lenx - 1; i++) {
		if (i % 4 == 0) {
			for (int j = 0; j < (int)heightmapLabyrinth->leny - 1; j++) {
				if (j % 4 == 0) {
					if ((float)heightmapLabyrinth->data[3 * (i + heightmapLabyrinth->lenx*j)] < 125) {
						glBegin(GL_QUADS);
						glTexCoord2f(0, 0);
						glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1);
						glTexCoord2f(1, 0);
						glVertex3f((i / 4)*size1, 0, (j / 4)*size1);
						glTexCoord2f(1, 1);
						glVertex3f((i / 4)*size1, 0, (j / 4)*size1 + size1);
						glTexCoord2f(0, 1);
						glVertex3f((i / 4)*size1 + size1, 0, (j / 4)*size1 + size1);
						glEnd();
					}
				}
			}
		}
	}
	glPopMatrix();
}

point coordWorldToMinimap(point worldPoint) {
	point result;

	result.x = (int)(worldPoint.x / size1) * 4;
	result.y = (int)(worldPoint.z / size1) * 4;

	if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.75*size1) {
		result.x += 4;
	}
	else if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.5*size1) {
		result.x += 2;
	}
	else if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.25*size1) {
		result.x += 1;
	}

	if ((worldPoint.z - ((int)(worldPoint.z / size1)) * size1) >= 0.75*size1) {
		result.y += 4;
	}
	else if ((worldPoint.z - ((int)(worldPoint.z / size1)) * size1) >= 0.5*size1) {
		result.y += 2;
	}
	else if ((worldPoint.z - ((int)(worldPoint.z / size1)) * size1) >= 0.25*size1) {
		result.y += 1;
	}


	result.z = 0;

	return result;
}


point coordMinimapToWorld(int x, int y) {
	point posInWorld = point(0, 0, 0);

	posInWorld.x = (x / 4) * size1 + (x % 4) * 0.25 * size1;
	posInWorld.z = (y / 4) * size1 + (x % 4) * 0.25 * size1;

	if ((float)heightmapLabyrinth->data[3 * (x + heightmapLabyrinth->lenx*y)] > 125)
	{
		posInWorld.y = size2;
	}
	else
	{
		posInWorld.y = 0;
	}

	return posInWorld;
}

void player() {
	point minimapPos = coordWorldToMinimap(point(posX, 0, posZ));
	float i = minimapPos.x;
	float j = minimapPos.y + 500;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)win->Xres, 0.0, (double)win->Yres);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glGenTextures(3, texPlayer->OpenGL_ID);
	glBindTexture(GL_TEXTURE_2D, texPlayer->OpenGL_ID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.95);
	glRasterPos2i(i, j);
	if (texPlayer->isRGBA)
		glDrawPixels(texPlayer->img_color->lenx, texPlayer->img_color->leny,
		GL_RGBA, GL_UNSIGNED_BYTE, texPlayer->img_all);
	else
		glDrawPixels(texPlayer->img_color->lenx, texPlayer->img_color->leny,
		GL_RGBA, GL_UNSIGNED_BYTE, texPlayer->img_color->data);
	glDisable(GL_ALPHA_TEST);
}

void minimap() {
	// render player's position on the minimap
	player();
	int i = 0;
	int j = 500;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)win->Xres, 0.0, (double)win->Yres);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glGenTextures(2, texMinimap->OpenGL_ID);
	glBindTexture(GL_TEXTURE_2D, texMinimap->OpenGL_ID[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.95);
	glRasterPos2i(i, j);
	if (texMinimap->isRGBA)
		glDrawPixels(texMinimap->img_color->lenx, texMinimap->img_color->leny,
		GL_RGBA, GL_UNSIGNED_BYTE, texMinimap->img_all);
	else
		glDrawPixels(texMinimap->img_color->lenx, texMinimap->img_color->leny,
		GL_RGBA, GL_UNSIGNED_BYTE, texMinimap->img_color->data);
	glDisable(GL_ALPHA_TEST);	
}


bool isWall(int i, int j) {
	point coordHeightmap = coordWorldToMinimap(point(i, j, 0));
	return ((float)heightmapLabyrinth->data[3 * (i + heightmapLabyrinth->lenx*j)] > 125);
}

float getDistanceXZ(point point1, point point2) {
	float f_distance;
	point pointD;
	pointD.x = point1.x - point2.x;
	pointD.z = point1.z - point2.z;

	f_distance = pointD.x * pointD.x + pointD.z * pointD.z;
	return f_distance;
}

float getHeightFromMinimapToWorldByWorldPosition(point posInWorld) {
	float heightInWorld;
	point posInPixel;

	posInPixel = coordWorldToMinimap(posInWorld);

	if ((float)heightmapLabyrinthH->data[3 * ((int)posInPixel.x + heightmapLabyrinth->lenx*(int)posInPixel.y)] > 125)
		//if ((float)heightmap_terrain->data[3 * ((int)posInPixel.x + heightmap_terrain->lenx*(int)posInPixel.y)] > 125)
	{
		posInPixel = coordWorldToMinimap(posInWorld);
		heightInWorld = size2;
	}
	else
	{
		heightInWorld = 0;
	}

	return heightInWorld;
}

void updatePosition() {
	point dir = cam->direction - cam->position;

	if (inp->keys[KEY_CODE_UP]){
		posX += speed*dir.x;
		posZ += speed*dir.z;
	}
	if (inp->keys[KEY_CODE_DOWN]){
		posX -= speed*dir.x;
		posZ -= speed*dir.z;
	}
	if (inp->keys[KEY_CODE_RIGHT]){
		posX += speed*produit_vectoriel(dir, cam->orientation).x;
		posZ += speed*produit_vectoriel(dir, cam->orientation).z;
	}
	if (inp->keys[KEY_CODE_LEFT]){
		posX -= speed*produit_vectoriel(dir, cam->orientation).x;
		posZ -= speed*produit_vectoriel(dir, cam->orientation).z;
	}


	//////////////////
	if (posX > ((heightmapLabyrinth->lenx / 4)*size1 - size1*0.5))
		posX = (heightmapLabyrinth->lenx / 4)*size1 - size1*0.5;
	if (posZ > ((heightmapLabyrinth->leny / 4)*size1 - size1*0.5))
		posZ = (heightmapLabyrinth->leny / 4)*size1 - size1*0.5;
	if (posX < -size1*0.5)
		posX = -size1*0.5;
	if (posZ < -size1*0.5)
		posZ = -size1*0.5;

	posY = getHeightFromMinimapToWorldByWorldPosition(point(posX, posY, posZ));


	//////////////////
	if (angleY > 360) angleY -= 360;
	if (angleY < 0) angleY += 360;

	if (angleX > 60) angleX = 60;
	if (angleX < -60) angleX = -60;

	angleX -= 0.01*(float)inp->Yrelmouse;
	angleY -= 0.01*(float)inp->Xrelmouse;


	//////////////////
	int i_xPlayerPredict = posX / size1;
	int i_zPlayerPredict = posZ / size1;

	if (posX >= 1.9*size1)
	{
		std::cout << std::endl;
	}


	//////////////////
	if (
		(i_xPlayerPredict + 1 <= heightmapLabyrinth->lenx / 4 - 1)
		&& (*(bPtr_bricks[i_xPlayerPredict + 1] + i_zPlayerPredict) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict + 1)*size1 + 0.5*size1, posY, posZ))
			<= 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			float f = getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict + 1)*size1 + 0.5*size1, posY, i_zPlayerPredict*size1 + 0.5*size1));
			b_canMoveAtX = false;
		}
	}

	if (
		(i_xPlayerPredict - 1 >= 0)
		&& (*(bPtr_bricks[i_xPlayerPredict - 1] + i_zPlayerPredict) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict - 1)*size1 + 0.5*size1, posY, posZ))
			<= 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtX = false;
		}
	}

	if (
		(i_zPlayerPredict + 1 <= heightmapLabyrinth->leny / 4 - 1)
		&& (*(bPtr_bricks[i_xPlayerPredict] + i_zPlayerPredict + 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point(posX, posY, (i_zPlayerPredict + 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtZ = false;
		}
	}

	if (
		(i_zPlayerPredict - 1 >= 0)
		&& (*(bPtr_bricks[i_xPlayerPredict] + i_zPlayerPredict - 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point(posX, posY, (i_zPlayerPredict - 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtZ = false;
		}
	}

	if (
		(i_xPlayerPredict + 1 <= heightmapLabyrinth->lenx / 4 - 1) && (i_zPlayerPredict + 1 <= heightmapLabyrinth->leny / 4 - 1)
		&& (*(bPtr_bricks[i_xPlayerPredict + 1] + i_zPlayerPredict + 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict + 1)*size1 + 0.5*size1, posY, (i_zPlayerPredict + 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtX = false;
			b_canMoveAtZ = false;
		}
	}

	if (
		(i_xPlayerPredict + 1 <= heightmapLabyrinth->lenx / 4 - 1) && (i_zPlayerPredict - 1 >= 0)
		&& (*(bPtr_bricks[i_xPlayerPredict + 1] + i_zPlayerPredict - 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict + 1)*size1 + 0.5*size1, posY, (i_zPlayerPredict - 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtX = false;
			b_canMoveAtZ = false;
		}
	}

	if (
		(i_xPlayerPredict - 1 >= 0) && (i_zPlayerPredict + 1 <= heightmapLabyrinth->leny / 4 - 1)
		&& (*(bPtr_bricks[i_xPlayerPredict - 1] + i_zPlayerPredict + 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict - 1)*size1 + 0.5*size1, posY, (i_zPlayerPredict + 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtX = false;
			b_canMoveAtZ = false;
		}
	}

	if (
		(i_xPlayerPredict - 1 >= 0) && (i_zPlayerPredict - 1 >= 0)
		&& (*(bPtr_bricks[i_xPlayerPredict - 1] + i_zPlayerPredict - 1) == true
		)
		)
	{
		if (getDistanceXZ(point(posX, posY, posZ), point((i_xPlayerPredict + 1)*size1 + 0.5*size1, posY, (i_zPlayerPredict + 1)*size1 + 0.5*size1))
			<= 0.5*size1 * 0.5*size1 + 0.5*size1 * 0.5*size1 + 0.25*size1)
		{
			b_canMoveAtX = false;
			b_canMoveAtZ = false;
		}
	}

	if (posY > posStart.y)
	{
		if (getHeightFromMinimapToWorldByWorldPosition(point(prevPosX, prevPosY, posZ)) > posStart.y)
		{
			b_canMoveAtZ = false;
		}
		if (getHeightFromMinimapToWorldByWorldPosition(point(posX, prevPosY, prevPosZ)) > posStart.y)
		{
			b_canMoveAtX = false;
		}
	}



	if (b_canMoveAtX == false)
	{
		posX = prevPosX;
	}
	if (b_canMoveAtZ == false)
	{
		posZ = prevPosZ;
	}
	posY = posStart.y;

	//////////////////
	cam->update(point(posX, posY, posZ), angleX, angleY, angleZ);


	//////////////////
	b_canMoveAtX = true;
	b_canMoveAtZ = true;
	prevPosX = posX;
	prevPosY = posY;
	prevPosZ = posZ;


	//////////////////
	//////////////////
}

/********************************************************************\
*                                                                    *
*  Démarre l'application (avant la main_loop). Renvoie false si      *
*  il y a une erreur.                                                *
*                                                                    *
\********************************************************************/
bool start()
{
	// initialisations de base
	// -----------------------
	win = new WINDOW();									// prépare la fenêtre
	win->create( 800, 600, 16 , 60 , false );			// crée la fenêtre

	tim = new TIMER();									// crée un timer

	inp = new MY_INPUT(win);								// initialise la gestion clavier souris
	create_context(*win);								// crée le contexte OpenGL sur la fenêtre
	init_font(*win, "Courier");							// initialise la gestion de texte avec OpenGL

  
	// initialisations d'OpenGL
	// ------------------------
	glClearColor(0.0f,0.0f,0.0f,1.0f);					// set clear color for color buffer (RGBA, black)
	glViewport(0,0,win->Xres,win->Yres);				// zone de rendu (tout l'écran)
 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// mapping quality = best
	glFrontFace(GL_CCW);								// front of face is defined counter clock wise
	glPolygonMode(GL_FRONT, GL_FILL);					// front of a face is filled
	glPolygonMode(GL_BACK, GL_FILL);					// back of a face is made of lines
	glCullFace(GL_BACK);								// cull back face only
	glDisable(GL_CULL_FACE);						    // disable back face culling


	win->set_title("Mon jeu");


	// Load textures
	heightmapLabyrinth = new Image();
	heightmapLabyrinth->load_tga("./data/labyrinth1.tga");

	heightmapLabyrinthH = new Image();
	heightmapLabyrinthH->load_tga("./data/labyrinth1_height.tga");

	texWall = new Texture();
	texWall->load_texture("./data/wall.tga", NULL);

	texGrass = new Texture();
	texGrass->load_texture("./data/grass.tga", NULL);

	texMinimap = new Texture();
	texMinimap->load_texture("./data/labyrinth1.tga", "./data/labyrinth1mask.tga");
	
	texPlayer = new Texture();
	texPlayer->load_texture("./data/player.tga", "./data/playermask.tga");

	// bricks
	bPtr_bricks = new bool*[heightmapLabyrinth->lenx / 4];
	for (int i = 0; i < heightmapLabyrinth->lenx / 4; ++i)
	{
		bPtr_bricks[i] = new bool[heightmapLabyrinth->leny / 4];

		for (int j = 0; j < heightmapLabyrinth->leny / 4; ++j)
		{
			*(bPtr_bricks[i] + j) = false;
		}
	}


	cam = new CAMERA();

	/*glNewList(cubeList, GL_COMPILE);
	cube(0, 0);
	glEndList();*/

	posStart = coordMinimapToWorld(13,9);
	posStart.y += 3.5;
	prevPosX = posX = posStart.x;
	prevPosY = posY = posStart.y;
	prevPosZ = posZ = posStart.z;
	
	return true;
}

/********************************************************************\
*                                                                    *
*  les formes                                               *
*                                                                    *
\********************************************************************/

/********************************************************************\
*                                                                    *
*  Boucle principale, appelée pour construire une image, gérer les   *
*  entrées.                                                          *
*                                                                    *
\********************************************************************/
void main_loop() 
{

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//				gestion des touches	et du temps													//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	inp->refresh();
	tim->update_horloge();
	inp->get_mouse_movement();//pour avoir les dÈplacement de la souris


	if (inp->keys[KEY_CODE_ESCAPE])
	{	  
		PostMessage(win->handle,WM_CLOSE,0,0);	// Stoppe la "pompe à message" en y envoyant le message "QUIT"
	}
	glClearColor(0.498f, 0.780f, 1.0f, 1.0f);

	/*if (tim->global_timer_25_Hz)				// augmente angleY tous les 20ème de seconde
        angleY += 1.0f;
	if (angleY >= 360) angleY -= 360;*/





	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						ça commence ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// votre code OpenGL ici
	glMatrixMode(GL_PROJECTION);  //la matrice de projection sera celle selectionnee
	//composition d'une matrice de projection
	glLoadIdentity(); //on choisit la matrice identité
	gluPerspective(60, (double)win->Xres / (double)win->Yres, 0.1, 30000);   //mise en place d une proj angle de vue 60 deg near 10 far 30000

	
	// camera
	updatePosition();



	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cam->position.x, cam->position.y, cam->position.z,		// position
		cam->direction.x, cam->direction.y, cam->direction.z,		// point cible
		cam->orientation.x, cam->orientation.y, cam->orientation.z);		// vecteur up

	// print debug info
	glPushMatrix();
	glTranslatef(cam->direction.x, cam->direction.y, cam->direction.z);
	char angleXArray[100] = "";
	char angleYArray[100] = "";
	char height[100] = "";
	char res[200] = "";
	sprintf(angleXArray, "%f", posX);
	sprintf(angleYArray, "%f", posZ);
	strcat(res, "x: ");
	strcat(res, angleXArray);
	//if (angleX - oldAngleX > 0)			strcat(res, " (+)");
	//else if (angleX - oldAngleX < 0)	strcat(res, " (-)");
	strcat(res, "; y: ");
	strcat(res, angleYArray);
	if (isWall(posX, posZ)) {
		strcat(res, "; WALL");
	} else {
		strcat(res, "; NOT WALL");
	}
	sprintf(height, "%f", getHeightFromMinimapToWorldByWorldPosition(point(posX, posY, posZ)));
	strcat(res, "; H=");
	strcat(res, height);
	
	write_2_screen(res);
	glPopMatrix();
	// End print debug info



	generateLabyrinth();
	minimap();

	
	/*if (isWall(posX, posZ)) {
		posX = prevPosX;
		posZ = prevPosZ;
	}*/
	oldAngleX = angleX;

	//////////////////////////
	for (int i = 0; i < heightmapLabyrinth->lenx / 4 - 1; ++i)
	{
		for (int j = 0; j < heightmapLabyrinth->leny / 4 - 1; ++j)
		{
			if (true)
			{
				if (j + 1 <= heightmapLabyrinth->leny / 4 - 1 && *(bPtr_bricks[i] + j + 1) == true)
				{
					*(bPtr_bricks[i] + j + 1) = false;
				}

				if (j - 1 >= 0 && *(bPtr_bricks[i] + j - 1) == true)
				{
					*(bPtr_bricks[i] + j - 1) = false;
				}

				if (i + 1 <= heightmapLabyrinth->lenx / 4 - 1 && *(bPtr_bricks[i + 1] + j) == true)
				{
					*(bPtr_bricks[i + 1] + j) = false;
				}

				if (i - 1 > 0 && *(bPtr_bricks[i - 1] + j) == true)
				{
					*(bPtr_bricks[i - 1] + j) = false;
				}


				int i_xPlayerUpdate = cam->position.x / size1;
				int i_zPlayerUpdate = cam->position.z / size1;

				if ((i_xPlayerUpdate == i + 1 && i_zPlayerUpdate == j)
					|| (i_xPlayerUpdate == i - 1 && i_zPlayerUpdate == j)
					|| (i_xPlayerUpdate == i && i_zPlayerUpdate == j + 1)
					|| (i_xPlayerUpdate == i && i_zPlayerUpdate == j - 1))
				{
					//debug("YOU DIED");
					//PostMessage(win->handle, WM_CLOSE, 0, 0);	// Stoppe la "pompe ?message" en y envoyant le message "QUIT"
				}
			}
		}
	}




	//////////////////////////
	

	


	/*glTranslatef(0,0,0);
	glRotatef(angleY,0,1,0);
	glBegin(GL_TRIANGLES);
		glColor3f(1,0,0); glVertex3f(-50,-50, -10);
		glColor3f(0,1,0); glVertex3f(+50,-50, -10);
		glColor3f(0,0,1); glVertex3f(  0,+50, -10);
	glEnd();*/


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//						ça finit ici															//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	swap_buffer(win);	// affiche l'image composée à l'écran
}


/********************************************************************\
*                                                                    *
* Arrête l'application                                               *
*                                                                    *
\********************************************************************/
void stop()
{
	delete inp;
	delete tim;

	if (win)
	{
		kill_font();
		kill_context(*win);
		delete win;
	}
}


/********************************************************************\
*                                                                    *
* Point d'entrée de notre programme pour Windows ("WIN32")           *
*                                                                    *
\********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	MSG  msg;

	win	= NULL;
	inp	= NULL;
	tim = NULL;

	if (start() == false)								// initialise la fenêtre et OpenGL
	{
		debug("start() : échec !");
		stop();
		return 1;
	}

	// main loop //
	// --------- //
	//   __
	//  /  \_
	//  |  \/ 
	//  \__/

	while (true)
	{
		if (PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))	// s'il y a un message, appelle WndProc() pour le traiter
		{
			if (!GetMessage(&msg,NULL,0,0))				// "pompe à message"
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else 
		{
			main_loop();								// sinon, appelle main_loop()
		}
	}


	stop();												// arrête OpenGL et ferme la fenêtre

	return 0;
}



/********************************************************************\
*                                                                    *
*  Boucle des messages                                               *
*                                                                    *
\********************************************************************/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch (msg)
    {

      case WM_MOVE:        win->Xpos = (dword) LOWORD(lParam);
                           win->Ypos = (dword) HIWORD(lParam);
                           return 0;

      case WM_CLOSE:       PostQuitMessage(0); // dit à GetMessage() de renvoyer 0
                           return 0;

      case WM_SYSCOMMAND:  // évite l'économiseur d'écran
                           switch (wParam)
                             {
                               case SC_SCREENSAVE:
                               case SC_MONITORPOWER:
                               return 0;
                             }
                           break;
/*
      case WM_CHAR:        la touche est traduite dans ce msg 
                           return 0;
*/

      case WM_KEYDOWN:     inp->set_key_down ((dword)wParam);
                           return 0;

      case WM_KEYUP:       inp->set_key_up   ((dword)wParam);
                           return 0;

      case WM_LBUTTONDOWN: inp->set_mouse_left_down();
                           return 0;

      case WM_LBUTTONUP:   inp->set_mouse_left_up();
                           return 0;

      case WM_RBUTTONDOWN: inp->set_mouse_right_down();
                           return 0;

      case WM_RBUTTONUP:   inp->set_mouse_right_up();
                           return 0;

      case WM_MBUTTONDOWN: inp->set_mouse_middle_down();
                           return 0;

      case WM_MBUTTONUP:   inp->set_mouse_middle_up();
                           return 0;

   }

  return DefWindowProc(hwnd,msg,wParam,lParam);
}

