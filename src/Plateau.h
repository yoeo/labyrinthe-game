#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <SDL.h>				//-->	Inclusion du SDK 2D SDL.
#include <gl.h>					//-->	Inclusion de l'API 3D OpenGL
#include <glu.h>				//-->	Inclusion de l'API 3D OpenGL
#include "SDL_GL.h"

class Plateau
{
public:
	Plateau(void);
	~Plateau(void);
	void ChargePlateau(int longueur, int largeur, int * cases, char * imageMur = NULL, char * imageSol = NULL,
		double * couleurMurHaut = NULL, double * couleurMurBas = NULL, double * couleurSol = NULL);
	void ChargePlateau(char * fichier);
	void DessinePlateau(void);
	bool EstCaseLibre(int i, int j);
	int PointDepart[2];
	int PointArrive[2];
private:
	void DessineMur(int x1, int x2, int y1, int y2);
	void DessineSol(int x1, int x2, int y1, int y2);
	void DessineFin(int i, int j);
	int ValeurCase(int i, int j);
	int LargeurPlateau;
	int LongueurPlateau;
	int * Cases;
	GLuint TextureMur;
	GLuint TextureSol;
	double CouleurMurHaut[4];
	double CouleurMurBas[4];
	double CouleurSol[4];
	const int VALEUR_CASE_PLEINE;
	const int VALEUR_CASE_LIBRE;
	const int VALEUR_CASE_DEPART;
	const int VALEUR_CASE_ARRIVE;
};
//
