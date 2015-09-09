//
//  main.cpp
//  Memorama
//
//  Created by Marco Ramirez and Ricardo Canales on 9/8/15.
//  Copyright (c) 2015 Marco Ramirez. All rights reserved.
//

#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

int screenWidth = 720, screenHeight = 640, timer = 0, seconds = 0, minutes = 0, delta = 1, turns = 0;
vector<int> num;
vector<int> states; // 0 == hidden, 1 == selected, 2 == revealed

bool stop = true;
bool inProgress = false;
bool win = false;

int randomGen (int i) { return rand()%i;}

void randomCards() {
    num.clear();
    states = vector<int>(16, 0);
    for (int i = 0; i < 8; ++i) {
        num.push_back(i);
        num.push_back(i);
    }
    
    
    random_shuffle(num.begin(), num.end(), randomGen);
    
    cout << "vector contains: ";
    for (vector<int>::iterator it=num.begin(); it!=num.end(); ++it)
        cout << ' ' << *it;
    
    cout << endl;
}

void drawTime(std::string pTimer) {
    glPushMatrix();
    glTranslatef(20, 450, 0.0);
    glScalef(0.3, -0.3, 0.0);
    for (int x = 0; x < pTimer.length(); x++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , pTimer[x]);
    }
    glPopMatrix();
}

void getTime() {
    //Convierte de milisegundos
    seconds = timer / 10;
    minutes = seconds / 60;
    if (seconds == 60) {
        seconds = 0;
    }
    
    //Convierte tiempo a string
    std::string minutesStr = std::to_string(minutes);
    std::string secondsStr = std::to_string(seconds % 60);
    std::string milisecondsStr = std::to_string(timer % 10);
    
    //Verifica si requiere 0
    if ((seconds % 60) <= 9) {
        secondsStr = "0" + secondsStr;
    }
    
    //Manda imprimir el tiempo
    drawTime(minutesStr + ":" + secondsStr + "." + milisecondsStr);
}

void myTimer(int i) {
    delta = 1;
    if (!stop) {
        timer += delta;
    }
    glutPostRedisplay();
    glutTimerFunc(100, myTimer,1);
}

void drawCardNum(std::string text,int x,int y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(0.35, -0.35, 0.0);
    for (int c=0; c < text.length(); c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , text[c]);
    }
    glPopMatrix();
}

void drawText(std::string text,int x,int y) {
    glPushMatrix();
    glTranslatef(x - 120, y, 0.0);
    glScalef(0.15, -0.15, 0.0);
    for (int c=0; c < text.length(); c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , text[c]);
    }
    glPopMatrix();
}

