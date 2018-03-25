//============================================================================
// Name        : Langton's.cpp
// Author      : D01000100
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <SDL2/SDL.h>
//#include <conio.h>
#include <time.h>

using namespace std;

#define Width 500
#define Height 500

//################################ Class Ant #########################################################

enum dirs{eUp,eRight,eDown,eLeft};

class Ant{
public:
		Ant(int,int);
		void step();
		void printWorld();
		bool getWorld(int,int);
		void getPos(int*posX,int*posY){*posX=x;*posY=y;};
		void setWorld(int posX,int posY,bool state){world[posX][posY]=state;};
		void invertDir(){invert = !invert;}
private:
		int x;
		int y;
		int lastX;
		int lastY;
		int dir;
		int generation;
		bool world[Width][Height];
		bool invert;
};

Ant::Ant(int posX,int posY){
		x=posX;
		y=posY;
		lastX=x;
		lastY=y;
		dir=eUp;
		generation=0;
		invert=true;
		for(int y=0;y<Height;y++){
				for(int x=0;x<Width;x++){
						world[x][y]=false;
				}
		}
}

void Ant::step(){
		// Backup last pos
		lastX=x;lastY=y;
		// Look under the ant
		bool state=world[x][y];
		// Invert state of the cell
		world[x][y]=(!state);
		// Change direction
			if(state){
					// Turn right
					dir=(dir==eLeft)?eUp:dir+1;
			}
			else{
					// Turn left
					dir=(dir==eUp)?eLeft:dir-1;
			}
		if(invert){
			switch(dir){
		case eUp:{
			dir=eDown;break;
			}
		case eRight:{
			dir=eLeft;break;
			}
		case eDown:{
			dir=eUp;break;
			}
		case eLeft:{
			dir=eRight;break;
			}

			}
		}
		// Step forward
		switch(dir){
				case eUp:{
						y--;
						break;
				}
				case eRight:{
						x++;
						break;
				}
				case eDown:{
						y++;
						break;
				}
				case eLeft:{
						x--;
						break;
				}
		}
		//Wrap around
		if(x<0)x=Width-1;
		else if(x>Width-1)x=0;
		if(y<0)y=Height-1;
		else if(y>Height-1)y=0;
//Augmente l'indice de génération
		generation++;
}

void Ant::printWorld(){
		cout<<"Langton's Ant : generation "<<generation<<endl;
		for(int y=0;y<Height;y++){
				for(int x=0;x<Width;x++){
						if(world[x][y])cout<<"#";
						else cout<<" ";
				}
				cout<<endl;
		}
}

bool Ant::getWorld(int x,int y){
		return world[x][y];
}

//#########################################################################################

void drawWorld(SDL_Renderer*renderer,Ant&ant,int w,int h){
		int x,y;
		ant.getPos(&x,&y);
		if(ant.getWorld(x,y)){
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
		}
		else{
				SDL_SetRenderDrawColor(renderer,0,0,0,0);
		}
		SDL_Rect rect;
		rect.x=x*w/Width;
		rect.y=y*h/Height;
		rect.w=w/Width;
		rect.h=h/Height;
		SDL_RenderFillRect(renderer,&rect);

		SDL_RenderPresent(renderer);
}

void updateWorld(SDL_Renderer*renderer,Ant&ant,int w,int h){
		for(int y=0;y<h;y++){
				for(int x=0;x<w;x++){
						if(ant.getWorld(x,y)){
								SDL_SetRenderDrawColor(renderer,255,255,255,255);
						}
						else{
								SDL_SetRenderDrawColor(renderer,0,0,0,0);
						}
						SDL_Rect rect;
						rect.x=x*w/Width;
						rect.y=y*h/Height;
						rect.w=w/Width;
						rect.h=h/Height;
						SDL_RenderFillRect(renderer,&rect);
				}
		}

		SDL_RenderPresent(renderer);
}

int map(int x, int in_min, int in_max, int out_min, int out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void modifyWorld(SDL_Renderer*renderer,Ant&ant,int w,int h,int mouseX,int mouseY,bool state){
		ant.setWorld(mouseX*Width/w,mouseY*Height/h,state);
		//cout<<"Mouse:"<<mouseX*Width/w<<","<<mouseY*Height/h<<endl;
		if(state){
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
		}
		else{
				SDL_SetRenderDrawColor(renderer,0,0,0,0);
		}
		SDL_Rect rect;
		rect.x=(mouseX*Width/w)*w/Width;
		rect.y=(mouseY*Height/h)*h/Height;
		/*
		rect.x=mouseX*w/Width;
		rect.y=mouseY*h/Height;
		*/
		rect.w=w/Width;
		rect.h=h/Height;

		SDL_RenderFillRect(renderer,&rect);

		SDL_RenderPresent(renderer);
}

void loop(SDL_Renderer*renderer,int w,int h){
		Ant ant(Width/2,Height/2);
		SDL_Event ev;
		bool autoStep=false;
		bool clicked=false;
		bool state=false;
		unsigned delayVal=0;
		for(;;){

				if(autoStep)ant.step();

				drawWorld(renderer,ant,w,h);

				while(SDL_PollEvent(&ev)){
						switch(ev.type){
								case SDL_QUIT:{
										return;
								}
								case SDL_KEYDOWN:{
										switch(ev.key.keysym.sym){
												case SDLK_ESCAPE: return;
												case 'q':return;
												case 'w':{autoStep=!autoStep;break;}
												case 'e':{autoStep=false;ant.step();break;}
												case 'a':{delayVal=(delayVal==0)?0:delayVal-1;break;}
												case 's':{delayVal++;break;}
												case 'x':{for(int i=0;i<10000;i++)ant.step();updateWorld(renderer,ant,w,h);break;}
			case 'd':{ant.invertDir();break;}
												default:{
														break;
												}
										}
										break;
								}
								case SDL_MOUSEBUTTONDOWN:{
										state=(ev.button.button==SDL_BUTTON_LEFT);
										clicked=true;
										break;
								}
								case SDL_MOUSEBUTTONUP:{
										state=(ev.button.button==SDL_BUTTON_LEFT);
										clicked=false;
										break;
								}
								case SDL_MOUSEMOTION:{
										if(clicked){
												int mouseX=ev.motion.x;
												int mouseY=ev.motion.y;
												modifyWorld(renderer,ant,w,h,(int)mouseX,(int)mouseY,state);
										}
										break;
								}
								default: break;
						}
				}

				SDL_Delay(delayVal);
		}
}

int main(int argc, char** argv) {
		srand(time(NULL));
		//freopen("CON", "w", stdout); // For Windows
		const int width=500,height=500;
		SDL_Window* window = SDL_CreateWindow
		(
				"Langton's ant", SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width,
				height,
				SDL_WINDOW_SHOWN
				);

		SDL_Renderer*renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

		//Black screen
		SDL_SetRenderDrawColor(renderer,0,0,0,0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer,255,255,255,255);

		loop(renderer,width,height);

		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
}
