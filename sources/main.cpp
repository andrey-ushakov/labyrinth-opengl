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

float size1 = 5;		// resolution
float size2 = 5;	// step

// Camera
CAMERA *cam = NULL;
float	angleX = 0;
float	angleY = 0;
float	angleZ = 0;
float oldAngleX = 0;
float oldAngleY = 0;
int posX = 0;
int posY = 20;
int posZ = 0;
int speed = 2;

/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/



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
						glVertex3f(		(i / 4)*size1 + size1,	0,		 (j / 4)*size1 );
						glTexCoord2f(1, 0);
						glVertex3f(		(i / 4)*size1,			0,		(j / 4)*size1 );
						glTexCoord2f(1, 1);
						glVertex3f(		(i / 4)*size1,			size2,	(j / 4)*size1 );
						glTexCoord2f(0, 1);
						glVertex3f(		(i / 4)*size1 + size1,	size2,	(j / 4)*size1 );
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
					else {
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
	result.y = (int)(worldPoint.y / size1) * 4;

	if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.75*size1) {
		result.x += 4;
	} else if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.5*size1) {
		result.x += 2;
	} else if ((worldPoint.x - ((int)(worldPoint.x / size1)) * size1) >= 0.25*size1) {
		result.x += 1;
	}

	if ((worldPoint.y - ((int)(worldPoint.y / size1)) * size1) >= 0.75*size1) {
		result.y += 4;
	} else if ((worldPoint.y - ((int)(worldPoint.y / size1)) * size1) >= 0.5*size1) {
		result.y += 2;
	} else if ((worldPoint.y - ((int)(worldPoint.y / size1)) * size1) >= 0.25*size1) {
		result.y += 1;
	}


	result.z = 0;

	return result;
}

void player() {
	point minimapPos = coordWorldToMinimap(point(posX, posZ, 0));
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

	texWall = new Texture();
	texWall->load_texture("./data/wall.tga", NULL);

	texGrass = new Texture();
	texGrass->load_texture("./data/grass.tga", NULL);

	texMinimap = new Texture();
	texMinimap->load_texture("./data/labyrinth1.tga", "./data/labyrinth1mask.tga");
	
	texPlayer = new Texture();
	texPlayer->load_texture("./data/player.tga", "./data/playermask.tga");

	cam = new CAMERA();
	
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



	angleX -= 0.01*(float)inp->Yrelmouse;
	angleY -= 0.01*(float)inp->Xrelmouse;

	//if ((angleX - 0.01*(float)inp->Yrelmouse) - oldAngleX > 0)			angleX -= 0.01*(float)inp->Yrelmouse;
	//else angleX += 0.01*(float)inp->Yrelmouse;

	if (angleY > 360) angleY -= 360;
	if (angleY < 0) angleY += 360;
	if (angleX > 60) angleX = 60;
	if (angleX < -60) angleX = -60;

	cam->update(point(posX, posY, posZ), angleX, angleY, angleZ);
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
	char res[200] = "";
	sprintf(angleXArray, "%f", (float)inp->Yrelmouse);
	sprintf(angleYArray, "%f", angleY);
	strcat(res, "x: ");
	strcat(res, angleXArray);
	//if (angleX - oldAngleX > 0)			strcat(res, " (+)");
	//else if (angleX - oldAngleX < 0)	strcat(res, " (-)");
	strcat(res, "; y: ");
	strcat(res, angleYArray);
	write_2_screen(res);
	glPopMatrix();
	// End print debug info


	

	


	

	oldAngleX = angleX;


	generateLabyrinth();
	minimap();

	


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

