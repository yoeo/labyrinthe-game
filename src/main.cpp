#include "main.h"

int main(int argc, char * argv[])
{
	try
	{
		Jeu jeu;
		jeu.LanceApplication();
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << std::endl << "Appuyez sur entree pour terminer..." << std::endl;
	}
	catch(const std::exception * e)
	{
		std::cerr << e->what() << std::endl;
		std::cerr << std::endl << "Appuyez sur entree pour terminer..." << std::endl;
	}
	catch(const std::string & message)
	{
		std::cerr << message << std::endl;
		std::cerr << std::endl << "Appuyez sur entree pour terminer..." << std::endl;
	}
	catch(...)
	{
		std::cerr << "Erreur inconnue" << std::endl;
		std::cerr << std::endl << "Appuyez sur entree pour terminer..." << std::endl;
	}
	return 0;
}
//
