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
#include "Block.h"
#include "Button.h"

/* GLOBAL VARIABLES */

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
const int LISTS_COUNT = 12;
std::list<Block> walls[LISTS_COUNT];
unsigned long walls_list_size[LISTS_COUNT];

Block goals[LISTS_COUNT][2];

std::list<Block> spikes[LISTS_COUNT];
unsigned long spikes_list_size[LISTS_COUNT];

Button buttons[LISTS_COUNT];


Character     boy
            , girl;

/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * L | boy.x | boy.y | girl.x | girl.y | boy.horizontal-side | boy.vertical-side | girl.horizontal-side | girl.vertical-side |
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * 0 |  1000 |  1000 |  1000  |  1000  |         1  (left)   |        0  (down) |         0     (right) |          1   (up)  |
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * 1 |  10   |   10    |   10     |    10    |
 * 2 |       |       |        |        |
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
const float character_init_pos[LISTS_COUNT][8] =
{
      {1000, 1000, 1000, 1000, 0, 0, 0, 0}
    , {722, 102, 53, 437, 1, 0, 0, 1}
    , {705, 63, 106, 215, 1, 0, 0, 1}
    , {224, 318, 496, 475, 0, 0, 1, 1}
    , {306, 363, 90, 135, 0, 0, 1, 1}
    , {444, 363, 444, 475, 0, 0, 1, 1}
    , {300, 363, 708, 435, 0, 0, 1, 1}
    , {202, 403, 152, 435, 0, 0, 1, 1}
    , {694, 16, 100, 56, 0, 0, 1, 1}
    , {146, 66, 673, 66, 0, 0, 1, 0}
    , {1000, 1000, 1000, 1000, 1, 0, 0, 1}
    , {1000, 1000, 1000, 1000, 1, 0, 0, 1}
};

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
 * 10 - Congratulations
 * 11 - Instructions
 */
static GLuint texturesBack[12];

/**
 * 0 - Boy_Down_Right
 * 1 - Boy_Down_Left
 * 2 - Boy_Up_Right
 * 3 - Boy_Up_Left
 * 4 - Girl_Down_Right
 * 5 - Girl_Down_Left
 * 6 - Girl_Up_Right
 * 7 - Girl_Up_Left
 */
static GLuint textures[5];

int current_level = 0;

GLint         window_width = 860
            , window_height = 575
            , vertical_movement_speed = 1
            , horizontal_movement_speed = 10
            , boy_side = character_init_pos[current_level][4]
            , girl_side = character_init_pos[current_level][6];

GLfloat     vertical_movement_range = 3.0;

GLboolean     vertical_movement_lock = false
            , horizontal_movement_lock = false
            , boy_move_down = (character_init_pos[current_level][5] == 0 ? true : false) //true
            , girl_move_down = (character_init_pos[current_level][7] == 0 ? true : false)
            , boy_vertical_move = false
            , girl_vertical_move = false;


/* PROTOTYPES */
void glChangeBackgroundColorHEX(int, int, int, float);
void glChangeColorHEX(int, int, int);
void glcreateWindow(std::string, int, int);
void drawCharacters();
void drawWalls();
void paintWall(Block, int);
void nextListItem(std::list<Block>& list);
Block frontListItem(std::list<Block>& list);
void movement(int, int, int);
void timeMoveHorizontalLeft(int);
void timeMoveHorizontalRight(int);
void timeMoveVertical(int);
void nextLevel();
void resetLevel();
void keyboard(unsigned char, int, int);
void display();
void addMenu();
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

Block frontListItem(std::list<Block>& list)
{
    return (Block)list.front();
}

/**
 * Mueve el primer elemento de la fila al final,
 * esto con el fin de recorrer la fila.
 * (Se debe de usar en un ciclo para no desacomodar la fila,
 * aunque el orden de los objetos no es importante)
 */
void nextListItem(std::list<Block>& list)
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

bool characterIsDown(Character _character, Block _wall)
{
    return _character.getY() + _character.getHeight() <= _wall.getY();
}

bool characterIsUp(Character _character, Block _wall)
{
    return _character.getY() >= _wall.getY() + _wall.getHeight();
}

bool characterIsRight(Character _character, Block _wall)
{
    return _character.getX() >= _wall.getX() + _wall.getWidth();
}

bool characterIsLeft(Character _character, Block _wall)
{
    return _character.getX() + _character.getWidth() <= _wall.getX();
}

bool characterIsInside(Character _character, Block _wall)
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

bool characterIsFloating(Character _character, std::list<Block> list)
{
    bool floating = true;
    
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Block _wall = frontListItem(list);
        
        
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

bool checkGoalReached()
{
    std::cout << "BoyX: " << boy.getX() << std::endl;
    std::cout << "BoyY: " << boy.getY() << std::endl;
    std::cout << "GirlX: " << girl.getX() << std::endl;
    std::cout << "GirlY: " << girl.getY() << std::endl;
    
    return
        (characterIsInside(boy, goals[current_level][0]) && !characterIsDown(boy, goals[current_level][0]) && !characterIsUp(boy, goals[current_level][0]) && !characterIsLeft(boy, goals[current_level][0]) && !characterIsRight(boy, goals[current_level][0]))
        &&
    (characterIsInside(girl, goals[current_level][1]) && !characterIsDown(girl, goals[current_level][1]) && !characterIsUp(girl, goals[current_level][1]) && !characterIsLeft(girl, goals[current_level][1]) && !characterIsRight(girl, goals[current_level][1]));
}

/***************************************************************************
 *******************           FIN DE CHEQUEOS           *******************
 ***************************************************************************/










/***************************************************************************
 ******************          INICIO DE COLISIONES          *****************
 ***************************************************************************/

//character going left
bool collisionLeft(Character _character, std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Block _wall = frontListItem(list);
        
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
bool collisionRight(Character _character, std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Block _wall = frontListItem(list);
        
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
float collisionTop(Character _character, std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Block _wall = frontListItem(list);
        
        if (characterIsDown(_character, _wall) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall))
        {
            if ((_character.getY() + _character.getHeight() + vertical_movement_range) >= _wall.getY())
            {
                return abs((_character.getY() + _character.getHeight()) - _wall.getY());
            }
        }
        
        nextListItem(list);
    }
    
    return -1.0f;
}

//character going down
float collisionBottom(Character _character, std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        Block _wall = frontListItem(list);
        
        if (characterIsUp(_character, _wall) && !characterIsLeft(_character, _wall) && !characterIsRight(_character, _wall))
        {
            if ( (_character.getY() - vertical_movement_range) <= (_wall.getY() + _wall.getHeight()) )
            {
                return abs((_wall.getY() + _wall.getHeight()) - _character.getY());
            }
        }
        
        nextListItem(list);
    }
    
    return -1.0f;
}

