#include "Plateau.h"
#include <exception>

Plateau::Plateau(void) : VALEUR_CASE_PLEINE(0), VALEUR_CASE_LIBRE(1),
						 VALEUR_CASE_DEPART(2), VALEUR_CASE_ARRIVE(3)
{
	this->PointDepart[0] = 0;
	this->PointDepart[1] = 0;
	this->PointArrive[0] = 0;
	this->PointArrive[1] = 0;
}

Plateau::~Plateau(void)
{
	if (this->Cases != NULL)
		delete[] this->Cases;
}

void Plateau::ChargePlateau(int longueur, int largeur, int * cases, char * imageMur, char * imageSol,
							double * couleurMurHaut, double * couleurMurBas, double * couleurSol)
{
	this->LargeurPlateau = largeur;
	this->LongueurPlateau = longueur;
	this->Cases = new int[this->LargeurPlateau * this->LongueurPlateau];
	for (int i = 0; i < this->LongueurPlateau; i++)
	{
		for (int j = 0; j < this->LargeurPlateau; j++)
		{
			this->Cases[i * this->LargeurPlateau + j] = cases[i * this->LargeurPlateau + j];
			if (this->ValeurCase(i, j) == this->VALEUR_CASE_DEPART)
			{
				this->PointDepart[0] = i;
				this->PointDepart[1] = j;
			}
			if (this->ValeurCase(i, j) == this->VALEUR_CASE_ARRIVE)
			{
				this->PointArrive[0] = i;
				this->PointArrive[1] = j;
			}
		}
	}

	try
	{
		if (imageMur != NULL)
			CreateGLTexture(imageMur, TextureMur);
		if (imageSol != NULL)
			CreateGLTexture(imageSol, TextureSol);
	}
	catch(std::exception &e)
	{
		std::stringstream message;
		message << "Impossible de charger l'image du plateau car : \" " << e.what() << " \"";
		std::exception a;

		throw message.str();
	}

	if (couleurMurBas != NULL)
		for (int i = 0; i < 4; i++)
			this->CouleurMurBas[i] = couleurMurBas[i];
	else
		for (int i = 0; i < 4; i++)
			this->CouleurMurBas[i] = 0;

	if (couleurMurHaut != NULL)
		for (int i = 0; i < 4; i++)
			this->CouleurMurHaut[i] = couleurMurHaut[i];
	else
		for (int i = 0; i < 4; i++)
			this->CouleurMurHaut[i] = 0;

	if (couleurSol != NULL)
		for (int i = 0; i < 4; i++)
			this->CouleurSol[i] = couleurSol[i];
	else
		for (int i = 0; i < 4; i++)
			this->CouleurSol[i] = 0;
}

