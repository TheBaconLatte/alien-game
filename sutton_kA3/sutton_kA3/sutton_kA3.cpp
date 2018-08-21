// sutton_kA3.cpp : Defines the entry point for the console application.
//Kerrie-Ann Sutton
//14378311

// GLUT/Open_GL Single buffer example using some basic primitives in 2D
// OpenGL_Game.cpp
// Bare bones version of "Happy Vikings"
// C.Markham Jan 2015
#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Required for sound only (consider OpenAL),
#include <mmsystem.h> // Again only required for sound only
#include <freeglut.h> // GLUT OpenGL Utility Toolkit
#include "Sprite.h" // Prototype Sprite class
// 3 Sprites are used in the "game"
Sprite player;
Sprite background;
Sprite alien;
Sprite alien2;
// Score data and method
bool hit = false;
bool hit2 = false;
int score=0;
char score_text[20];


// Callback function called each time screen is to be redrawn (called by OS not you directly)
	static void display(void)
	{
		 // Keep game logic out of display()...
		 glClearColor(1.0f, 1.0f, 1.0f, 1.0f) ;
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer
		 background.drawSprite(); // Draw background at X,Y in a given direction
		 player.drawSprite(); // Draw player
		 alien.drawSprite();
		 alien2.drawSprite();

		 sprintf(score_text,"Score: %d",score); // Display text
		 Sprite::text(835,405,score_text);
		 glutSwapBuffers(); // Display buffer just drawn and then switch to draw on the other buffer
	}
// Callback function to allow dynamic resizing of runtime display
	static void resize(int width, int height)
	{
		 glViewport(0, 0, width, height);
		 Sprite::spriteModeOn(0,0); // (0,0) Causes orginal screen size to be reused
	}
// Callback to manage standard keys on the keyboard
// In this case 'q' or 'Q' causes program to terminate
	static void qwerty_keys(unsigned char key, int x, int y)
	{
			 switch (key)
			 {
				 case 'Q':
				 case 'q': glutLeaveMainLoop () ; break; // (Stop!)
				 default: break;
			 }
	}
// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
	static void function_keys(int key, int x, int y)
	{
		 switch (key)
		 {
			 case GLUT_KEY_LEFT:
				 player.frame_no = 0;
				 player.sprite_x_position-=10;
				 player.direction = true;
				 break;
			 case GLUT_KEY_RIGHT:
				 player.frame_no = 0;
				 player.sprite_x_position+=10;
				 player.direction = false;
				 break;
			 case GLUT_KEY_DOWN:
				 //player.frame_no = 1;
				 player.advance_frame(0,1);
				 break;
			 default: 
				 player.frame_no = 0;
				 break;
		 }
	}

// Game loop
// static void idle() // if using glutIdleFunc(idle); this line should replace the one below
	static void idle(int v) // Runs between frame updates in doublebuffer mode, game logic goes here
	{

		alien.advance_frame(0,2); // Cycle through 3 images
		alien.sprite_x_position+=5; //alien one moves from left to right
		alien2.advance_frame(0,2); // Cycle through 3 images
		alien2.sprite_x_position-=5; //alien 2 moves right to left
		
		if(alien.sprite_x_position>1024){
			alien.sprite_x_position = 0;
		}

		if(alien2.sprite_x_position<0){
			alien2.sprite_x_position = 1024;
		}

		if(player.collision(alien) && player.frame_no==1){
			hit = true;
		}

		if(hit == true){
			 score++; // If player collides with alien and the frame is 1, increase the score
			 hit = false;
			 alien.sprite_x_position=0;
		}

		if(player.collision(alien2) && player.frame_no==1){ //if user hits alien 2
			hit = true;
		}

		if(hit == true){
			 score++; // If player collides with alien and the frame is 1, increase the score
			 hit = false;
			 alien2.sprite_x_position=1024;
		}
		glutPostRedisplay(); // Send message to ask for screen to be redrawn
		glutTimerFunc(10,idle,0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
		
		//player.frame_no = 0;
	}
		/* Program entry point - starts here */


	int main(int argc, char *argv[])
	{
		 Sprite::spriteModeOn(1024,480); // Create a screen 640x480 to contain game
		// Start playing background music (uses Windows calls, there are other ways incluing OpenAL)
		 //PlaySound(L"Music.wav",NULL, SND_LOOP| SND_ASYNC);
		// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
		// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128
		 player.load("spaceship.bmp",2,1,320,0,164,328,false);
		 background.load("background.bmp",1,1,0,0,1024,480,true); //background: https://www.pixilart.com/8-bit-ethan
		 alien.load("spritesheet.bmp",3,1,0,0,64,128,false);
		 alien2.load("spritesheet.bmp",3,1,1024,0,64,128,true);
		 glutReshapeFunc(resize); // Called when window is resized (not required)
		 glutDisplayFunc(display); // Callback function to redraw, an event created by glutPostRedisplay()
		 glutKeyboardFunc(qwerty_keys); // QWERTY Keys
		 glutSpecialFunc(function_keys); // Cursor keys etc
		//glutIdleFunc(idle); // Called during idle time
		 glutTimerFunc(10,idle,0); // Use timer callback for more consistent timing

		 glutMainLoop(); // Start main loop running (go!)

		return EXIT_SUCCESS;
	} 