bool collisionSpikes(Character _character, std::list<Block> list)
{
    return
        (collisionTop(_character, list) <= 0 ? false : true)
        || collisionRight(_character, list)
        || (collisionBottom(_character, list) <= 0 ? false : true)
        || collisionLeft(_character, list);
}

bool collisionButton(Character _character, Button button)
{
    std::list<Block> temp;
    temp.push_back(Block(button.getX(), button.getY(), button.getWidth(), button.getHeight()));
    
    return (collisionTop(_character, temp) <= 0 ? false : true)
    || collisionRight(_character, temp)
    || (collisionBottom(_character, temp) <= 0 ? false : true)
    || collisionLeft(_character, temp);
}

/***************************************************************************
 ******************            FIN DE COLISIONES           *****************
 ***************************************************************************/










/***************************************************************************
 ******************          INICIO DE MOVIMIENTOS         *****************
 ***************************************************************************/

void moveCharactersFloating() {
    if (characterIsFloating(boy, walls[current_level]) && characterIsFloating(girl, walls[current_level]))
    {
        if (!vertical_movement_lock)
        {
            vertical_movement_lock = true;
            boy_vertical_move = !boy_vertical_move;
            girl_vertical_move = !girl_vertical_move;
            glutTimerFunc(vertical_movement_speed,timeMoveVertical, 1);
        }
    }
    else if (characterIsFloating(boy, walls[current_level]))
    {
        if (!vertical_movement_lock)
        {
            vertical_movement_lock = true;
            boy_vertical_move = !boy_vertical_move;
            glutTimerFunc(vertical_movement_speed,timeMoveVertical, 1);
        }
    }
    else if (characterIsFloating(girl, walls[current_level]))
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
        if (!collisionRight(boy, walls[current_level]) && (!collisionRight(boy, buttons[current_level].getDoors()) || buttons[current_level].isActive()) )
        {
            boy.setX(boy.getX() + 1);
        }
        if (!collisionLeft(girl, walls[current_level]) && (!collisionLeft(girl, buttons[current_level].getDoors()) || buttons[current_level].isActive()))
        {
            girl.setX(girl.getX() - 1);
        }
        
        moveCharactersFloating();
        
        if (checkGoalReached())
        {
            nextLevel();
            glutPostRedisplay();
        }
        else if ( collisionButton(boy, buttons[current_level]) || collisionButton(girl, buttons[current_level]) )
        {
            buttons[current_level].setActive(true);
            horizontal_movement_lock = false;
            glutPostRedisplay();
        }
        else if (collisionSpikes(boy, spikes[current_level]) || collisionSpikes(girl, spikes[current_level]))
        {
            resetLevel();
            glutPostRedisplay();
        }
        else
        {
            glutPostRedisplay();
            glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalRight, --v);
        }
    }
    else
    {
        horizontal_movement_lock = false;
    }
}

