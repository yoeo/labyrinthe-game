#include "Jeu.h"

Jeu::Jeu(void)
{
	SDL_Init(SDL_INIT_VIDEO);					//		Initialise SDL.
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	TTF_Init();													//		Initialise TTF.
//--------------------------------------------------------------------> Réglages de l'application.
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);									//		Affiche le curseur.
	SDL_EnableKeyRepeat(10, 10);								//		Active la répétition des touche.
	this->LargeurEcran = 800;									//		Dimentions de la fenêtre:Largeur.
	this->HauteurEcran = 600;									//		Dimentions de la fenêtre:Longueur.
	this->ImageParSec = 30;										//		Nomgre d'images par seconde (IPS != 0).
	this->EnCours = true;										//		Intérupteur de l'application.
//--------------------------------------------------------------------> Ouverture de la fenêtre.
	SDL_WM_SetIcon(IMG_Load("Images/mon_icone.png"), NULL);		//		Charge l'icone de la fenêtre.
	this->Ecran = SDL_SetVideoMode(								//		Ouvre la fenêtre.
		this->LargeurEcran, this->HauteurEcran, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);// | SDL_FULLSCREEN);
	SDL_WM_SetCaption("Le Labyrinthe", NULL);					//		Affiche le titre de la fenêtre.
//--------------------------------------------------------------------> Initialise GL.
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);									//		Enable Texture Mapping
	glEnable(GL_BLEND);											//		active transparence
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5,0.5,1.0,0.0);								//		Sélection de la couleur de fond.
	glMatrixMode(GL_MODELVIEW);
