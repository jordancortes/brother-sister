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
#include <list>

#include "Character.h"
#include "Wall.h"

/* GLOBAL VARIABLES */
Character     boy
            , girl;

/**
 * Arreglo de listas de paredes
 ********************************
 * 0 - outer wall
 * 1 - Level 1
 * 2 - Level 2
 * ...
 * 9 - Level 9
 * 10 - Level 10
 */
std::list<Wall> walls[11];
unsigned long lists_size[11];
const int LISTS_COUNT = 11;
int current_level = 0;

GLint         window_width = 960
            , window_height = 640
            , vertical_movement_speed = 1
            , horizontal_movement_speed = 10;

GLfloat     vertical_movement_range = 3.0;

GLboolean     vertical_movement_lock = false
            , boy_move_down = true
            , girl_move_down = false
            , boy_vertical_move = false
            , girl_vertical_move = false;


/* PROTOTYPES */
void glChangeBackgroundColorHEX(int red, int green, int blue, float opacity);
void glChangeColorHEX(int red, int green, int blue);
void glcreateWindow(std::string name, int window_width, int window_height);
void drawCharacters();
void drawWalls();
void paintWall(Wall wall);
void nextListItem(std::list<Wall>& list);
Wall frontListItem(std::list<Wall>& list);
void movement(int key, int x, int y);
void timeMoveHorizontalLeft(int v);
void timeMoveHorizontalRight(int v);
void timeMoveVertical(int v);
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
void glChangeBackgroundColorHEX(int red, int green, int blue, float opacity)
{
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
    gluOrtho2D(0, window_width, 0, window_height);
}


/***************************************************************************
 ******************           INICIO DE LISTAS             *****************
 ***************************************************************************/

Wall frontListItem(std::list<Wall>& list)
{
    return (Wall)list.front();
}

/**
 * Mueve el primer elemento de la fila al final,
 * esto con el fin de recorrer la fila.
 * (Se debe de usar en un ciclo para no desacomodar la fila,
 * aunque el orden de los objetos no es importante)
 */
void nextListItem(std::list<Wall>& list)
{
    list.push_back(list.front());
    list.pop_front();
}

/***************************************************************************
 ******************             FIN DE LISTAS              *****************
 ***************************************************************************/










/***************************************************************************
 ******************           INICIO DE CHEQUEOS           *****************
 ***************************************************************************/

bool characterIsDown(Character _character, Wall _wall)
{
    return _character.getY() + _character.getHeight() <= _wall.getY();
}

bool characterIsUp(Character _character, Wall _wall)
{
    return _character.getY() >= _wall.getY() + _wall.getHeight();
}

bool characterIsRight(Character _character, Wall _wall)
{
    return _character.getX() >= _wall.getX() + _wall.getWidth();
}

bool characterIsLeft(Character _character, Wall _wall)
{
    return _character.getX() + _character.getWidth() <= _wall.getX();
}

bool characterIsInside(Character _character, Wall _wall)
{
    return
    (
        ((_character.getY() + _character.getHeight()) > _wall.getY() && _character.getY() < _wall.getY())
        ||
        (_character.getY() < (_wall.getY() + _wall.getHeight()) && (_character.getY() + _character.getHeight()) > (_wall.getY() + _wall.getHeight()))
        ||
        ((_character.getX() + _character.getWidth()) > _wall.getX() && _character.getX() < _wall.getX())
        ||
        (_character.getX() < (_wall.getX() + _wall.getWidth()) && (_character.getX() + _character.getWidth()) > (_wall.getX() + _wall.getWidth()))
    );
}

bool characterIsFloating(Character _character, std::list<Wall> list)
{
    bool floating = true;
    
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Wall _wall = frontListItem(list);
        
        
        if  (
            (characterIsUp(_character, _wall) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall) && _character.getY() == (_wall.getY() + _wall.getHeight()))
            ||
            (characterIsDown(_character, _wall) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall) && (_character.getY() + _character.getHeight()) == _wall.getY())
            ||
            (characterIsLeft(_character, _wall) && !characterIsUp(_character, _wall) && !characterIsDown(_character, _wall) && (_character.getX() + _character.getWidth()) == _wall.getX())
            ||
            (characterIsRight(_character, _wall) && !characterIsUp(_character, _wall) && !characterIsDown(_character, _wall) && _character.getX() == (_wall.getX() + _wall.getWidth()))
            )
        {
            floating = false;
        }
        
        nextListItem(list);
    }
    
    return floating;
}

/***************************************************************************
 *******************           FIN DE CHEQUEOS           *******************
 ***************************************************************************/










