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

#include "ImageLoader.h"
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

/**
 * 0 - Intro
 * 1 - Level 1
 * 2 - Level 2
 * 3 - Level 3
 * 4 - Level 4
 * 5 - Level 5
 * 6 - Level 6
 * 7 - Level 7
 * 8 - Level 8
 * 9 - Level 9
 * 10 - Level 10
 * 11 - Instructions
 */
static GLuint texturesBack[12];

int current_level = 0;

GLint         window_width = 860
            , window_height = 575
            , vertical_movement_speed = 1
            , horizontal_movement_speed = 10;

GLfloat     vertical_movement_range = 3.0;

GLboolean     vertical_movement_lock = false
            , boy_move_down = true
            , girl_move_down = false
            , boy_vertical_move = false
            , girl_vertical_move = false;


/* PROTOTYPES */
void glChangeBackgroundColorHEX(int, int, int, float);
void glChangeColorHEX(int, int, int);
void glcreateWindow(std::string, int, int);
void drawCharacters();
void drawWalls();
void paintWall(Wall);
void nextListItem(std::list<Wall>& list);
Wall frontListItem(std::list<Wall>& list);
void movement(int, int, int);
void timeMoveHorizontalLeft(int);
void timeMoveHorizontalRight(int);
void timeMoveVertical(int);
void keyboard(unsigned char, int, int);
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
    //TODO cuando ambos quedan flotando al mismo tiempo
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
                
                glutPostRedisplay();
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
                
                glutPostRedisplay();
            }
        }
    }
    
    if (girl_vertical_move)  //si se debe de mover la niña
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
                
                glutPostRedisplay();
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
                
                glutPostRedisplay();
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

void loadTexture(Image* image, GLuint txtName[], int k)
{
    glBindTexture(GL_TEXTURE_2D, txtName[k]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
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
    
    //Fondo
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, texturesBack[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1024, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1024, 1024);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, 1024);
    glEnd();
    
    
    drawCharacters();
    
    drawWalls(walls[current_level]);
    
    glutSwapBuffers();
}

/***************************************************************************
 ******************             FIN DE PINTADO             *****************
 ***************************************************************************/