void timeMoveHorizontalLeft(int v)
{
    if (v >= 0)
    {
        if (!collisionLeft(boy, walls[current_level]) && (!collisionLeft(boy, buttons[current_level].getDoors()) || buttons[current_level].isActive()))
        {
            boy.setX(boy.getX() - 1);
        }
        if (!collisionRight(girl, walls[current_level]) && (!collisionRight(girl, buttons[current_level].getDoors()) || buttons[current_level].isActive()))
        {
            girl.setX(girl.getX() + 1);
        }
        
        moveCharactersFloating();
        
        if (checkGoalReached())
        {
            nextLevel();
            glutPostRedisplay();
        }
        else if ( collisionButton(boy, buttons[current_level]) || collisionButton(girl, buttons[current_level]) )
        {
            buttons[current_level].setActive(true);
            horizontal_movement_lock = false;
            glutPostRedisplay();
        }
        else if (collisionSpikes(boy, spikes[current_level]) || collisionSpikes(girl, spikes[current_level]))
        {
            resetLevel();
            glutPostRedisplay();
        }
        else
        {
            glutPostRedisplay();
            glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalLeft, --v);
        }
    }
    else
    {
        horizontal_movement_lock = false;
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
            
            if (-1.0f == boy_diff_collision && !buttons[current_level].isActive())
            {
                boy_diff_collision = collisionTop(boy, buttons[current_level].getDoors());
            }
            
            if (-1.0f == boy_diff_collision) //si no colisiona
            {
                boy.setY(boy.getY() + vertical_movement_range);
                
                if (checkGoalReached())
                {
                    nextLevel();
                    glutPostRedisplay();
                }
                else if ( collisionSpikes(boy, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                else
                {
                    if ( collisionButton(boy, buttons[current_level]) )
                    {
                        buttons[current_level].setActive(true);
                    }
                    
                    glutPostRedisplay();
                    glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
                }
            }
            else //si colisiona
            {
                if (0.0f != boy_diff_collision)
                {
                    boy.setY(boy.getY() + boy_diff_collision);
                }
                
                boy_vertical_move = false;
                if (!girl_vertical_move)
                {
                    vertical_movement_lock = false;
                }
                
                if (checkGoalReached())
                {
                    nextLevel();
                }
                else if ( collisionButton(boy, buttons[current_level]) )
                {
                    buttons[current_level].setActive(true);
                }
                else if ( collisionSpikes(boy, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                
                glutPostRedisplay();
            }
        }
        else //si va para abajo
        {
            boy_diff_collision = collisionBottom(boy, walls[current_level]);
            
            if (-1.0f == boy_diff_collision && !buttons[current_level].isActive())
            {
                boy_diff_collision = collisionBottom(boy , buttons[current_level].getDoors());
            }
            
            if (-1.0f == boy_diff_collision)
            {
                boy.setY(boy.getY() - vertical_movement_range);
                
                if (checkGoalReached())
                {
                    nextLevel();
                    glutPostRedisplay();
                }
                else if ( collisionSpikes(boy, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                else
                {
                    if ( collisionButton(boy, buttons[current_level]) )
                    {
                        buttons[current_level].setActive(true);
                    }
                    
                    glutPostRedisplay();
                    glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
                }
            }
            else
            {
                if (0.0f != boy_diff_collision)
                {
                    boy.setY(boy.getY() - boy_diff_collision);
                }
                
                boy_vertical_move = false;
                if (!girl_vertical_move)
                {
                    vertical_movement_lock = false;
                }
                
                if (checkGoalReached())
                {
                    nextLevel();
                }
                else if ( collisionButton(boy, buttons[current_level]) )
                {
                    buttons[current_level].setActive(true);
                }
                else if ( collisionSpikes(boy, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
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
            
            if (-1.0f == girl_diff_collision && !buttons[current_level].isActive())
            {
                girl_diff_collision = collisionTop(girl, buttons[current_level].getDoors());
            }
            
            if (-1.0f == girl_diff_collision) //si no colisiona
            {
                girl.setY(girl.getY() + vertical_movement_range);
                
                if (checkGoalReached())
                {
                    nextLevel();
                    glutPostRedisplay();
                }
                else if ( collisionSpikes(girl, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                else
                {
                    if ( collisionButton(girl, buttons[current_level]) )
                    {
                        buttons[current_level].setActive(true);
                    }
                    
                    glutPostRedisplay();
                    glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
                }
            }
            else //si colisiona
            {
                if (0.0f != girl_diff_collision)
                {
                    girl.setY(girl.getY() + girl_diff_collision);
                }
                
                girl_vertical_move = false;
                if (!boy_vertical_move)
                {
                    vertical_movement_lock = false;
                }
                
                if (checkGoalReached())
                {
                    nextLevel();
                }
                else if ( collisionButton(girl, buttons[current_level]) )
                {
                    buttons[current_level].setActive(true);
                }
                else if ( collisionSpikes(girl, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                
                glutPostRedisplay();
            }
        }
        else //si va para abajo
        {
            girl_diff_collision = collisionBottom(girl, walls[current_level]);
            
            if (-1.0f == girl_diff_collision && !buttons[current_level].isActive())
            {
                girl_diff_collision = collisionBottom(girl, buttons[current_level].getDoors());
            }
            
            if (-1.0f == girl_diff_collision)
            {
                girl.setY(girl.getY() - vertical_movement_range);
                if (checkGoalReached())
                {
                    nextLevel();
                    glutPostRedisplay();
                }
                else if ( collisionSpikes(girl, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
                }
                else
                {
                    if ( collisionButton(girl, buttons[current_level]) )
                    {
                        buttons[current_level].setActive(true);
                    }
                    
                    glutPostRedisplay();
                    glutTimerFunc(vertical_movement_speed,timeMoveVertical,1);
                }
            }
            else
            {
                if (0.0f != girl_diff_collision)
                {
                    girl.setY(girl.getY() - girl_diff_collision);
                }
                
                girl_vertical_move = false;
                if (!boy_vertical_move)
                {
                    vertical_movement_lock = false;
                }
                
                if (checkGoalReached())
                {
                    nextLevel();
                }
                else if ( collisionButton(girl, buttons[current_level]) )
                {
                    buttons[current_level].setActive(true);
                }
                else if ( collisionSpikes(girl, spikes[current_level]) )
                {
                    resetLevel();
                    glutPostRedisplay();
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

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    if ((current_level != 0 && current_level < 11))
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
            case 'n':
            case 'N':
                nextLevel();
                break;
            case 'r':
            case 'R':
                resetLevel();
                /*case 'f':
                 std::cout << "Floating: " << (characterIsFloating(boy, walls[current_level]) ? "True" : "False") << std::endl;
                 break;*/
            default:
                break;
        }
    }
}

void movement(int key, int x, int y)
{
    if (!horizontal_movement_lock)
    {
        if (current_level != 0 && current_level < LISTS_COUNT - 1)
        {
            if (GLUT_KEY_LEFT == key)
            {
                horizontal_movement_lock = true;
                boy_side = 1;
                girl_side = 0;
                glutTimerFunc(horizontal_movement_speed,timeMoveHorizontalLeft,10);
                glutPostRedisplay();
            }
            else if (GLUT_KEY_RIGHT == key)
            {
                boy_side = 0;
                girl_side = 1;
                horizontal_movement_lock = true;
                glutTimerFunc(horizontal_movement_speed, timeMoveHorizontalRight, 10);
                glutPostRedisplay();
            }
        }
    }
}

void nextLevel()
{
    current_level++;
    (current_level > 10 ? current_level = 0 : 0);
    addMenu();
    resetLevel();
}

void changeLevel(int level)
{
    current_level = level;
    
    if (0 == current_level || 11 == current_level)
    {
        glutDetachMenu(GLUT_RIGHT_BUTTON);
        glutDestroyMenu(1);
    }
    else
    {
        addMenu();
    }
    
    resetLevel();
}

void resetLevel()
{
    vertical_movement_lock = false;
    horizontal_movement_lock = false;
    
    boy_vertical_move = false;
    girl_vertical_move = false;
    
    boy_move_down = (character_init_pos[current_level][5] == 0 ? true : false);
    girl_move_down = (character_init_pos[current_level][7] == 0 ? true : false);
    
    buttons[current_level].setActive(false);
    
    boy_side = character_init_pos[current_level][4];
    girl_side = character_init_pos[current_level][6];
    
    boy.setX(character_init_pos[current_level][0]);
    boy.setY(character_init_pos[current_level][1]);
    girl.setX(character_init_pos[current_level][2]);
    girl.setY(character_init_pos[current_level][3]);
    glutPostRedisplay();
}

void myMouseButton(int button, int state, int mouseX, int mouseY)
{
    if (0 == current_level)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            GLint x = mouseX;
            GLint y = window_height - mouseY; //el alto se puede modificar con la ventana
            if (x >= 366 && x <= 494 && y >= 249 && y <= 304) // iniciar
            {
                nextLevel();
            }
            else if (x >= 314 && x <= 547 && y >= 177 && y <= 232) // instrucciones
            {
                changeLevel(11);
            }
            else if (x >= 366 && x <= 494 && y >= 107 && y <= 162) // salir
            {
                exit(0);
            }
        }
    }
    if (11 == current_level)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            GLint x = mouseX;
            GLint y = window_height - mouseY; //el alto se puede modificar con la ventana
            if (x >= 366 && x <= 494 && y >= 38 && y <= 93) // iniciar
            {
                changeLevel(0);
            }
        }
    }
}

void procesaMenu(int val)
{
    if (20 == val)
    {
        changeLevel(0);
    }
    else if (10 == val)
    {
        resetLevel();
    }
}

void addMenu()
{
    if (current_level != 0 && current_level < LISTS_COUNT)
    {
        int mainMenu;
        
        mainMenu = glutCreateMenu(procesaMenu);
        
        glutSetMenu(mainMenu);
        glutAddMenuEntry("Reiniciar Nivel", 10);
        glutAddMenuEntry("Salir", 20);
        glutSetMenu(mainMenu);
        
        glutAttachMenu(GLUT_RIGHT_BUTTON);
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
    glEnable(GL_TEXTURE_2D);
    
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, textures[0]);
    
        if (0 == boy_side && boy_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(boy.getX(), boy.getY());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY() + boy.getHeight());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(boy.getX(), boy.getY() + boy.getHeight());
            glEnd();
        }
        else if (1 == boy_side && boy_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(boy.getX(), boy.getY());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY() + boy.getHeight());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(boy.getX(), boy.getY() + boy.getHeight());
            glEnd();
        }
        else if (0 == boy_side && !boy_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(boy.getX(), boy.getY());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY() + boy.getHeight());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(boy.getX(), boy.getY() + boy.getHeight());
            glEnd();
        }
        else if (1 == boy_side && !boy_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(boy.getX(), boy.getY());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(boy.getX() + boy.getWidth(), boy.getY() + boy.getHeight());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(boy.getX(), boy.getY() + boy.getHeight());
            glEnd();
        }
    glPopMatrix();
    
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        
        if (0 == girl_side && girl_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(girl.getX(), girl.getY());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY() + girl.getHeight());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(girl.getX(), girl.getY() + girl.getHeight());
            glEnd();
        }
        else if (1 == girl_side && girl_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(girl.getX(), girl.getY());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY() + girl.getHeight());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(girl.getX(), girl.getY() + girl.getHeight());
            glEnd();
        }
        else if (0 == girl_side && !girl_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(girl.getX(), girl.getY());
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY() + girl.getHeight());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(girl.getX(), girl.getY() + girl.getHeight());
            glEnd();
        }
        else if (1 == girl_side && !girl_move_down)
        {
            glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(girl.getX(), girl.getY());
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(girl.getX() + girl.getWidth(), girl.getY() + girl.getHeight());
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(girl.getX(), girl.getY() + girl.getHeight());
            glEnd();
        }
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void drawWalls(std::list<Block> list)
{
    glChangeColorHEX(255, 0, 0);
    for (unsigned long x = 0; x < list.size(); x++)
    {
        paintWall(frontListItem(list), -1);
        nextListItem(list);
    }
    glChangeColorHEX(255, 255, 255);
}

void drawSpikes(std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        paintWall(frontListItem(list), 2);
        nextListItem(list);
    }
    
    glChangeColorHEX(255, 255, 255);
}

void drawDoors(std::list<Block> list)
{
    for (unsigned long x = 0; x < list.size(); x++)
    {
        paintWall(frontListItem(list), 4);
        nextListItem(list);
    }
}

void paintWall(Block wall, int txt)
{
    if (-1 != txt)
    {
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, textures[txt]);
        
        //TRBL
        if (1 == wall.getSide())
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(wall.getX(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), 0.0f);
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), wall.getRepeatY());
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
            glTexCoord2f(0.0f, wall.getRepeatY());
            glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
            glEnd();
        }
        else if (2 == wall.getSide())
        {
            glBegin(GL_QUADS);
            glTexCoord2f(wall.getRepeatX(), 0.0f);
            glVertex2f(wall.getX(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), wall.getRepeatY());
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
            glTexCoord2f(0.0f, wall.getRepeatY());
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
            glEnd();
        }
        else if (3 == wall.getSide())
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, wall.getRepeatY());
            glVertex2f(wall.getX(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), wall.getRepeatY());
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), 0.0f);
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
            glEnd();
        }
        else if (4 == wall.getSide())
        {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, wall.getRepeatY());
            glVertex2f(wall.getX(), wall.getY());
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
            glTexCoord2f(wall.getRepeatX(), 0.0f);
            glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
            glTexCoord2f(wall.getRepeatX(), wall.getRepeatY());
            glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
            glEnd();
        }
        
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glBegin(GL_QUADS);
        glVertex2f(wall.getX(), wall.getY());
        glVertex2f(wall.getX() + wall.getWidth(), wall.getY());
        glVertex2f(wall.getX() + wall.getWidth(), wall.getY() + wall.getHeight());
        glVertex2f(wall.getX(), wall.getY() + wall.getHeight());
        glEnd();
    }
}

