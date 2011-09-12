#include "SDL_GL.h"
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL_opengl.h>
#include <GL.h>

#include <iostream>
#include <sstream>

void inline SDL_CopyPixel(SDL_Surface* src, int srcX, int srcY, SDL_Surface* dst, int dstX, int dstY)
{
    int bpp = src->format->BytesPerPixel;

	Uint8 * srcPixel = (Uint8 *)src->pixels + srcY * src->pitch + srcX * bpp;
    Uint8 * dstPixel = (Uint8 *)dst->pixels + dstY * dst->pitch + dstX * bpp;

    switch(bpp)
	{
        case 1:
            *dstPixel = *srcPixel;
            break;

        case 2:
            *(Uint16 *)dstPixel = *(Uint16 *)srcPixel;
            break;

        case 3:
            /* L'opération sur 24 bits est lente */
            dstPixel[0] = srcPixel[0];
            dstPixel[1] = srcPixel[1];
            dstPixel[2] = srcPixel[2];
            break;

        case 4:
            *(Uint32 *)dstPixel = *(Uint32 *)srcPixel;
            break;
    }
}


SDL_Surface * SDL_Recadre(SDL_Surface * img, int largeur, int hauteur)
{
	SDL_Surface * rsl = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, img->format->BitsPerPixel, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask);


	double pasX = (double)img->w / largeur;
	double pasY = (double)img->h / hauteur;

	for(int rsX = 0; rsX < largeur; rsX++)
	{
		int imX = (int)(rsX * pasX);
		for(int rsY = 0; rsY < hauteur; rsY++)
		{
			int imY = (int)(rsY * pasY);
			SDL_CopyPixel(img, imX, imY, rsl, rsX, rsY);
		}
	}
	return rsl;
}

void CreateGLTexture(char * file, GLuint & glTexture)
{
	GLenum glError;
	SDL_Surface * sdlImg = NULL;

	if((glError = glGetError()))
	{
		std::ostringstream s;
		s << "Erreur de OpenGL avant le chargement de l\'image : " << file << std::endl << " Détail : " << glError;
		throw s.str();
	}
	sdlImg = IMG_Load(file);

	CreateGLTexture(sdlImg, glTexture);
}

void CreateGLTexture(SDL_Surface * sdlImg, GLuint & glTexture)
{
	GLenum glError;
	if (sdlImg)
	{
		if (( (sdlImg->w & (sdlImg->w - 1)) != 0 ) || ( (sdlImg->h & (sdlImg->h - 1)) != 0 ))		// Check that the image's width is a power of 2
		{
			const int TEXMAX = 512;
			int w = 2;
			int h = 2;

			while((w < sdlImg->w) && (w < TEXMAX))
				w *= 2;
			while((h < sdlImg->h) && (h < TEXMAX))
				h *= 2;

			SDL_Surface * tmp = SDL_Recadre(sdlImg, w, h);
			SDL_FreeSurface(sdlImg);
			sdlImg = tmp;
		}

		GLenum texFormat;
		switch(sdlImg->format->BytesPerPixel)			// get the number of channels in the SDL sdlImg
		{
		case 3:											// no alpha channel
			texFormat = (sdlImg->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
			break;

		case 4:											// contains an alpha channel
			texFormat = (sdlImg->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
			break;

		default:										// this error should not go unhandled
			std::ostringstream s;
			s << "Impossible de charger l\'image, La palette de couleurs n\'est pas suportée.";
			throw s.str();
		}

		glEnable( GL_TEXTURE_2D );
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glGenTextures( 1, &glTexture );					// Have OpenGL generate a texture object handle for us
		glBindTexture( GL_TEXTURE_2D, glTexture ); 		// Bind the texture object
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 		// Edit the texture object's image data using the information SDL_sdlImg gives us
		glTexImage2D( GL_TEXTURE_2D, 0, sdlImg->format->BytesPerPixel, sdlImg->w, sdlImg->h, 0, texFormat, GL_UNSIGNED_BYTE, sdlImg->pixels );

		SDL_FreeSurface(sdlImg);
		if((glError = glGetError()))
		{
			std::ostringstream s;
			s << "Erreur de OpenGL durant le chargement de l\'image, " << std::endl << "Code : " << glError;
			throw s.str();
		}
	}
	else
	{
		std::ostringstream s;
		s << "Impossible de charger l\'image, "<< std::endl << "Code : " << SDL_GetError();
		throw s.str();
	}
}

