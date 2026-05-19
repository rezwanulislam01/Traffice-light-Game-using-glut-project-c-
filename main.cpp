#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>


int state = 0;

int redTime = 3;
int yellowTime = 1;
int greenTime = 3;
int currentTime = 0;

float carX = 5.0f;
const float carY = 20.0f;
const float carW = 12.0f;
const float carH = 6.0f;
const float moveStep = 2.0f;

const float stopLineX = 80.0f;
bool gameEnded = false;
bool playerWon = false;


const int winW = 800;
const int winH = 600;


void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
      glVertex2f(x1, y1);
      glVertex2f(x2, y1);
      glVertex2f(x2, y2);
      glVertex2f(x1, y2);
    glEnd();
}


void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        float theta = i * 3.1415926f / 180.0f;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}


void drawText(float x, float y, const std::string &text) {
    glRasterPos2f(x, y);
    for (const char &c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(0.6f, 0.85f, 1.0f);
    drawRect(0, 0, 100, 100);



    glColor3f(0.15f, 0.15f, 0.15f);
    drawRect(0, 10, 100, 40);


    glColor3f(1.0f, 1.0f, 1.0f);
    for (float x = 2.0f; x < 100.0f; x += 8.0f) {
        drawRect(x, 24.5f, x + 4.0f, 25.5f);
    }


    glColor3f(1.0f, 1.0f, 1.0f);
    drawRect(stopLineX - 0.5f, 10.0f, stopLineX + 0.5f, 40.0f);



    glColor3f(0.2f, 0.2f, 0.2f);
    drawRect(stopLineX + 3.0f, 40.0f, stopLineX + 4.5f, 70.0f);


    glColor3f(0.12f, 0.12f, 0.12f);
    drawRect(stopLineX + 1.0f, 70.0f, stopLineX + 9.0f, 92.0f);


    if (state == 0) glColor3f(1.0f, 0.0f, 0.0f); else glColor3f(0.3f, 0.0f, 0.0f);
    drawCircle(stopLineX + 5.0f, 86.0f, 3.8f);

    if (state == 1) glColor3f(1.0f, 1.0f, 0.0f); else glColor3f(0.3f, 0.3f, 0.0f);
    drawCircle(stopLineX + 5.0f, 79.0f, 3.8f);

    if (state == 2) glColor3f(0.0f, 1.0f, 0.0f); else glColor3f(0.0f, 0.3f, 0.0f);
    drawCircle(stopLineX + 5.0f, 72.0f, 3.8f);


if (!gameEnded) {
    glColor3f(0.0f, 0.2f, 0.8f);
} else {
    if (playerWon) glColor3f(0.0f, 1.0f, 0.0f);
    else glColor3f(1.0f, 0.0f, 0.0f);
}
    float carLeft = carX - carW/2.0f;
    float carRight = carX + carW/2.0f;
    drawRect(carLeft, carY, carRight, carY + carH);

    drawRect(carLeft + 2.0f, carY + carH, carRight - 2.0f, carY + carH + 3.0f);

    glColor3f(0.05f, 0.05f, 0.05f);
    drawCircle(carLeft + 2.5f, carY - 1.0f, 1.5f);
    drawCircle(carRight - 2.5f, carY - 1.0f, 1.5f);


    std::string lightName = (state == 0 ? "RED" : (state == 1 ? "YELLOW" : "GREEN"));
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(2.0f, 95.0f, "Light: " + lightName);

    int remain = 0;
    if (state == 0) remain = redTime - currentTime;
    else if (state == 1) remain = yellowTime - currentTime;
    else remain = greenTime - currentTime;
    if (remain < 0) remain = 0;
    drawText(2.0f, 91.0f, "Seconds left: " + std::to_string(remain));


    drawText(2.0f, 87.0f, "Press RIGHT ARROW to move car. Press 'R' to reset.");


    if (gameEnded) {
        std::string msg = playerWon ? "YOU PASSED! Press R to play again." : "GAME OVER! You crossed on RED/YELLOW. Press R to retry.";
        glColor3f(0.0f, 0.0f, 0.0f);
        drawText(30.0f, 60.0f, msg);
    }

    glutSwapBuffers();
}


void timerFunc(int val) {
    if (!gameEnded) {
        currentTime++;

        if (state == 0 && currentTime >= redTime) {
            state = 1; currentTime = 0;
        } else if (state == 1 && currentTime >= yellowTime) {
            state = 2; currentTime = 0;
        } else if (state == 2 && currentTime >= greenTime) {
            state = 0; currentTime = 0;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000, timerFunc, 0);
}


void specialKeys(int key, int x, int y) {
    if (gameEnded) return;

    if (key == GLUT_KEY_RIGHT) {
        carX += moveStep;


        if (carX + carW/2.0f > 100.0f) carX = 100.0f - carW/2.0f;


        float carFront = carX + carW/2.0f;
        if (carFront >= stopLineX) {

            if (state == 2) {
                playerWon = true;
            } else {
                playerWon = false;
            }
            gameEnded = true;
        }
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {

        carX = 5.0f;
        gameEnded = false;
        playerWon = false;
        currentTime = 0;
        state = 0;
        glutPostRedisplay();
    } else if (key == 27) {
        exit(0);
    }
}

void initGL() {
    glClearColor(0.6f, 0.85f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

int main(int argc, char** argv) {
    std::cout << "Traffic Light Mini-Game (Keyboard Control)\n";
    std::cout << "Enter Red light duration (seconds): ";
    std::cin >> redTime;
    std::cout << "Enter Yellow light duration (seconds): ";
    std::cin >> yellowTime;
    std::cout << "Enter Green light duration (seconds): ";
    std::cin >> greenTime;


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Traffic Light Game - Press RIGHT to move");

    initGL();
    glutDisplayFunc(display);
    glutTimerFunc(1000, timerFunc, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