void drawButtonDoors(Button button)
{
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(button.getX(), button.getY());
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(button.getX() + button.getWidth(), button.getY());
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(button.getX() + button.getWidth(), button.getY() + button.getHeight());
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(button.getX(), button.getY() + button.getHeight());
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    if (!button.isActive())
    {
        drawDoors(button.getDoors());
    }
}

void drawGoals()
{
    glChangeColorHEX(0, 0, 255);
    paintWall(goals[current_level][0], -1);
    paintWall(goals[current_level][1], -1);
    glChangeColorHEX(255, 255, 255);
}

void drawBackground()
{
    GLfloat image_size = 1024.0;
    
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, texturesBack[current_level]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(image_size, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(image_size, image_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, image_size);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawBackground();
    
    drawCharacters();
    
   // drawWalls(walls[current_level]);
    
    drawSpikes(spikes[current_level]);
    
    drawButtonDoors(buttons[current_level]);
    
   // drawGoals();
    
    glutSwapBuffers();
}


/***************************************************************************
 ******************             FIN DE PINTADO             *****************
 ***************************************************************************/










void initRendering()
{
    Image* image;
    
    glEnable(GL_TEXTURE_2D);
    
    /** FONDOS **/
    glGenTextures(12, texturesBack); //Make room for our texture
    
    image = loadBMP("images/background/inicio.bmp");
    loadTexture(image, texturesBack, 0);
    image = loadBMP("images/background/Nivel1.bmp");
    loadTexture(image, texturesBack, 1);
    image = loadBMP("images/background/Nivel2.bmp");
    loadTexture(image, texturesBack, 2);
    image = loadBMP("images/background/Nivel3.bmp");
    loadTexture(image, texturesBack, 3);
    image = loadBMP("images/background/Nivel4.bmp");
    loadTexture(image, texturesBack, 4);
    image = loadBMP("images/background/Nivel5.bmp");
    loadTexture(image, texturesBack, 5);
    image = loadBMP("images/background/Nivel6.bmp");
    loadTexture(image, texturesBack, 6);
    image = loadBMP("images/background/Nivel7.bmp");
    loadTexture(image, texturesBack, 7);
    image = loadBMP("images/background/Nivel8.bmp");
    loadTexture(image, texturesBack, 8);
    image = loadBMP("images/background/Nivel9.bmp");
    loadTexture(image, texturesBack, 9);
    image = loadBMP("images/background/felicidades.bmp");
    loadTexture(image, texturesBack, 10);
    image = loadBMP("images/background/instrucciones.bmp");
    loadTexture(image, texturesBack, 11);
    
    /** OTRAS TEXTURAS **/
    glGenTextures(5, textures); //Make room for our texture
    image = loadBMP("images/character/boy.bmp");
    loadTexture(image, textures, 0);
    image = loadBMP("images/character/girl.bmp");
    loadTexture(image, textures, 1);
    image = loadBMP("images/other/spike.bmp");
    loadTexture(image, textures, 2);
    image = loadBMP("images/other/button.bmp");
    loadTexture(image, textures, 3);
    image = loadBMP("images/other/door.bmp");
    loadTexture(image, textures, 4);
    
    delete image;
}

void init()
{
    glChangeBackgroundColorHEX(100, 100, 100, 1.0);
    
    boy = Character(character_init_pos[current_level][0], character_init_pos[current_level][1], 40.0, 50.0);
    girl = Character(character_init_pos[current_level][2], character_init_pos[current_level][3], 40.0, 50.0);
    
    //Inicialización de paredes externas
    for (unsigned long x = 0; x < LISTS_COUNT; x++) {
        walls[x].push_back(Block(-5,0,5, window_height));
        walls[x].push_back(Block(window_width, 0, 5, window_height));
        walls[x].push_back(Block(0,-5,window_width, 5));
        walls[x].push_back(Block(0, window_height, window_width, 5));
    }
    
    /** NIVEL 1 **/
    walls[1].push_back(Block(0, 0, 860, 102));
    walls[1].push_back(Block(0, 487, 860, 102));
    walls[1].push_back(Block(297, 253, 221, 88));

    /** NIVEL 2 **/
    walls[2].push_back(Block(0, 0, 40, 265));
    walls[2].push_back(Block(0, 0, 140, 143));
    walls[2].push_back(Block(140, 0, 720, 63));
    walls[2].push_back(Block(519, 63, 81, 260));
    walls[2].push_back(Block(779, 63, 81, 514));
    walls[2].push_back(Block(219, 225, 300, 98));
    walls[2].push_back(Block(0, 265, 340, 312));
    walls[2].push_back(Block(340, 445, 180, 132));
    walls[2].push_back(Block(520, 525, 260, 52));

    buttons[2] = Button(551, 323, 32, 8);
    buttons[2].addDoor(Block(300, 63, 32, 162, 1, 1.0, 2.53125));
    
    /** NIVEL 3 **/
    walls[3].push_back(Block(0, 0, 40, 577));
    walls[3].push_back(Block(40, 0, 820, 58));
    walls[3].push_back(Block(343, 58, 141, 260));
    walls[3].push_back(Block(484, 58, 120, 130));
    walls[3].push_back(Block(819, 58, 41, 519));
    walls[3].push_back(Block(40, 340, 59, 90));
    walls[3].push_back(Block(99, 116, 121, 314));
    walls[3].push_back(Block(220, 116, 44, 202));
    walls[3].push_back(Block(220, 402, 513, 28));
    walls[3].push_back(Block(692, 252, 41, 150));
    walls[3].push_back(Block(643, 252, 49, 66));
    walls[3].push_back(Block(301, 430, 130, 43));
    walls[3].push_back(Block(719, 485, 100, 40));
    walls[3].push_back(Block(39, 525, 780, 52));
    
    buttons[3] = Button(545, 188, 32, 8);
    buttons[3].addDoor(Block(606, 430, 32, 95, 1, 1.0, 1.484375));
    
    /** NIVEL 4 **/
    walls[4].push_back(Block(0, 0, 80, 577));
    walls[4].push_back(Block(80, 0, 780, 63));
    walls[4].push_back(Block(780, 63, 81, 514));
    walls[4].push_back(Block(80, 485, 702, 92));
    walls[4].push_back(Block(139, 63, 41, 300));
    walls[4].push_back(Block(180, 265, 300, 98));
    walls[4].push_back(Block(299, 186, 181, 79));
    walls[4].push_back(Block(439, 125, 300, 78));
    walls[4].push_back(Block(299, 445, 101, 41));
     
    spikes[4].push_back(Block(442, 363, 16, 16, 1));
    spikes[4].push_back(Block(458, 363, 16, 16, 1));
    
    spikes[4].push_back(Block(145, 363, 16, 16, 1));
    spikes[4].push_back(Block(161, 363, 16, 16, 1));
    
    spikes[4].push_back(Block(400, 470, 16, 16, 3));
    spikes[4].push_back(Block(416, 470, 16, 16, 3));
   
    spikes[4].push_back(Block(536, 470, 16, 16, 3));
    spikes[4].push_back(Block(552, 470, 16, 16, 3));
    spikes[4].push_back(Block(568, 470, 16, 16, 3));
    spikes[4].push_back(Block(584, 470, 16, 16, 3));
    spikes[4].push_back(Block(600, 470, 16, 16, 3));
    spikes[4].push_back(Block(616, 470, 16, 16, 3));
    spikes[4].push_back(Block(632, 470, 16, 16, 3));
    spikes[4].push_back(Block(648, 470, 16, 16, 3));
    spikes[4].push_back(Block(664, 470, 16, 16, 3));
    spikes[4].push_back(Block(680, 470, 16, 16, 3));

    spikes[4].push_back(Block(480, 202, 16, 16, 1));
    spikes[4].push_back(Block(496, 202, 16, 16, 1));
    spikes[4].push_back(Block(512, 202, 16, 16, 1));
    spikes[4].push_back(Block(528, 202, 16, 16, 1));
    spikes[4].push_back(Block(544, 202, 16, 16, 1));

    spikes[4].push_back(Block(654, 202, 16, 16, 1));
    spikes[4].push_back(Block(670, 202, 16, 16, 1));
    spikes[4].push_back(Block(686, 202, 16, 16, 1));
    spikes[4].push_back(Block(702, 202, 16, 16, 1));
    spikes[4].push_back(Block(718, 202, 16, 16, 1));

    spikes[4].push_back(Block(566, 62, 16, 16, 1));
    spikes[4].push_back(Block(582, 62, 16, 16, 1));
    spikes[4].push_back(Block(598, 62, 16, 16, 1));
    spikes[4].push_back(Block(614, 62, 16, 16, 1));
    spikes[4].push_back(Block(630, 62, 16, 16, 1));
    
    buttons[4] = Button(350, 62, 32, 8);
    buttons[4].addDoor(Block(80, 185, 59, 32, 2, 0.92, 1.0));
    
    /** NIVEL 5 **/
    walls[5].push_back(Block(0, 0, 40, 577));
    walls[5].push_back(Block(40, 0, 820, 28));
    walls[5].push_back(Block(40, 525, 820, 52));
    walls[5].push_back(Block(559, 345, 301, 180));
    walls[5].push_back(Block(559, 185, 121, 160));
    walls[5].push_back(Block(680, 185, 95, 98));
    walls[5].push_back(Block(564, 0, 41, 103));
    walls[5].push_back(Block(605, 0, 255, 63));
    walls[5].push_back(Block(819, 63, 41, 283));
    walls[5].push_back(Block(119, 125, 141, 238));
    walls[5].push_back(Block(119, 85, 41, 40));
    walls[5].push_back(Block(219, 85, 41, 40));
    walls[5].push_back(Block(260, 265, 299, 98));
    walls[5].push_back(Block(299, 363, 41, 39));
    walls[5].push_back(Block(349, 155, 121, 50));
    
    spikes[5].push_back(Block(40, 509, 16, 16, 3));
    spikes[5].push_back(Block(56, 509, 16, 16, 3));
    spikes[5].push_back(Block(72, 509, 16, 16, 3));
    spikes[5].push_back(Block(88, 509, 16, 16, 3));
    spikes[5].push_back(Block(104, 509, 16, 16, 3));
   
    spikes[5].push_back(Block(126, 362, 16, 16, 1));
    spikes[5].push_back(Block(142, 362, 16, 16, 1));
    spikes[5].push_back(Block(158, 362, 16, 16, 1));
    spikes[5].push_back(Block(174, 362, 16, 16, 1));
    
    spikes[5].push_back(Block(205, 509, 16, 16, 3));
    spikes[5].push_back(Block(221, 509, 16, 16, 3));
    spikes[5].push_back(Block(237, 509, 16, 16, 3));
    spikes[5].push_back(Block(253, 509, 16, 16, 3));
    spikes[5].push_back(Block(269, 509, 16, 16, 3));
    
    spikes[5].push_back(Block(341, 362, 16, 16, 1));
    spikes[5].push_back(Block(357, 362, 16, 16, 1));
    
    spikes[5].push_back(Block(362, 140, 16, 16, 3));
    spikes[5].push_back(Block(378, 140, 16, 16, 3));
    spikes[5].push_back(Block(394, 140, 16, 16, 3));
    spikes[5].push_back(Block(410, 140, 16, 16, 3));
    spikes[5].push_back(Block(426, 140, 16, 16, 3));
    spikes[5].push_back(Block(442, 140, 16, 16, 3));
    
    spikes[5].push_back(Block(362, 27, 16, 16, 1));
    spikes[5].push_back(Block(378, 27, 16, 16, 1));
    spikes[5].push_back(Block(394, 27, 16, 16, 1));
    spikes[5].push_back(Block(410, 27, 16, 16, 1));
    spikes[5].push_back(Block(426, 27, 16, 16, 1));
    spikes[5].push_back(Block(442, 27, 16, 16, 1));
    
    /** NIVEL 6**/
    walls[6].push_back(Block(0, 0, 40, 577));
    walls[6].push_back(Block(40, 445, 260, 132));
    walls[6].push_back(Block(40, 0, 260, 63));
    walls[6].push_back(Block(259, 125, 41, 320));
    walls[6].push_back(Block(300, 265, 40, 98));
    walls[6].push_back(Block(99, 265, 160, 98));
    walls[6].push_back(Block(219, 125, 40, 140));
    walls[6].push_back(Block(139, 125, 80, 78));
    walls[6].push_back(Block(299, 0, 260, 16));
    walls[6].push_back(Block(559, 0, 301, 103));
    walls[6].push_back(Block(299, 525, 460, 52));
    walls[6].push_back(Block(759, 103, 101, 474));
    walls[6].push_back(Block(639, 485, 120, 40));
    walls[6].push_back(Block(519, 485, 41, 40));
    walls[6].push_back(Block(639, 425, 61, 60));
    walls[6].push_back(Block(639, 265, 120, 58));
  
    spikes[6].push_back(Block(302, 15, 16, 16, 1));
    spikes[6].push_back(Block(318, 15, 16, 16, 1));
    spikes[6].push_back(Block(334, 15, 16, 16, 1));
    spikes[6].push_back(Block(350, 15, 16, 16, 1));
    spikes[6].push_back(Block(366, 15, 16, 16, 1));
    spikes[6].push_back(Block(382, 15, 16, 16, 1));
    spikes[6].push_back(Block(398, 15, 16, 16, 1));
    spikes[6].push_back(Block(414, 15, 16, 16, 1));
    spikes[6].push_back(Block(430, 15, 16, 16, 1));
    spikes[6].push_back(Block(446, 15, 16, 16, 1));
    spikes[6].push_back(Block(462, 15, 16, 16, 1));
    spikes[6].push_back(Block(478, 15, 16, 16, 1));
    spikes[6].push_back(Block(494, 15, 16, 16, 1));
    spikes[6].push_back(Block(510, 15, 16, 16, 1));
    spikes[6].push_back(Block(526, 15, 16, 16, 1));
    spikes[6].push_back(Block(542, 15, 16, 16, 1));
    
    spikes[6].push_back(Block(696, 322, 16, 16, 1));
    spikes[6].push_back(Block(712, 322, 16, 16, 1));
    spikes[6].push_back(Block(728, 322, 16, 16, 1));
    spikes[6].push_back(Block(744, 322, 16, 16, 1));

    spikes[6].push_back(Block(305, 510, 16, 16, 3));
    spikes[6].push_back(Block(323, 510, 16, 16, 3));
    spikes[6].push_back(Block(339, 510, 16, 16, 3));
    spikes[6].push_back(Block(355, 510, 16, 16, 3));
    spikes[6].push_back(Block(371, 510, 16, 16, 3));
    spikes[6].push_back(Block(387, 510, 16, 16, 3));
    spikes[6].push_back(Block(403, 510, 16, 16, 3));
    spikes[6].push_back(Block(419, 510, 16, 16, 3));
    spikes[6].push_back(Block(435, 510, 16, 16, 3));
    spikes[6].push_back(Block(451, 510, 16, 16, 3));
    spikes[6].push_back(Block(467, 510, 16, 16, 3));
    spikes[6].push_back(Block(483, 510, 16, 16, 3));
    spikes[6].push_back(Block(499, 510, 16, 16, 3));
    
    spikes[6].push_back(Block(560, 510, 16, 16, 3));
    spikes[6].push_back(Block(576, 510, 16, 16, 3));
    spikes[6].push_back(Block(592, 510, 16, 16, 3));
    spikes[6].push_back(Block(608, 510, 16, 16, 3));
    spikes[6].push_back(Block(624, 510, 16, 16, 3));
    
    buttons[6] = Button(720, 102, 32, 8);
    
    buttons[6].addDoor(Block(300, 435, 340, 50, 2, 1.0, 5.31));
    buttons[6].addDoor(Block(648, 322, 44, 103, 1, 1.0, 1.60));
    
    /** NIVEL 7 **/
    walls[7].push_back(Block(0, 0, 90, 577));
    walls[7].push_back(Block(92, 485, 768, 92));
    walls[7].push_back(Block(92, 0, 160, 63));
    walls[7].push_back(Block(252, 0, 179, 16));
    walls[7].push_back(Block(431, 0, 429, 63));
    walls[7].push_back(Block(771, 63, 89, 422));
    walls[7].push_back(Block(735, 306, 41, 179));
    walls[7].push_back(Block(431, 266, 181, 157));
    walls[7].push_back(Block(431, 146, 41, 120));
    walls[7].push_back(Block(551, 335, 81, 151));
    walls[7].push_back(Block(632, 335, 60, 88));
    walls[7].push_back(Block(531, 98, 181, 70));
    walls[7].push_back(Block(571, 168, 41, 40));
    walls[7].push_back(Block(131, 106, 121, 297));
    
    spikes[7].push_back(Block(96, 235, 16, 16, 1));
    spikes[7].push_back(Block(112, 235, 16, 16, 1));

    spikes[7].push_back(Block(254, 15, 16, 16, 1));
    spikes[7].push_back(Block(270, 15, 16, 16, 1));
    spikes[7].push_back(Block(286, 15, 16, 16, 1));
    spikes[7].push_back(Block(302, 15, 16, 16, 1));
    spikes[7].push_back(Block(318, 15, 16, 16, 1));
    spikes[7].push_back(Block(334, 15, 16, 16, 1));
    spikes[7].push_back(Block(350, 15, 16, 16, 1));
    spikes[7].push_back(Block(366, 15, 16, 16, 1));
    spikes[7].push_back(Block(382, 15, 16, 16, 1));
    spikes[7].push_back(Block(398, 15, 16, 16, 1));
    spikes[7].push_back(Block(414, 15, 16, 16, 1));

    buttons[7] = Button(477, 422, 32, 8);
    
    buttons[7].addDoor(Block(92, 194, 40, 41, 2, 1.0, 0.625));
    
    /** NIVEL 8 **/
    walls[8].push_back(Block(0, 0, 69, 577));
    walls[8].push_back(Block(69, 0, 719, 16));
    walls[8].push_back(Block(69, 545, 719,32));
    walls[8].push_back(Block(788, 0, 72, 577));
    walls[8].push_back(Block(69, 106, 100, 77));
    walls[8].push_back(Block(128, 183, 41, 60));
    walls[8].push_back(Block(169, 146, 180, 53));
    walls[8].push_back(Block(308, 106, 41, 40));
    walls[8].push_back(Block(428, 395, 41, 151));
    walls[8].push_back(Block(348, 395, 80, 88));
    walls[8].push_back(Block(348, 249, 41, 149));
    walls[8].push_back(Block(268, 249, 281, 57));
    walls[8].push_back(Block(390, 106, 37, 145));
    walls[8].push_back(Block(175, 366, 121, 97));
    walls[8].push_back(Block(521, 366, 121, 97));
    walls[8].push_back(Block(648, 106, 140, 117));
    walls[8].push_back(Block(468, 106, 180, 77));

    spikes[8].push_back(Block(69, 530, 16, 16, 3));
    spikes[8].push_back(Block(85, 530, 16, 16, 3));
    spikes[8].push_back(Block(101, 530, 16, 16, 3));
    spikes[8].push_back(Block(117, 530, 16, 16, 3));
    spikes[8].push_back(Block(133, 530, 16, 16, 3));

    spikes[8].push_back(Block(774, 530, 16, 16, 3));
    spikes[8].push_back(Block(758, 530, 16, 16, 3));
    spikes[8].push_back(Block(742, 530, 16, 16, 3));
    spikes[8].push_back(Block(726, 530, 16, 16, 3));
    spikes[8].push_back(Block(710, 530, 16, 16, 3));

    spikes[8].push_back(Block(550, 91, 16, 16, 3));
    spikes[8].push_back(Block(566, 91, 16, 16, 3));
    spikes[8].push_back(Block(582, 91, 16, 16, 3));
    spikes[8].push_back(Block(598, 91, 16, 16, 3));
    spikes[8].push_back(Block(614, 91, 16, 16, 3));
    spikes[8].push_back(Block(630, 91, 16, 16, 3));
    spikes[8].push_back(Block(646, 91, 16, 16, 3));
    
    /** NIVEL 9 **/
    walls[9].push_back(Block(0, 0, 80, 577));
    walls[9].push_back(Block(80, 0, 699, 66));
    walls[9].push_back(Block(80, 565, 699, 12));
    walls[9].push_back(Block(779, 0, 81, 577));
    walls[9].push_back(Block(79, 189, 261, 137));
    walls[9].push_back(Block(519, 189, 260, 137));
    walls[9].push_back(Block(250, 435, 361, 57));
    walls[9].push_back(Block(330, 377, 201, 60));
    
    spikes[9].push_back(Block(80, 173, 16, 16, 3));
    spikes[9].push_back(Block(96, 173, 16, 16, 3));
    spikes[9].push_back(Block(112, 173, 16, 16, 3));
    spikes[9].push_back(Block(128, 173, 16, 16, 3));
    spikes[9].push_back(Block(144, 173, 16, 16, 3));
    spikes[9].push_back(Block(160, 173, 16, 16, 3));
    spikes[9].push_back(Block(176, 173, 16, 16, 3));
    spikes[9].push_back(Block(192, 173, 16, 16, 3));
    spikes[9].push_back(Block(208, 173, 16, 16, 3));
    spikes[9].push_back(Block(224, 173, 16, 16, 3));
    spikes[9].push_back(Block(240, 173, 16, 16, 3));
    spikes[9].push_back(Block(256, 173, 16, 16, 3));
    spikes[9].push_back(Block(272, 173, 16, 16, 3));
    spikes[9].push_back(Block(288, 173, 16, 16, 3));
    spikes[9].push_back(Block(304, 173, 16, 16, 3));
   
    spikes[9].push_back(Block(764, 173, 16, 16, 3));
    spikes[9].push_back(Block(748, 173, 16, 16, 3));
    spikes[9].push_back(Block(732, 173, 16, 16, 3));
    spikes[9].push_back(Block(716, 173, 16, 16, 3));
    spikes[9].push_back(Block(700, 173, 16, 16, 3));
    spikes[9].push_back(Block(684, 173, 16, 16, 3));
    spikes[9].push_back(Block(668, 173, 16, 16, 3));
    spikes[9].push_back(Block(652, 173, 16, 16, 3));
    spikes[9].push_back(Block(636, 173, 16, 16, 3));
    spikes[9].push_back(Block(620, 173, 16, 16, 3));
    spikes[9].push_back(Block(604, 173, 16, 16, 3));
    spikes[9].push_back(Block(588, 173, 16, 16, 3));
    spikes[9].push_back(Block(572, 173, 16, 16, 3));
    spikes[9].push_back(Block(556, 173, 16, 16, 3));
    spikes[9].push_back(Block(540, 173, 16, 16, 3));

    spikes[9].push_back(Block(340, 240, 16, 16, 2));
    spikes[9].push_back(Block(340, 256, 16, 16, 2));
    spikes[9].push_back(Block(340, 272, 16, 16, 2));

    spikes[9].push_back(Block(504, 240, 16, 16, 4));
    spikes[9].push_back(Block(504, 256, 16, 16, 4));
    spikes[9].push_back(Block(504, 272, 16, 16, 4));
    
    spikes[9].push_back(Block(342, 65, 16, 16, 1));
    spikes[9].push_back(Block(358, 65, 16, 16, 1));
    spikes[9].push_back(Block(374, 65, 16, 16, 1));
    spikes[9].push_back(Block(390, 65, 16, 16, 1));
    spikes[9].push_back(Block(406, 65, 16, 16, 1));
    spikes[9].push_back(Block(422, 65, 16, 16, 1));
    spikes[9].push_back(Block(438, 65, 16, 16, 1));
    spikes[9].push_back(Block(454, 65, 16, 16, 1));
    spikes[9].push_back(Block(470, 65, 16, 16, 1));
    spikes[9].push_back(Block(486, 65, 16, 16, 1));
    spikes[9].push_back(Block(502, 65, 16, 16, 1));

    spikes[9].push_back(Block(80, 65, 16, 16, 1));
    spikes[9].push_back(Block(96, 65, 16, 16, 1));

    spikes[9].push_back(Block(764, 65, 16, 16, 1));
    spikes[9].push_back(Block(748, 65, 16, 16, 1));

    buttons[9] = Button(580, 65, 32, 8);
    
    buttons[9].addDoor(Block(340, 197, 180, 43, 2, 1.0, 2.81));
    
    /** BASES NIVEL 1 **/
    goals[1][0] = Block(390, 341, 35, 4); // NIÑO
    goals[1][1] = Block(390, 249, 35, 4); // NIÑA
    
    /** BASES NIVEL 2 **/
    goals[2][0] = Block(412, 323, 38, 4); // NIÑO
    goals[2][1] = Block(412, 221, 38, 4); // NIÑA

    /** BASES NIVEL 3 **/
    goals[3][0] = Block(47, 430, 35, 4); // NIÑO
    goals[3][1] = Block(47, 336, 35, 4); // NIÑA
    
    /** BASES NIVEL 4 **/
    goals[4][0] = Block(222, 363, 37, 3); // NIÑA
    goals[4][1] = Block(222, 262, 37, 3); // NIÑO
    
    /** BASES NIVEL 5 **/
    goals[5][0] = Block(694, 283, 38, 4); // NIÑA
    goals[5][1] = Block(694, 181, 38, 4); // NIÑO
    
    /** BASES NIVEL 6 **/
    goals[6][0] = Block(132, 363, 36, 4); // NIÑA
    goals[6][1] = Block(132, 262, 36, 4); // NIÑO
    
    /** BASES NIVEL 7 **/
    goals[7][0] = Block(645, 423, 35, 4); // NIÑA
    goals[7][1] = Block(645, 332, 35, 4); // NIÑO
    
    /** BASES NIVEL 8 **/
    goals[8][0] = Block(390, 483, 35, 4); // NIÑO
    goals[8][1] = Block(390, 392, 35, 4); // NIÑA
    
    /** BASES NIVEL 9 **/
    goals[9][0] = Block(301, 492, 28, 5); // NIÑO
    goals[9][1] = Block(531, 492, 28, 5); // NIÑA
    
    
    /*
     * Determinacion de cantidad de paredes por lista (no se deben declarar
     * mas paredes despues de esta linea)
     */
    for (int x = 0; x < LISTS_COUNT; x++)
    {
        walls_list_size[x] = walls[x].size();
    }
    
    for (int x = 0; x < LISTS_COUNT; x++)
    {
        spikes_list_size[x] = spikes[x].size();
    }
    
    initRendering();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glcreateWindow("Brother&Sister", window_width, window_height);
    
    init();
    
    glutDisplayFunc(display);
    //glutReshapeFunc(handleResize);
    glutMouseFunc(myMouseButton);
    glutSpecialFunc(movement);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}