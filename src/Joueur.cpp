#include "Joueur.h"

void InitialiseVecteur(Vecteur & vecteur, double x, double y, double z)
{
	vecteur.x = x;
	vecteur.y = y;
	vecteur.z = z;
}

Joueur::Joueur(void) :
	TAILLE_CASE(1), PAS_VECTEUR(0.03), PAS_ANGLE(3), MAX_ANGLE_VERTICAL(45), TAILLE_VUE(0.9)
{
	this->AngleHorizontal = 0;
	this->AngleVertical   = 0;
	InitialiseVecteur(this->Position , 0, 0, 0);
	InitialiseVecteur(this->Haut     , 0, 0, 1);
	this->VueDeHaut = false;
}

Joueur::~Joueur(void)
{
}

void Joueur::ChangePosition(int i, int j)
{
	InitialiseVecteur(this->Position,
					  j + this->TAILLE_CASE / 2,
					  i + this->TAILLE_CASE / 2,
					  this->TAILLE_CASE / 2);
}

void Joueur::Actualiser(void)
{
	if (this->VueDeHaut)
		this->ActualiserVueHaut();
	else
		this->ActualiserVuePerspective();
}

void Joueur::ActualiserVuePerspective(void)
{
	this->AngleVertical = (this->AngleVertical < -MAX_ANGLE_VERTICAL) ? -MAX_ANGLE_VERTICAL :
						 ((this->AngleVertical >  MAX_ANGLE_VERTICAL) ?  MAX_ANGLE_VERTICAL : this->AngleVertical);

	glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
	glLoadIdentity();														// Reset The Projection Matrix
	gluPerspective(45, 4 / 3, 1, 100);										// Calculate The Aspect Ratio Of The Window
	glScaled(1 / this->TAILLE_VUE, 1 / this->TAILLE_VUE, 1 / this->TAILLE_VUE);
	glTranslated(0, 0, -this->TAILLE_VUE);
	gluLookAt(this->Position.x, this->Position.y, this->Position.z,
		this->Position.x + Cos(this->AngleVertical) * Cos(this->AngleHorizontal),
		this->Position.y + Cos(this->AngleVertical) * Sin(this->AngleHorizontal),
		this->Position.z - Sin(this->AngleVertical),
		this->Haut.x, this->Haut.y, this->Haut.z);
	glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
}

void Joueur::ActualiserVueHaut(void)
{
	glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
	glLoadIdentity();														// Reset The Projection Matrix
	glOrtho(-1, 11, -1, 11, -5, 5);
	glRotated(10, 1, 0, 0);
	glRotated(-10, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
}
//