/***************************************************************************
 ******************          INICIO DE COLISIONES          *****************
 ***************************************************************************/

//character going left
bool collisionLeft(Character _character, std::list<Wall> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Wall _wall = frontListItem(list);
        
        if (characterIsRight(_character, _wall) && !characterIsDown(_character, _wall) && !characterIsUp(_character, _wall))
        {
            if ((_character.getX() - 1) < (_wall.getX() + _wall.getWidth()))
            {
                return true;
            }
        }
        
        nextListItem(list);
    }
    
    return false;
}

//character going right
bool collisionRight(Character _character, std::list<Wall> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Wall _wall = frontListItem(list);
        
        if (characterIsLeft(_character, _wall) && !characterIsDown(_character, _wall) && !characterIsUp(_character, _wall))
        {
            if ((_character.getX() + _character.getWidth() + 1) > _wall.getX())
            {
                return true;
            }
        }
        
        nextListItem(list);
    }
    
    return false;
}

//character going up
float collisionTop(Character _character, std::list<Wall> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Wall _wall = frontListItem(list);
        
        if ((characterIsDown(_character, _wall) || characterIsInside(_character, _wall)) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall))
        {
            if ((_character.getY() + _character.getHeight() + vertical_movement_range) > _wall.getY())
            {
                if (characterIsInside(_character, _wall))
                {
                    return (_character.getY() + _character.getHeight()) - _wall.getY();
                }
            }
        }
        
        nextListItem(list);
    }
    
    return 0;
}

//character going down
float collisionBottom(Character _character, std::list<Wall> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Wall _wall = frontListItem(list);
        
        if ((characterIsUp(_character, _wall) || characterIsInside(_character, _wall)) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall))
        {
            if ( (_character.getY() - vertical_movement_range) < (_wall.getY() + _wall.getHeight()) )
            {
                if (characterIsInside(_character, _wall))
                {
                    return (_wall.getY() + _wall.getHeight()) - _character.getY();
                }
            }
        }
        
        nextListItem(list);
    }
    
    return 0;
}

/***************************************************************************
 ******************            FIN DE COLISIONES           *****************
 ***************************************************************************/










/***************************************************************************
 ******************          INICIO DE MOVIMIENTOS         *****************
 ***************************************************************************/

void moveCharactersFloating() {
    if (characterIsFloating(boy, walls[current_level]))
    {
        if (!vertical_movement_lock)
        {
            vertical_movement_lock = true;
            
            boy_vertical_move = !boy_vertical_move;
            
            glutTimerFunc(vertical_movement_speed,timeMoveVertical, 1);
        }
    }
    
    if (characterIsFloating(girl, walls[current_level]))
    {
        if (!vertical_movement_lock)
        {
            vertical_movement_lock = true;
            
            girl_vertical_move = !girl_vertical_move;
            
            glutTimerFunc(vertical_movement_speed,timeMoveVertical, 1);
        }
    }
}

void timeMoveHorizontalRight(int v)
{
    if (v >= 0)
    {
        if (!collisionRight(boy, walls[current_level]))
        {
            boy.setX(boy.getX() + 1);
        }
        if (!collisionLeft(girl, walls[current_level]))
        {
            girl.setX(girl.getX() - 1);
        }
        
        moveCharactersFloating();
        
        glutPostRedisplay();
        glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalRight, --v);
    }
}

void timeMoveHorizontalLeft(int v)
{
    if (v >= 0)
    {
        if (!collisionLeft(boy, walls[current_level]))
        {
            boy.setX(boy.getX() - 1);
        }
        if (!collisionRight(girl, walls[current_level]))
        {
            girl.setX(girl.getX() + 1);
        }
        
        moveCharactersFloating();
        
        glutPostRedisplay();
        glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalLeft, --v);
    }
}