void reshape(int w,int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,h,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void clearSelected(){
    for(int i = 0; i < states.size();i++){
        if(states[i] == 1){
            states[i] = 0;
        }
    }
}

void checkPairs(){
    // checa si son pares
    int selected = -1;
    bool success = false;
    for(int i = 0; i < states.size();i++){
        if(states[i] == 1 && selected == -1){
            selected = i;
        } else if(states[i] == 1 && num[i] == num[selected]){
            states[i] = 2;
            states[selected] = 2;
            success = true;
        }
    }
    if(success){
        clearSelected();
    }
    
}

void display() {
    //BKG Color
    glClearColor(0,0,1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    checkPairs();
    
    if(inProgress){
        
        // checa si esta ganado
        win = true;
        inProgress = false;
        for(int i = 0; i < states.size(); i++){
            if(states[i] != 2){
                win = false;
                inProgress = true;
                break;
            }
        }
        
        
        
    }
    if(win) drawText("You won in " + to_string(turns/2) + "!", 300, 360);
    
    // pinta Rectangulos
    int x1 = 0;
    int x2 = 45;
    int currentRectangle = 0;
    for (int x = 0; x < 8; x++) {
        if(states[currentRectangle] == 0){
            glColor3f(0.0f, 0.3f, 0.5f);
        } else if(states[currentRectangle] == 1){
            glColor3f(0.7f, 0.0f, 0.7f);
        } else {
            glColor3f(0.40f, 0.60f, 0.80f);
        }
        currentRectangle++;
        glRecti(x1,0, x2, 120);
        x1 += 45;
        x2 += 45;
        if(states[currentRectangle] == 0){
            glColor3f(0.5f, 0.0f, 0.3f);
        } else if(states[currentRectangle] == 1){
            glColor3f(0.7f, 0.0f, 0.7f);
        } else {
            glColor3f(0.40f, 0.60f, 0.80f);
        }
        currentRectangle++;
        glRecti(x1,0, x2, 120);
        x1 += 45;
        x2 += 45;
    }
    
    glColor3f(1,1,1);
    
    // pinta numeros
    string carta;
    int x=0;
    for(int i = 0; i < num.size(); i++){
        carta = to_string(num[i]);
        if(states[i] != 0)
            drawCardNum(carta, (10 + (x * 45)), 100);
        x++;
    }
    
    
    //Imprime Timer
    getTime();
    
    string turno = to_string(turns/2);
    drawText("Turno: " + turno,350,450);
    
    //Instrucciones
    drawText("'I' :Iniciar ",150,550);
    drawText("'P' :Pausa",280,550);
    drawText("'R' :Reiniciar",440,550);
    drawText("'Esc' :Salir",620,550);
    
    //Autor
    drawText("Autores: Marco Ramirez : A01191344 y Ricardo Canales : A01191463",150,500);
    
    //Intercambia los frame buffers
    glutSwapBuffers();//ya tiene integrado el glFlush
}

void onMenu(int opcion) {
    switch(opcion) {
            //Iniciar
        case 1:
            if(inProgress == false){
                randomCards();
                inProgress = true;
            }
            stop = false;
            break;
            //Reiniciar
        case 2:
            stop = true;
            timer = 0;
            turns = 0;
            win = false;
            randomCards();
            inProgress = false;
            glClear( GL_COLOR_BUFFER_BIT );
            glFlush();// Limpia la pantalla
            break;
            //Pausa
        case 3:
            stop = true;
            break;
            //Salir
        case 4:
            exit(-1);
            break;
            //Ayuda
        case 5:
            //Display/Hide Card Nums
            break;
    }
    glutPostRedisplay();
}
void crearMenu(void) {
    int autores, menuPrincipal;
    
    autores = glutCreateMenu(onMenu);
    glutAddMenuEntry("Marco Ramirez : A01191344", 0);
    glutAddMenuEntry("Ricardo Canales : A01191463", 0);
    
    menuPrincipal = glutCreateMenu(onMenu);
    glutAddMenuEntry("Iniciar", 1);
    glutAddMenuEntry("Reiniciar", 2);
    glutAddMenuEntry("Pausa", 3);
    glutAddMenuEntry("Salir", 4);
    glutAddMenuEntry("Ayuda", 5);
    glutAddSubMenu("Autores", autores);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        //Si el usuario oprime el boton izq del mouse
        int selected;
        if (button == GLUT_LEFT_BUTTON && stop != true) {
            if (y < 120 && y >= 0) {
                if(turns%2 == 0){
                    clearSelected();
                }
                selected = x / 720.0 * 16;
                if(states[selected] != 2) {
                    states[selected] = 1;
                    turns++;
                }
            }
        }
    }
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
    switch (theKey) {
            //Inicio
        case 'i':
        case 'I':
            if(inProgress == false){
                randomCards();
                inProgress = true;
            }
            stop = false;
            break;
            
            //Stop
        case 'p':
        case 'P':
            stop = true;
            break;
            
            //Reset
        case 'R':
        case 'r':
            stop = true;
            timer = 0;
            turns = 0;
            win = false;
            randomCards();
            inProgress = false;
            glClear( GL_COLOR_BUFFER_BIT );
            glFlush();// Limpia la pantalla
            break;
            
            //Salir
        case 27:
            exit(-1);
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(720,640);
    glutInitWindowPosition(100,100);
    //Double frame buffer
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
    glutCreateWindow("Memorama");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(100, myTimer,1);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    randomCards();
    crearMenu();
    glutMainLoop();
    return EXIT_SUCCESS;
}