//--------------------------------------------------------------------> Initialise le Joueur
	this->ChargerPolice();
	this->Labyrinthe.ChargePlateau("Stages/stage1.pla");
	this->Camera.ChangePosition(this->Labyrinthe.PointDepart[0], this->Labyrinthe.PointDepart[1]);
	this->Camera.Actualiser();

}
//
//	Terminer le jeu
//
Jeu::~Jeu(void)
{
	TTF_Quit();													//		Décharge (arrête) TTF.
	SDL_Quit();													//		Décharge (arrête) SDL.
}
//
//	Charge la police de caractère.
//
void Jeu::ChargerPolice(void)
{
/*
	int depart = 32;
	int nombreCaracteres = 192;
	int i;
	char caracteres[depart + nombreCaracteres];

	for (i = depart; i < nombreCaracteres; i++)
	{
		caracteres[i] = (char)i;
		std::cout << (char)i;
	}
//	caracteres[0] = ' ';
	caracteres[i] = '\0';
std::cout << caracteres;
	PoliceFichier = TTF_OpenFont("Font/Computerfont.ttf", 65);	//		Charge la police par défaut.
	PoliceCouleur.r = 10;
	PoliceCouleur.g = 10;
	PoliceCouleur.b = 20;

	PoliceSurface = TTF_RenderText_Blended(PoliceFichier, caracteres, PoliceCouleur);			//		Ecrit du texte.
	CreateGLTexture(PoliceSurface, PoliceTexture);
*/
}
/*
{
	int n = 16;
	int m = 16;
	int k = 0;
	char caracteres[n * m + n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			caracteres[k++] = (char)(i * m + j);
		}
		caracteres[k++] = '\n';
	}
	caracteres[0] = ' ';
	caracteres[k] = '\0';
std::cout << caracteres;
	PoliceFichier = TTF_OpenFont("Font/Computerfont.ttf", 65);	//		Charge la police par défaut.
	PoliceCouleur.r = 10;
	PoliceCouleur.g = 10;
	PoliceCouleur.b = 20;

	PoliceSurface = TTF_RenderText_Blended(PoliceFichier, caracteres, PoliceCouleur);			//		Ecrit du texte.
	CreateGLTexture(PoliceSurface, PoliceTexture);
}*/
//
//	Exécute le jeu.
//
void Jeu::LanceApplication(void)
{
	Uint32 tempsEcoule;
	Uint32 tempsDeDepart;
	Uint32 intervalle = 1000 / this->ImageParSec;

	while (this->EnCours)
	{
        tempsDeDepart = SDL_GetTicks();

		this->EnCours = this->CaptureEvenement();
		this->Dessine();

        tempsEcoule = SDL_GetTicks() - tempsDeDepart;
        if (tempsEcoule < intervalle)
            SDL_Delay(intervalle - tempsEcoule);
	}
}
//
//	Capture les evenements.
//
bool Jeu::CaptureEvenement(void)
{
	static bool click = false;
	SDL_Event event;										//		Capture un évenement et l'analyse


	SDL_PollEvent(& event);
	switch(event.type)
	{
	case SDL_QUIT:											//		L'utilisateur a fermé la fenêtre.
		return false;
		break;
	case SDL_MOUSEMOTION:
		return this->EvenementSouris(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
		click = true;
		break;
	case SDL_MOUSEBUTTONUP:
		click = false;
		break;
	case SDL_KEYDOWN:
		return this->EvenementClavier(event);
		break;
	}

	if (click)
	{
		this->CameraAvance();
		this->Camera.Actualiser();
	}
	return true;
}

bool Jeu::EvenementClavier(SDL_Event & event)
{
		switch(event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return false;
			break;
		case SDLK_DOWN :
			this->Camera.AngleVertical += this->Camera.PAS_ANGLE;
			this->Camera.Actualiser();
			break;
		case SDLK_UP :
			this->Camera.AngleVertical -= this->Camera.PAS_ANGLE;
			this->Camera.Actualiser();
			break;
		case SDLK_LEFT :
			this->Camera.AngleHorizontal += this->Camera.PAS_ANGLE;
			this->Camera.Actualiser();
			break;
		case SDLK_RIGHT :
			this->Camera.AngleHorizontal -= this->Camera.PAS_ANGLE;
			this->Camera.Actualiser();
			break;
		case SDLK_SPACE:
			this->CameraAvance();
			this->Camera.Actualiser();
			break;
		case SDLK_b:
			this->Camera.VueDeHaut = !this->Camera.VueDeHaut;
			break;
		default:
			break;
		}
		return true;
}

bool Jeu::EvenementSouris(SDL_Event &event)
{
	this->Camera.AngleHorizontal += -event.motion.xrel;
	this->Camera.AngleVertical   +=  event.motion.yrel;
	this->Camera.Actualiser();

	return true;
}
/*
bool Jeu::EvenementSouris(SDL_Event &event)
{
	static bool retours = true;
	if (retours)
	{
		this->Camera.AngleHorizontal += -event.motion.xrel;
		this->Camera.AngleVertical   +=  event.motion.yrel;
		this->Camera.Actualiser();
	}
	else
		retours = true;

	if ((event.motion.x > this->LargeurEcran - 100) ||
		(event.motion.y > this->HauteurEcran - 100) ||
		(event.motion.x < 100) ||
		(event.motion.y < 100))
	{
		SDL_WarpMouse(this->LargeurEcran / 2, this->HauteurEcran / 2);
		retours = false;
	}
	return true;
}
*/
void Jeu::CameraAvance(void)
{
	double i = this->Camera.Position.y + this->Camera.PAS_VECTEUR * Sin(this->Camera.AngleHorizontal);
	double j = this->Camera.Position.x + this->Camera.PAS_VECTEUR * Cos(this->Camera.AngleHorizontal);
	double demiVue = this->Camera.TAILLE_VUE / 2;

	bool existeCoinOccupe = !(this->Labyrinthe.EstCaseLibre(i - demiVue, j - demiVue) &&
							  this->Labyrinthe.EstCaseLibre(i - demiVue, j + demiVue) &&
							  this->Labyrinthe.EstCaseLibre(i + demiVue, j - demiVue) &&
							  this->Labyrinthe.EstCaseLibre(i + demiVue, j + demiVue) );
	bool dansCoinX = (((j - (int)j) < demiVue) || ((j - (int)j) > 1 - demiVue)) &&
					 (((this->Camera.Position.y - (int)this->Camera.Position.y) < demiVue) ||
					 (( this->Camera.Position.y - (int)this->Camera.Position.y) > 1 - demiVue));
	bool dansCoinY = (((i - (int)i) < demiVue) || ((i - (int)i) > 1 - demiVue)) &&
					 (((this->Camera.Position.x - (int)this->Camera.Position.x) < demiVue) ||
					 (( this->Camera.Position.x - (int)this->Camera.Position.x) > 1 - demiVue));
	if ((i - (this->Camera.TAILLE_VUE / 2) >= 0) &&
		!(existeCoinOccupe && dansCoinY) &&
		this->Labyrinthe.EstCaseLibre(i + demiVue, j) &&
		this->Labyrinthe.EstCaseLibre(i - demiVue, j) )
	{
		this->Camera.Position.y = i;
	}
	if ((j - (this->Camera.TAILLE_VUE / 2) >= 0) &&
		!(existeCoinOccupe && dansCoinX) &&
		this->Labyrinthe.EstCaseLibre(i, j + demiVue) &&
		this->Labyrinthe.EstCaseLibre(i, j - demiVue) )
	{
		this->Camera.Position.x = j;
	}
}

//
//	Déssine sur l'écran.
//
void Jeu::Dessine(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

	Labyrinthe.DessinePlateau();
	DessineMenu();

	glFinish();
	SDL_GL_SwapBuffers();
}

void Jeu::DessineMenu(void)
{
/*
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, PoliceTexture);
	glTexCoord2d(0, 0);	glVertex3d(-1, -1, -1);
	glTexCoord2d(0, 1);	glVertex3d(-1, 5, -1);
	glTexCoord2d(1, 1);	glVertex3d(5, 5, 5);
	glTexCoord2d(1, 0);	glVertex3d(-1, -1, 5);
*/
}

void Jeu::EcrireTexte2D(std::string texte, double x, double y)
{

}
//