void Plateau::ChargePlateau(char * fichier)
{
	std::ifstream descriptionPlateau;
	std::stringstream messageErreur;
	messageErreur << "Impossible de charger le plateau a partir du fichier : \" " << fichier << " \". ";

	descriptionPlateau.open(fichier);
	if (!descriptionPlateau.is_open())
	{
		messageErreur << "Le fichier specifie est introuvable.";
		throw messageErreur.str();
	}

	int largeur = 0;
	int hauteur = 0;
	int * points = NULL;
	double * couleurSol = NULL;
	double * couleurMurHaut = NULL;
	double * couleurMurBas = NULL;
	std::string imageMur = "";
	std::string imageSol = "";

	while (!descriptionPlateau.eof())
	{
		std::string ligne;
		char recupereLigne[100];
		descriptionPlateau.getline(recupereLigne, 100);
		ligne.assign(recupereLigne);
		if (ligne.find("=") != std::string::npos)
		{
			int position = ligne.find("=");
			std::string element(ligne.substr(0, position));;
			std::stringstream valeur(ligne.substr(position + 1, ligne.length() - position - 1));

			if (element.find("largeur") != std::string::npos)
			{
				valeur >> largeur;
				if (largeur <= 0)
				{
					messageErreur << element << element.find("largeur") << "La largeur est inadequate, largeur = " << largeur;
					throw messageErreur.str();
				}

			}
			else if (element.find("hauteur") != std::string::npos)
			{
				valeur >> hauteur;
				if (hauteur <= 0)
				{
					messageErreur << "La hauteur est inadequate, hauteur = " << hauteur;
					throw messageErreur.str();
				}

			}
			else if (element.find("image-mur") != std::string::npos)
			{
				valeur >> imageMur;
			}
			else if (element.find("image-sol") != std::string::npos)
			{
				valeur >> imageSol;
			}
			else if (element.find("points") != std::string::npos)
			{
				points = new int[largeur * hauteur];

				for (int i = 0; i < largeur * hauteur; i++)
				{
					if (descriptionPlateau.eof())
					{
						messageErreur << "Le nombre de cases specifie est trop reduit,";
						messageErreur << "nombre = " << i << " mais devrai etre " << largeur * hauteur;
						throw messageErreur.str();
					}

					int cellule;
					do
					{
						cellule = descriptionPlateau.get() - '0';
					}
					while ((cellule < 0) || (cellule > 9));
					points[i] = cellule;
				}
			}
			else if (element.find("couleur-sol") != std::string::npos)
			{
				couleurSol = new double[4];
				valeur >> couleurSol[0] >> couleurSol[1] >> couleurSol[2] >> couleurSol[3];
			}
			else if (element.find("couleur-mur-haut") != std::string::npos)
			{
				couleurMurHaut = new double[4];
				valeur >> couleurMurHaut[0] >> couleurMurHaut[1] >> couleurMurHaut[2] >> couleurMurHaut[3];
			}
			else if (element.find("couleur-mur-bas") != std::string::npos)
			{
				couleurMurBas = new double[4];
				valeur >> couleurMurBas[0] >> couleurMurBas[1] >> couleurMurBas[2] >> couleurMurBas[3];
			}
		}
	}
	this->ChargePlateau(hauteur, largeur, points, (char *)imageMur.c_str(), (char *)imageSol.c_str(),
		couleurMurHaut, couleurMurBas, couleurSol);

	if (points != NULL)
		delete[] points;
	if (couleurSol != NULL)
		delete[] couleurSol;
	if (couleurMurHaut != NULL)
		delete[] couleurMurHaut;
	if (couleurMurBas != NULL)
		delete[] couleurMurBas;
}

void Plateau::DessinePlateau(void)
{
	glLoadIdentity();
	this->DessineFin(this->PointArrive[0], this->PointArrive[1]);
	for (int i = 0; i < this->LongueurPlateau; i++)
	{
		for (int j = 0; j < this->LargeurPlateau; j++)
		{
			this->DessineSol(j, j + 1, i, i + 1);
			if (this->EstCaseLibre(i, j))
			{
				int x1 = 0;
				int x2 = 0;
				int y1 = 0;
				int y2 = 0;
				if (!this->EstCaseLibre(i, j - 1))
				{
					x1 = j;
					x2 = j;
					y1 = i;
					y2 = i + 1;
					this->DessineMur(x1, x2, y1, y2);
				}
				if (!this->EstCaseLibre(i - 1, j))
				{
					x1 = j;
					x2 = j + 1;
					y1 = i;
					y2 = i;
					this->DessineMur(x1, x2, y1, y2);
				}
				if (!this->EstCaseLibre(i, j + 1))
				{
					x1 = j + 1;
					x2 = j + 1;
					y1 = i;
					y2 = i + 1;
					this->DessineMur(x1, x2, y1, y2);
				}
				if (!this->EstCaseLibre(i + 1, j))
				{
					x1 = j;
					x2 = j + 1;
					y1 = i + 1;
					y2 = i + 1;
					this->DessineMur(x1, x2, y1, y2);
				}
			}
		}
	}
}

