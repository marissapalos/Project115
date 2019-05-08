/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>
//------New-------
#include <fstream>
#include <sstream>

/* GLUT callback Handlers */

using namespace std;

Maze *M = new Maze(10);                         // Set Maze grid size
Player *P = new Player();                       // create player


wall W[1000];                                    // wall with number of bricks
Enemies E[100];                                  // create number of enemies
Timer *T0 = new Timer();                        // animation timer

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

//-------New---------
int level; //User is allowed to decide Level 1 or Level 2
int const n = 20;
int const m = 20;
int matrix[n][m];
string line; //Read one line at a time from text file
string item; //Item could either be an enemy, wall, arrow, player, or treasure chest
int wallCount = 0; //How many walls
int enemyCount = 0; //How many enemies
int x,y; //Map coordinates
//-------------------

void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    fstream myfile; // to read in a file

    while(level != 1 && level != 2){
    cout << "What Level would you like to play? " << endl;
    cout << "1. Level 1 (Easy)" << endl;
    cout << "2. Level 2 (Expert)" << endl;
    cout << "Please enter 1 or 2" << endl;
    cin >> level;
}
    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(level == 1){
        M->loadBackgroundImage("images/bak.jpg");
        myfile.open("level1.txt");
    }

    if(level == 2){
        M->loadBackgroundImage("images/bak.jpg");
        myfile.open("level2.txt");
    }

    if(myfile.is_open()){
        cout << "Yup, file is open." << endl;
        for (int y = 0; y<20; y++){ // create an empty matrix
                for (int x = 0; x<20; x++){
                    matrix[x][y] = 0;
            }

        while ( getline (myfile,line) ){ //While there is still lines in file to be read
        stringstream ss(line); //Useful for parsing files
        ss >> item >> x >> y; // Initialize item, x, and y

        if(item == "enemy"){
            if(level == 1){
            E[enemyCount].initEnm(M->getGridSize(),4,"images/e.png"); //Load dragon image
            }
            else{E[enemyCount].initEnm(M->getGridSize(),4,"images/e.png");} // Load bat image
            E[enemyCount].placeEnemy(float(x%(M->getGridSize())),float(y%(M->getGridSize())));
            enemyCount++;
            }

        else if(item == "wall"){
            if(level == 1){
                W[wallCount].wallInit(M->getGridSize(),"images/wall.png");
            }

            else{ W[wallCount].wallInit(M->getGridSize(),"images/bak.jpg");}

                W[wallCount].placeWall(x,y); // place the wall
                wallCount++; // increment the amount of walls
                matrix[x][y] = 1; //Arrange items in matrix
            }

        else if(item =="arrow"){ // if arrow
            M->loadSetOfArrowsImage("images/arr.png");      // load set of arrows image
            M->placeStArrws(x,y);
        }

        else if(item =="player"){ // load player
            P->initPlayer(M->getGridSize(),"images/k.png",6);   // initialize player pass grid size,image and number of frames
            P->loadArrowImage("images/arr.png");                // Load arrow image
            P->placePlayer(x,y);
        }

        else if(item =="chest"){ //load chest
           M->loadChestImage("images/chest.png");              // load chest image
           M->placeChest(x,y);
            }
        }
    myfile.close();
}
}
    }


void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        glPushMatrix();
         M->drawBackground();
        glPopMatrix();


        for(int i=0; i<M->getGridSize();i++)
        {
           W[i].drawWall();
        }

        glPushMatrix();
            M->drawGrid();
        glPopMatrix();

        glPushMatrix();
            P->drawplayer();
        glPopMatrix();

        for(int i=0; i<10;i++)
        {
            if(E[i].live){
            E[i].drawEnemy();
             }
        }

        glPushMatrix();
            P->drawArrow();
        glPopMatrix();

         glPushMatrix();
           M->drawChest();
        glPopMatrix();

        glPushMatrix();
           M->drawArrows();
        glPopMatrix();

    glutSwapBuffers();
}




