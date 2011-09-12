#pragma once
#include <iostream>
#include <SDL.h>				//-->	Inclusion du SDK 2D SDL.
#include <gl.h>					//-->	Inclusion de l'API 3D OpenGL
#include <glu.h>
#include <math.h>
#define PI       3.14159265358979323846

struct Vecteur
{
	double x;
	double y;
	double z;
};
void InitialiseVecteur(Vecteur & vecteur, double x, double y, double z);

class Joueur
{
public:
	Joueur(void);
	~Joueur(void);
	void ChangePosition(int i, int j);
	void Actualiser(void);
	void ActualiserVuePerspective(void);
	void ActualiserVueHaut(void);
	Vecteur Position;
	double AngleHorizontal;
	double AngleVertical;
	bool VueDeHaut;
	const double TAILLE_VUE;
	const double MAX_ANGLE_VERTICAL;
	const double TAILLE_CASE;
	const double PAS_ANGLE;
	const double PAS_VECTEUR;
private:
	Vecteur Haut;
};

inline double Sin(double x)
{
	return sin((PI / 180) * x);
}

inline double Cos(double x)
{
	return cos((PI / 180) * x);
}

//
