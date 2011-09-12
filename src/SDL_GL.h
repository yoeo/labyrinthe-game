#include <SDL.h>
#include <GL.h>



void inline SDL_CopyPixel(SDL_Surface* src, int srcX, int srcY, SDL_Surface* dst, int dstX, int dstY);
SDL_Surface * SDL_Recadre(SDL_Surface * img, int largeur, int hauteur);
void CreateGLTexture(char * file, GLuint & glTexture);
void CreateGLTexture(SDL_Surface * sdlImg, GLuint & glTexture);
