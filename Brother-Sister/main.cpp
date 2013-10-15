//
//  main.cpp
//  Brother-Sister
//
//  Created by Jordan Cortés Guzmán on 12/10/2013.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string>
#include <sstream>

#include "Character.h"

/* GLOBAL VARIABLES */
Character boy, girl;
GLint   window_width = 960 , window_height = 640, vertical_move_time = 50;
GLfloat final_position;
GLboolean movement_lock = false, boy_down = true;


/* PROTOTYPES */
void glChangeBackgroundColorHEX(int red, int green, int blue, float opacity);
void glChangeColorHEX(int red, int green, int blue);
void glcreateWindow(std::string name, int window_width, int window_height);
void drawCharacters();
void movement(int key, int x, int y);
void time(int v);
void keyboard(unsigned char key, int x, int y);
void display();
void init();

/*
 ********** FUNCION *********
 * Cambia el color de fondo de la ventana. Valores RGBA.
 * El valor minimo es 0 y maximo es 255 para los enteros.
 * El valor de opacidad minimo es 0 y maximo es 1.
 *
 ******** PARAMETROS ********
 * red     - int   - intensidad de rojo
 * green   - int   - intensidad de verde
 * blue    - int   - intensidad de azul
 * opacity - float - transparencia
 *
 */
void glChangeBackgroundColorHEX(int red, int green, int blue, float opacity) {
    glClearColor( ( 1.0 / 255.0 ) * float( red )
                 , ( 1.0 / 255.0 ) * float( green )
                 , ( 1.0 / 255.0 ) * float( blue )
                 , opacity );
}

/*
 ********** FUNCION *********
 * Cambia el color para pintar. Valores RGB.
 * El valor minimo es 0 y maximo es 255
 *
 ******** PARAMETROS ********
 * red   - int - intensidad de rojo
 * green - int - intensidad de verde
 * blue  - int - intensidad de azul
 *
 */
void glChangeColorHEX(int red, int green, int blue)
{
    glColor3f( ( 1.0 / 255.0 ) * float( red )
              , ( 1.0 / 255.0 ) * float( green )
              , ( 1.0 / 255.0 ) * float( blue ) );
}

/*
 ********** FUNCION *********
 * Crea una ventana al centro de la pantalla
 * y con una proyeccion tomada de las medidas
 * de la ventana, y donde el origen (0,0) es
 * arriba e izquierda.
 *
 ******** PARAMETROS ********
 * name         - std::string - titulo de la ventana
 * windowWidth  - int         - ancho de la ventana
 * windowHeight - int         - alto de la ventana
 *
 */
void glcreateWindow(std::string name, int window_width, int window_height)
{
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( ( glutGet( GLUT_SCREEN_WIDTH ) - window_width ) / 2
                           , ( glutGet( GLUT_SCREEN_HEIGHT ) - window_height ) / 2 );
	glutInitWindowSize( window_width,window_height );
    glutCreateWindow( name.c_str() );
}

void drawCharacters()
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(boy.getX(), boy.getY());
    glVertex2f(boy.getX() + boy.getWidth(), boy.getY());
    glVertex2f(boy.getX() + boy.getWidth(), boy.getY() + boy.getHeight());
    glVertex2f(boy.getX(), boy.getY() + boy.getHeight());
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(girl.getX(), girl.getY());
    glVertex2f(girl.getX() + girl.getWidth(), girl.getY());
    glVertex2f(girl.getX() + girl.getWidth(), girl.getY() + girl.getHeight());
    glVertex2f(girl.getX(), girl.getY() + girl.getHeight());
    glEnd();
    glPopMatrix();
}

void time(int v)
{
    GLboolean end = false;
    
    if (boy_down)
    {
        boy.setY(boy.getY() + 0.1);
        girl.setY(girl.getY() - 0.1);
        
        if (boy.getY() >= final_position)
        {
            end = true;
            
            boy.setY(0.8);
            girl.setY(-1.0);
        }
    }
    else if (!boy_down)
    {
        boy.setY(boy.getY() - 0.1);
        girl.setY(girl.getY() + 0.1);
        
        if (boy.getY() <= final_position)
        {
            end = true;
            
            boy.setY(-1.0);
            girl.setY(0.8);
        }
    }
    
    glutPostRedisplay();
    
    if (end)
    {
        final_position = 0.0;
        movement_lock = false;
        boy_down = !boy_down;
    }
    else
    {
        glutTimerFunc(vertical_move_time,time,1);
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'z':
        case 'Z':
            if (!movement_lock)
            {
                std::cout << boy.getY() << "\n";
                final_position = girl.getY();
                movement_lock = true;
                glutTimerFunc(vertical_move_time,time, 1);
            }
            break;
        default:
            break;
    }
}

void movement(int key, int x, int y)
{
    if (GLUT_KEY_LEFT == key)
    {
        boy.setX(boy.getX() - 0.05);
        girl.setX(girl.getX() + 0.05);
        glutPostRedisplay();
    }
    else if (GLUT_KEY_RIGHT == key)
    {
        boy.setX(boy.getX() + 0.05);
        girl.setX(girl.getX() - 0.05);
        glutPostRedisplay();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawCharacters();
    
    glutSwapBuffers();
}

void init()
{
    glChangeBackgroundColorHEX(100, 100, 100, 1.0);
    boy = Character(0.0, -1.0, 0.1, 0.2);
    girl = Character(0.0, 0.8, 0.1, 0.2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glcreateWindow("Brother&Sister", window_width, window_height);
    
    init();
    
    glutDisplayFunc(display);
    glutSpecialFunc(movement);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}