void timeMoveVertical(int v)
{
    if (boy_vertical_move) //si se debe de mover el niño
    {
        float boy_diff_collision;
        
        if (!boy_move_down) //si va para arriba
        {
            boy_diff_collision = collisionTop(boy, walls[current_level]);
            
            if (0 >= collisionTop(boy, walls[current_level])) //si no colisiona
            {
                boy.setY(boy.getY() + vertical_movement_range);
                glutPostRedisplay();
                glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
            }
            else //si colisiona
            {
                boy.setY(boy.getY() - boy_diff_collision);
                
                boy_vertical_move = false;
                if (!girl_vertical_move)
                {
                    vertical_movement_lock = false;
                }
            }
        }
        else //si va para abajo
        {
            boy_diff_collision = collisionBottom(boy, walls[current_level]);
            
            if (0 >= collisionBottom(boy, walls[current_level]))
            {
                boy.setY(boy.getY() - vertical_movement_range);
                glutPostRedisplay();
                glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
            }
            else
            {
                boy.setY(boy.getY() + boy_diff_collision);
                
                boy_vertical_move = false;
                if (!girl_vertical_move)
                {
                    vertical_movement_lock = false;
                }
            }
        }
    }
    
    if (girl_vertical_move)  //si se debe de mover el niño
    {
        float girl_diff_collision;
        
        if (!girl_move_down) //si va para arriba
        {
            girl_diff_collision = collisionTop(girl, walls[current_level]);
            
            if (0 >= collisionTop(girl, walls[current_level])) //si no colisiona
            {
                girl.setY(girl.getY() + vertical_movement_range);
                glutPostRedisplay();
                glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
            }
            else //si colisiona
            {
                girl.setY(girl.getY() - girl_diff_collision);
                
                girl_vertical_move = false;
                if (!boy_vertical_move)
                {
                    vertical_movement_lock = false;
                }
            }
        }
        else //si va para abajo
        {
            girl_diff_collision = collisionBottom(girl, walls[current_level]);
            
            if (0 >= collisionBottom(girl, walls[current_level]))
            {
                girl.setY(girl.getY() - vertical_movement_range);
                glutPostRedisplay();
                glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
            }
            else
            {
                girl.setY(girl.getY() + girl_diff_collision);
                
                girl_vertical_move = false;
                if (!boy_vertical_move)
                {
                    vertical_movement_lock = false;
                }
            }
        }
    }
}

/***************************************************************************
 ******************            FIN DE MOVIMIENTOS          *****************
 ***************************************************************************/










/***************************************************************************
 ******************     INICIO DE FUNCIONES ESPECIALES     *****************
 ***************************************************************************/

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'z':
        case 'Z':
            if (!vertical_movement_lock)
            {
                vertical_movement_lock = true;
                
                boy_vertical_move = !boy_vertical_move;
                boy_move_down = !boy_move_down;
                
                girl_vertical_move = !girl_vertical_move;
                girl_move_down = !girl_move_down;
                
                glutTimerFunc(vertical_movement_speed,timeMoveVertical, 1);
            }
            break;
        /*case 'f':
            std::cout << "Floating: " << (characterIsFloating(boy, walls[current_level]) ? "True" : "False") << std::endl;
            break;*/
        default:
            break;
    }
}

void movement(int key, int x, int y)
{
        if (GLUT_KEY_LEFT == key)
        {
            glutTimerFunc(horizontal_movement_speed,timeMoveHorizontalLeft,10);
            glutPostRedisplay();
        }
        else if (GLUT_KEY_RIGHT == key)
        {
            glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalRight, 10);
            glutPostRedisplay();
        }
}

/***************************************************************************
 ******************      FIN DE FUNCIONES ESPECIALES       *****************
 ***************************************************************************/










/***************************************************************************
 ******************            INICIO DE PINTADO           *****************
 ***************************************************************************/

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

void drawWalls(std::list<Wall> list)
{
    glChangeColorHEX(255, 0, 0);
    for (int x = 0; x < list.size(); x++)
    {
        paintWall(frontListItem(list));
        nextListItem(list);
    }
    glChangeColorHEX(255, 255, 255);
}

void paintWall(Wall wall)
{
    glBegin(GL_QUADS);
    glVertex2f(wall.getX(), wall.getY());
    glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
    glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
    glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawCharacters();
    
    drawWalls(walls[current_level]);
    
    glutSwapBuffers();
}

/***************************************************************************
 ******************             FIN DE PINTADO             *****************
 ***************************************************************************/









void init()
{
    glChangeBackgroundColorHEX(100, 100, 100, 1.0);
    
    boy = Character(window_width/2, 0.0, 40.0, 50.0);
    girl = Character(window_width/2, 0.0, 40.0, 50.0);
    girl.setY(window_height - girl.getHeight());
    
    //Inicialización de paredes externas
    walls[0].push_back(Wall(-5,0,5, window_height));
    walls[0].push_back(Wall(window_width, 0, 5, window_height));
    walls[0].push_back(Wall(0,-5,window_width, 5));
    walls[0].push_back(Wall(0, window_height, window_width, 5));
    
    walls[0].push_back(Wall(900, 10, 10, 10));
    walls[0].push_back(Wall(300, 102, 102, 102));
    
    /*
     * Determinacion de cantidad de paredes por lista (no se deben declarar
     * mas paredes despues de esta linea)
     */
    for (int x = 0; x < LISTS_COUNT; x++)
    {
        lists_size[x] = walls[x].size();
    }
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