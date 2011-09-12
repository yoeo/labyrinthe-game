#pragma once
#include <iostream>
#include <math.h>
#include <SDL.h>				//-->	Inclusion du SDK 2D SDL.
#include <SDL_Image.h>			//		Compl�ment de SDL qui perm�t de charger plus des formats d'images.
#include <SDL_ttf.h>			//		Compl�ment qui permet  d'�crire du text.
#include <gl.h>					//-->	Inclusion de l'API 3D OpenGL

#include "plateau.h"
#include "joueur.h"

class Jeu
{
public:
	Jeu(void);
	~Jeu(void);
	void LanceApplication(void);
private:
	void ChargerPolice(void);
	bool CaptureEvenement(void);
	bool EvenementSouris(SDL_Event & event);
	bool EvenementClavier(SDL_Event & event);
	void Dessine(void);
	void DessineMenu(void);
	void CameraAvance(void);
	void EcrireTexte2D(std::string texte, double x, double y);
	//	Les variables.
	SDL_Surface * Ecran;			//		Surface repr�santant la fen�tre.
	int LargeurEcran;			//		Largeur de la fen�tre.
	int HauteurEcran;			//		Hauteur de la fen�tre.
	bool EnCours;				//		Int�rupteur de l'application.
	int ImageParSec;			//		Nombre d'images par seconde.
	//	Variables pour �criture du texte.
	TTF_Font * PoliceFichier;		//		Police de l'application.
	SDL_Surface * PoliceSurface;		//		Image des mots �crits.
	SDL_Color PoliceCouleur;			//		Couleur de la police.
	GLuint PoliceTexture;
	//	Objets du jeu.
	Plateau Labyrinthe;
	Joueur Camera;
};
//
