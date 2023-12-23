#ifndef LTEXTURE_H
#define LTEXTURE_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		//bool loadFromFile( std::string path );
		
		//Creates image from font string
		//bool loadFromRenderedText( std::string textureText, int fontsize);

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* dstrect = NULL);
		
		//Gets image dimensions
		int getWidth();
		int getHeight();
	//	bool loadFromRenderedText( std::string textureText, int fontsize);
//		virtual bool loadFromFile( std::string path ){return 0;}
//		virtual bool loadFromRenderedText( std::string textureText, int fontsize){return 0;}	
	protected:
		//The actual hardware texture
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
		//Image dimensions
	private:
		//int mWidth;
		//int mHeight;
};
//type
class Text: public LTexture{
	public:
//		Text();
		bool loadFromRenderedText( std::string textureText, int fontsize);	
};

class Image: public LTexture{
	public:
		Image();
		bool loadFromFile( std::string path );
		
};


#endif