void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(69.0, (float)w / (float)h, 1.0, 20.0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

void initRendering()
{
    Image* image;
    
    glEnable(GL_TEXTURE_2D);
    
    glGenTextures(1, texturesBack); //Make room for our texture
    
    image = loadBMP("BMP/Nivel1.bmp");
    loadTexture(image, texturesBack, 0); //cual imagen y en que posicion del arreglo de texturas
    
    delete image;
}

void init()
{
    glChangeBackgroundColorHEX(100, 100, 100, 1.0);
    
    //boy = Character(window_width/2, 0.0, 40.0, 50.0);
    boy = Character(10000, 0.0, 40.0, 50.0);
    //girl = Character(window_width/2, 0.0, 40.0, 50.0);
    girl = Character(10000, 0.0, 40.0, 50.0);
    girl.setY(window_height - girl.getHeight());
    
    //Inicialización de paredes externas
    walls[0].push_back(Wall(-5,0,5, window_height));
    walls[0].push_back(Wall(window_width, 0, 5, window_height));
    walls[0].push_back(Wall(0,-5,window_width, 5));
    walls[0].push_back(Wall(0, window_height, window_width, 5));
    
    /** NIVEL 1
    walls[0].push_back(Wall(0, 0, 860, 102));
    walls[0].push_back(Wall(0, 487, 860, 102));
    walls[0].push_back(Wall(297, 253, 221, 88));
    **/
    
    /** NIVEL 2
    walls[0].push_back(Wall(0, 0, 40, 265));
    walls[0].push_back(Wall(0, 0, 140, 143));
    walls[0].push_back(Wall(140, 0, 720, 63));
    walls[0].push_back(Wall(519, 63, 81, 260));
    walls[0].push_back(Wall(779, 63, 81, 514));
    walls[0].push_back(Wall(219, 225, 300, 98));
    walls[0].push_back(Wall(0, 265, 340, 312));
    walls[0].push_back(Wall(340, 445, 180, 132));
    walls[0].push_back(Wall(520, 525, 260, 52));
    **/
    
    /** NIVEL 3
    walls[0].push_back(Wall(0, 0, 40, 577));
    walls[0].push_back(Wall(40, 0, 820, 58));
    walls[0].push_back(Wall(343, 58, 141, 260));
    walls[0].push_back(Wall(484, 58, 120, 130));
    walls[0].push_back(Wall(819, 58, 41, 519));
    walls[0].push_back(Wall(40, 340, 59, 90));
    walls[0].push_back(Wall(99, 116, 121, 314));
    walls[0].push_back(Wall(220, 116, 44, 202));
    walls[0].push_back(Wall(220, 402, 513, 28));
    walls[0].push_back(Wall(692, 252, 41, 150));
    walls[0].push_back(Wall(643, 252, 49, 66));
    walls[0].push_back(Wall(301, 430, 130, 43));
    walls[0].push_back(Wall(719, 485, 100, 40));
    walls[0].push_back(Wall(39, 525, 780, 52));
    **/
    
    /** NIVEL 4
    walls[0].push_back(Wall(0, 0, 80, 577));
    walls[0].push_back(Wall(80, 0, 780, 63));
    walls[0].push_back(Wall(779, 63, 81, 514));
    walls[0].push_back(Wall(779, 63, 81, 514));
    walls[0].push_back(Wall(80, 485, 699, 92));
    walls[0].push_back(Wall(139, 63, 41, 300));
    walls[0].push_back(Wall(180, 265, 300, 98));
    walls[0].push_back(Wall(299, 186, 181, 79));
    walls[0].push_back(Wall(439, 125, 301, 78));
    walls[0].push_back(Wall(299, 445, 101, 41));
    **/
    
    /** NIVEL 5
    walls[0].push_back(Wall(0, 0, 40, 577));
    walls[0].push_back(Wall(40, 0, 820, 28));
    walls[0].push_back(Wall(40, 525, 820, 52));
    walls[0].push_back(Wall(559, 345, 301, 180));
    walls[0].push_back(Wall(559, 185, 121, 160));
    walls[0].push_back(Wall(680, 185, 100, 98));
    walls[0].push_back(Wall(564, 0, 41, 103));
    walls[0].push_back(Wall(605, 0, 255, 63));
    walls[0].push_back(Wall(819, 63, 41, 283));
    walls[0].push_back(Wall(119, 125, 141, 238));
    walls[0].push_back(Wall(119, 85, 41, 40));
    walls[0].push_back(Wall(219, 85, 41, 40));
    walls[0].push_back(Wall(260, 265, 299, 98));
    walls[0].push_back(Wall(299, 363, 41, 39));
    walls[0].push_back(Wall(349, 155, 121, 78));
    **/
    
    /** NIVEL 6
    walls[0].push_back(Wall(0, 0, 40, 577));
    walls[0].push_back(Wall(40, 445, 260, 132));
    walls[0].push_back(Wall(40, 0, 260, 63));
    walls[0].push_back(Wall(259, 125, 41, 320));
    walls[0].push_back(Wall(300, 265, 40, 98));
    walls[0].push_back(Wall(99, 265, 160, 98));
    walls[0].push_back(Wall(219, 125, 40, 140));
    walls[0].push_back(Wall(139, 125, 80, 78));
    walls[0].push_back(Wall(299, 0, 260, 16));
    walls[0].push_back(Wall(559, 0, 301, 103));
    walls[0].push_back(Wall(299, 525, 460, 52));
    walls[0].push_back(Wall(759, 103, 101, 474));
    walls[0].push_back(Wall(639, 485, 120, 40));
    walls[0].push_back(Wall(519, 485, 41, 40));
    walls[0].push_back(Wall(639, 425, 61, 60));
    walls[0].push_back(Wall(639, 265, 120, 58));
    **/
    
    /** NIVEL 7
    walls[0].push_back(Wall(0, 0, 92, 577));
    walls[0].push_back(Wall(92, 485, 768, 92));
    walls[0].push_back(Wall(92, 0, 160, 63));
    walls[0].push_back(Wall(252, 0, 179, 16));
    walls[0].push_back(Wall(431, 0, 429, 63));
    walls[0].push_back(Wall(771, 63, 89, 422));
    walls[0].push_back(Wall(730, 306, 41, 179));
    walls[0].push_back(Wall(431, 266, 181, 157));
    walls[0].push_back(Wall(431, 146, 41, 120));
    walls[0].push_back(Wall(551, 335, 81, 151));
    walls[0].push_back(Wall(632, 335, 60, 88));
    walls[0].push_back(Wall(531, 98, 181, 97));
    walls[0].push_back(Wall(571, 195, 41, 40));
    walls[0].push_back(Wall(131, 106, 121, 297));
    **/
    
    /** NIVEL 8
    walls[0].push_back(Wall(0, 0, 69, 577));
    walls[0].push_back(Wall(69, 0, 719, 16));
    walls[0].push_back(Wall(69, 545, 719,32));
    walls[0].push_back(Wall(788, 0, 72, 577));
    walls[0].push_back(Wall(69, 106, 100, 77));
    walls[0].push_back(Wall(128, 183, 41, 60));
    walls[0].push_back(Wall(169, 146, 180, 57));
    walls[0].push_back(Wall(308, 106, 41, 40));
    walls[0].push_back(Wall(428, 395, 41, 151));
    walls[0].push_back(Wall(348, 395, 80, 88));
    walls[0].push_back(Wall(348, 246, 41, 149));
    walls[0].push_back(Wall(268, 246, 281, 57));
    walls[0].push_back(Wall(388, 106, 41, 140));
    walls[0].push_back(Wall(175, 366, 121, 97));
    walls[0].push_back(Wall(521, 366, 121, 97));
    walls[0].push_back(Wall(648, 106, 140, 117));
    walls[0].push_back(Wall(468, 106, 180, 77));
    **/
    
    /** NIVEL 9 **/
    
    
    /** NIVEL 10
    walls[0].push_back(Wall(0, 0, 80, 577));
    walls[0].push_back(Wall(80, 0, 699, 66));
    walls[0].push_back(Wall(80, 565, 699, 12));
    walls[0].push_back(Wall(779, 0, 81, 577));
    walls[0].push_back(Wall(79, 189, 261, 137));
    walls[0].push_back(Wall(519, 189, 260, 137));
    walls[0].push_back(Wall(250, 435, 361, 57));
    walls[0].push_back(Wall(330, 375, 201, 60));
    **/
    
    
    
    
    /** BASES NIVEL 1
    walls[0].push_back(Wall(393, 341, 29, 40)); // NIÑO
    walls[0].push_back(Wall(393, 213, 29, 40)); // NIÑA
    **/
    
    /** BASES NIVEL 2
    walls[0].push_back(Wall(415, 323, 32, 43)); // NIÑO
    walls[0].push_back(Wall(415, 182, 32, 43)); // NIÑA
    **/

    /** BASES NIVEL 3
    walls[0].push_back(Wall(50, 430, 29, 39)); // NIÑO
    walls[0].push_back(Wall(50, 301, 29, 39)); // NIÑA
    **/
    
    /** BASES NIVEL 4
    walls[0].push_back(Wall(225, 363, 31, 42)); // NIÑA
    walls[0].push_back(Wall(225, 224, 31, 41)); // NIÑO
    **/
    
    /** BASES NIVEL 5
    walls[0].push_back(Wall(697, 283, 32, 42)); // NIÑA
    walls[0].push_back(Wall(697, 142, 32, 43)); // NIÑO
    **/
    
    /** BASES NIVEL 6
    walls[0].push_back(Wall(135, 363, 30, 40)); // NIÑA
    walls[0].push_back(Wall(135, 225, 30, 40)); // NIÑO
    **/
    
    /** BASES NIVEL 7
    walls[0].push_back(Wall(648, 423, 29, 40)); // NIÑA
    walls[0].push_back(Wall(648, 295, 29, 40)); // NIÑO
    **/
    
    /** BASES NIVEL 8
    walls[0].push_back(Wall(393, 483, 29, 40)); // NIÑO
    walls[0].push_back(Wall(393, 355, 29, 40)); // NIÑA
    **/
    
    /** BASES NIVEL 9 **/

    /** BASES NIVEL 10
    walls[0].push_back(Wall(301, 492, 29, 40)); // NIÑO
    walls[0].push_back(Wall(531, 492, 29, 40)); // NIÑA
    **/
    
    
    /*
     * Determinacion de cantidad de paredes por lista (no se deben declarar
     * mas paredes despues de esta linea)
     */
    for (int x = 0; x < LISTS_COUNT; x++)
    {
        lists_size[x] = walls[x].size();
    }
    
    initRendering();
}

#include <unistd.h>

int main(int argc, char **argv)
{
    char path[1024];
    getcwd(path, sizeof(path));
    puts(path);
    
    glutInit(&argc, argv);
    
    glcreateWindow("Brother&Sister", window_width, window_height);
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(movement);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}