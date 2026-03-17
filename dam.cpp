#include <GL/glut.h>
#include <cstdlib>

int waterLevel = 140;   // current reservoir level
int gateOpen = 20;      // gate opening height
int flow = 0;           // flow length animation
int inflow = 1;         // water entering reservoir

void drawText(float x, float y, const char* str) {
    glRasterPos2f(x, y);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
        str++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Sky
    glColor3f(0.8, 0.9, 1);
    glBegin(GL_QUADS);
        glVertex2i(0, 300);
        glVertex2i(800, 300);
        glVertex2i(800, 600);
        glVertex2i(0, 600);
    glEnd();

    // Ground
    glColor3f(0.6, 0.8, 0.4);
    glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(800, 0);
        glVertex2i(800, 300);
        glVertex2i(0, 300);
    glEnd();

    // Reservoir water
    glColor3f(0, 0.4, 1);
    glBegin(GL_QUADS);
        glVertex2i(50, 100);
        glVertex2i(300, 100);
        glVertex2i(300, waterLevel);
        glVertex2i(50, waterLevel);
    glEnd();

    // Dam wall
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2i(300, 100);
        glVertex2i(380, 100);
        glVertex2i(360, 260);
        glVertex2i(300, 260);
    glEnd();

    // Gate opening
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_QUADS);
        glVertex2i(330, 100);
        glVertex2i(350, 100);
        glVertex2i(350, 100 + gateOpen);
        glVertex2i(330, 100 + gateOpen);
    glEnd();

    // Flowing water from gate
    if (gateOpen > 0) {
        glColor3f(0, 0.5, 1);
        glBegin(GL_QUADS);
            glVertex2i(350, 100);
            glVertex2i(350 + flow, 100);
            glVertex2i(350 + flow, 100 + gateOpen / 2);
            glVertex2i(350, 100 + gateOpen / 2);
        glEnd();
    }

    // Downstream river
    glColor3f(0, 0.4, 1);
    glBegin(GL_QUADS);
        glVertex2i(380, 80);
        glVertex2i(780, 80);
        glVertex2i(780, 120);
        glVertex2i(380, 120);
    glEnd();

    // Labels
    glColor3f(0, 0, 0);
    drawText(20, 560, "Dam Reservoir Simulator");
    drawText(20, 530, "W/S : Increase/Decrease Water Level");
    drawText(20, 500, "O/C : Open/Close Gate");
    drawText(20, 470, "ESC : Exit");

    glutSwapBuffers();
}

void update(int value) {
    // basic water storage logic
    int outflow = gateOpen / 10;
    waterLevel = waterLevel + inflow - outflow;

    if (waterLevel < 100) waterLevel = 100;
    if (waterLevel > 250) waterLevel = 250;

    if (gateOpen > 0) {
        flow += 5;
        if (flow > 200) flow = 50;
    } else {
        flow = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(100, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': waterLevel += 5; break;
        case 's': waterLevel -= 5; break;
        case 'o': gateOpen += 5; break;
        case 'c': gateOpen -= 5; break;
        case 27: exit(0);
    }

    if (gateOpen < 0) gateOpen = 0;
    if (gateOpen > 80) gateOpen = 80;
    if (waterLevel < 100) waterLevel = 100;
    if (waterLevel > 250) waterLevel = 250;

    glutPostRedisplay();
}

void init() {
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Dam Reservoir Simulator");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(100, update, 0);

    glutMainLoop();
    return 0;
}