void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ' ':
          // if(!M->liveSetOfArrws)      // if setof arrows were picked by player
             P->shootArrow();
        break;
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

 void idle(void)
{

    //Your Code here

    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};


void Specialkeys(int key, int x, int y)
{
int up_counter;
int down_counter;
int left_counter;
int right_counter;
    // Your Code here
    switch(key)
    {
    case GLUT_KEY_UP:
        if (matrix[P->getPlayerLoc().x][P->getPlayerLoc().y+1] == 0){
            cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            P->movePlayer("up");
            E[0].moveEnemy("up");
            E[1].moveEnemy("up");
            E[2].moveEnemy("up");
            up_counter = 0;
            down_counter = 0;
            left_counter = 0;
            right_counter = 0;
            break;
        }
        else if (matrix[P->getPlayerLoc().x][P->getPlayerLoc().y+1] != 0){
            if (up_counter < 5){
                cout << "Rawr Wall incoming D:< " << up_counter << endl;
                cout << W[2].GetWallLoc.x << W[2].GetWallLoc.y << endl;
                cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
                P->movePlayer("up");
                E[0].moveEnemy("up");
                E[1].moveEnemy("up");
                E[2].moveEnemy("up");
                up_counter++;
                break;
            }
            else{
                break;
            }
        }

    case GLUT_KEY_DOWN:
       if (matrix[P->getPlayerLoc().x][P->getPlayerLoc().y-1] == 0){
            cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            P->movePlayer("down");
            E[0].moveEnemy("down");
            E[1].moveEnemy("down");
            E[2].moveEnemy("down");
            up_counter = 0;
            down_counter = 0;
            left_counter = 0;
            right_counter = 0;
            break;
        }
        else if (matrix[P->getPlayerLoc().x][P->getPlayerLoc().y-1] != 0){
            if (down_counter < 5){
                cout << "Rawr Wall incoming D:< " << up_counter << endl;
                cout << W[2].GetWallLoc.x << W[2].GetWallLoc.y << endl;
                cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
                P->movePlayer("down");
                E[0].moveEnemy("down");
                E[1].moveEnemy("down");
                E[2].moveEnemy("down");
                down_counter++;
                break;
            }
            else{
                break;
            }
        }

    case GLUT_KEY_LEFT:
        if (matrix[P->getPlayerLoc().x-1][P->getPlayerLoc().y] == 0){
            cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            P->movePlayer("left");
            E[0].moveEnemy("left");
            E[1].moveEnemy("left");
            E[2].moveEnemy("left");
            up_counter = 0;
            down_counter = 0;
            left_counter = 0;
            right_counter = 0;
            break;
        }
        else if (matrix[P->getPlayerLoc().x-1][P->getPlayerLoc().y] != 0){
            if (left_counter < 5){
                cout << "Rawr Wall incoming D:< " << up_counter << endl;
                cout << W[2].GetWallLoc.x << W[2].GetWallLoc.y << endl;
                cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
                P->movePlayer("left");
                E[0].moveEnemy("left");
                E[1].moveEnemy("left");
                E[2].moveEnemy("left");
                left_counter++;
                break;
            }
            else{
                break;
            }
        }

    case GLUT_KEY_RIGHT:
        if (matrix[P->getPlayerLoc().x+1][P->getPlayerLoc().y] == 0){
            cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            P->movePlayer("right");
            E[0].moveEnemy("right");
            E[1].moveEnemy("right");
            E[2].moveEnemy("right");
            up_counter = 0;
            down_counter = 0;
            left_counter = 0;
            right_counter = 0;
            break;
        }
        else if (matrix[P->getPlayerLoc().x+1][P->getPlayerLoc().y] != 0){
            if (right_counter < 5){
                cout << "Rawr Wall incoming D:< " << up_counter << endl;
                cout << W[2].GetWallLoc.x << W[2].GetWallLoc.y << endl;
                cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
                P->movePlayer("right");
                E[0].moveEnemy("right");
                E[1].moveEnemy("right");
                E[2].moveEnemy("right");
                right_counter++;
                break;
            }
            else{
                break;
            }
        }

   }
  glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