void Plateau::DessineMur(int x1, int x2, int y1, int y2)
{
	glBindTexture(GL_TEXTURE_2D, this->TextureMur);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);		glColor4dv(this->CouleurMurBas);		glVertex3d(x1, y1, 0);
	glTexCoord2d(0, 1);		glColor4dv(this->CouleurMurBas);		glVertex3d(x2, y2, 0);
	glTexCoord2d(0, 0);		glColor4dv(this->CouleurMurHaut);		glVertex3d(x2, y2, 1);
	glTexCoord2d(1, 0);		glColor4dv(this->CouleurMurHaut);		glVertex3d(x1, y1, 1);
	glEnd();
}

void Plateau::DessineSol(int x1, int x2, int y1, int y2)
{
	glBindTexture(GL_TEXTURE_2D, this->TextureSol);
	glColor4dv(this->CouleurSol);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 1);		glVertex3d(x1, y1, 0);
	glTexCoord2d(0, 1);		glVertex3d(x2, y1, 0);
	glTexCoord2d(0, 0);		glVertex3d(x2, y2, 0);
	glTexCoord2d(1, 0);		glVertex3d(x1, y2, 0);
	glEnd();
}

void Plateau::DessineFin(int i, int j)
{
	glColor4d(0.8, 0.4, 0.2, 1);
	static double variation = 0;
	variation += 0.01;

	GLUquadricObj *obj = gluNewQuadric();
	gluQuadricNormals(obj, GL_SMOOTH);
	gluQuadricDrawStyle(obj, (GLenum)GLU_FILL);
	glPushMatrix();
	glTranslated(j + 0.5, i + 0.5, 0.5);
	gluSphere(obj, 0.15 * abs(sin(variation)) + 0.15, 20, 20);
	glPopMatrix();
	gluDeleteQuadric(obj);

/*
//	Bézier
	static double baseZ = 0;
	baseZ += 0.01;
	int maxZ = 20;
	double pas = 0.25;
	i += 0.5;
	j += 0.5;
	glColor4d(0.8, 0.6, 0.5, 0.1);
	for (double z = baseZ; z < maxZ + baseZ; z += pas)
	{
		glBegin(GL_QUADS);

		glVertex3d(j + abs(cos(z)), i, z - baseZ);
		glVertex3d(j, i + abs(cos(z)), z - baseZ);
		glVertex3d(j, i + abs(cos(z + pas)), z + pas - baseZ);
		glVertex3d(j + abs(cos(z + pas)), i, z + pas - baseZ);

		glVertex3d(j + cos(z), i, z - baseZ);
		glVertex3d(j, i - cos(z), z - baseZ);
		glVertex3d(j, i - cos(z + pas), z + pas - baseZ);
		glVertex3d(j + cos(z + pas), i, z + pas - baseZ);

		glVertex3d(j - cos(z), i, z - baseZ);
		glVertex3d(j, i + cos(z), z - baseZ);
		glVertex3d(j, i + cos(z + pas), z + pas - baseZ);
		glVertex3d(j - cos(z + pas), i, z + pas - baseZ);

		glVertex3d(j - cos(z), i, z - baseZ);
		glVertex3d(j, i - cos(z), z - baseZ);
		glVertex3d(j, i - cos(z + pas), z + pas - baseZ);
		glVertex3d(j - cos(z + pas), i, z + pas - baseZ);

		glEnd();
	}
*/
}

bool Plateau::EstCaseLibre(int i, int j)
{
	return (this->ValeurCase(i, j) != this->VALEUR_CASE_PLEINE);
}


int  Plateau::ValeurCase(int i, int j)
{
	if ((i >= 0) && (i < this->LongueurPlateau) && (j >= 0) && (j < this->LargeurPlateau))
		return this->Cases[i * this->LargeurPlateau + j];
	else
		return 0;
}
//
