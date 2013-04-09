//
//  main.cpp
//  Ders 6
//
//  Created by Gurel Erceis on 3/17/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//
// Thing to install and test before writing any code
// glm
// assimp (http://assimp.sourceforge.net/main_downloads.html)

#include <iostream>
#include "Scene.h"
#include <GL\glew.h>
#include <GL/GLUT.h>
#include <math.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#define WINDOW_TITLE_PREFIX "Ders 7"

void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

unsigned FrameCount = 0;

int CurrentWidth = 800,
CurrentHeight = 600,
WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float speed = 0.01;   // speed artýrýldý

static void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    scene->draw();
    
    glutSwapBuffers();
    FrameCount++;
}

static void Key(unsigned char key, int x, int y)
{
	/*
	  padleri aþaðý yukarý hareket ettirmede, kullanýlan pad objesi ekrana yatay geldiðinden,
	  baþlangýçta z ekseni etrafýnda 90 derece döndürülmüþtür. Dolayýsýyla translate iþlemlerinden
	  önce ve sonra rotate edilmiþtir.
	*/
    printf("%d", key);
	switch (key) {
		case 27: // ESC
			exit(0);
            break;
        case 'q': // ESC
			if(pad1->getPosition().y>0.832)
				break;
			else{
				pad1->rotate(-90,0,0,1);
				pad1->translate(0, 2, 0);
				pad1->rotate(90,0,0,1);
			}
            break;
        case 'a': // ESC
			if(pad1->getPosition().y<-1.43)
				break;
			else{
				pad1->rotate(-90,0,0,1);
				pad1->translate(0, -2, 0);
				pad1->rotate(90,0,0,1);
			}
			break;
        case 'w': // ESC
			if(pad2->getPosition().y>0.832)
				break;
			else{
				pad2->rotate(-90,0,0,1);
				pad2->translate(0, 2, 0);
				pad2->rotate(90,0,0,1);
			}
            break;
        case 's': // ESC
			if(pad2->getPosition().y<-1.43)
				break;
			else{
				pad2->rotate(-90,0,0,1);
				pad2->translate(0, -2, 0);
				pad2->rotate(90,0,0,1);
			}
            break;
	}
}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
    if (0 != value) {
		char* TempString = (char*)
        malloc(512 + strlen(WINDOW_TITLE_PREFIX));
        
		sprintf(
                TempString,
                "%s: %d Frames Per Second @ %d x %d",
                WINDOW_TITLE_PREFIX,
                FrameCount * 4,
                CurrentWidth,
                CurrentHeight
                );
        
		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	glutTimerFunc(250, timerCallback, 1);
}

void IdleFunction(void)
{
    glm::mat4 ballMatrix = ball->getModelMatrix();
	//glm::mat4 pad1Matrix = pad1->getModelMatrix();
    float x = ball->getPosition().x;
    float y = ball->getPosition().y;
	float s1y = pad1->getPosition().y;  // stick1 in y si 
	float s2y = pad2->getPosition().y;  // stick2 in y si
	//printf("pad2 loc: %f\n",pad2->getPosition().y); Debug için
	//printf("ball loc: %f\n",y);
    //Oyun bitme ve topun yön deðiþtirme kontrolleri
	if( y > 1.20 )
    {
        directionY *= -1;
    }
    if( y < -1.20 )
    {
        directionY *= -1;
    }
    if( x > 1.56 )
    {
		if((((y-0.288)-s2y) <= 0.106 && ((y-0.288)-s2y) >= 0 ) || ((y-0.288)-s2y >= -0.096 && ((y-0.288)-s2y) <= 0))
			directionX *= -1;
		else
			exit(0);
    }
    if( x < -1.56 )
    {
        if((((y-0.288)-s1y) <= 0.106 && ((y-0.288)-s1y) >= 0 ) || ((y-0.288)-s1y >= -0.096 && ((y-0.288)-s1y) <= 0))
			directionX *= -1;
		else
			exit(0);
    }
	//
    ball->translate(directionX*speed, directionY*speed, 0);
	glutPostRedisplay();
}

void setupScene(){
    scene = new Scene();
    
    pad1 = new ColorFillNode("stick.obj");
    
    pad2 = new ColorFillNode("stick.obj");
    
    ball = new ColorFillNode("ball.obj");
    
    pad1->scale(0.016,0.016,0.016);
    pad2->scale(0.016,0.016,0.016);
	ball->scale(0.1,0.1,0.1);
    scene->addNode(pad1);
    scene->addNode(pad2);
    scene->addNode(ball);
   
    camera = new Camera();
    
    camera->translate(0.f, 0.f, 3.f);
    
    pad1->translate(-80.f, 0.f, 0.f);
    pad1->rotate(90,0,0,1);
    pad2->translate( 120.f, 0.f, 0.f);
    pad2->rotate(90,0,0,1);
	
    scene->setCamera(camera);
}

int main (int argc, char ** argv)
{
    
    GLenum type;
    
	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
    type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");
    
    glClearColor(0.0, 1.0, 0.0, 1.0);
    // Z-Buffer i aciyoruz
    glEnable(GL_DEPTH_TEST);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); 
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("GLSL Version: %s\n",glslVersion);
    
    const GLubyte* glVersion = glGetString(GL_VERSION);
    
    printf("GL Version: %s\n",glVersion);
       
    setupScene();
    
    timerCallback(0);
    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(Draw);
    glutKeyboardFunc(Key);
    glutIdleFunc(IdleFunction);
    glutMainLoop();
    
    return 0;
}


