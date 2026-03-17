#include <GL/glut.h>
#include <cstdlib>

// Scene rotation
int angleX = 20;
int angleY = -30;

// Camera zoom
int camZ = 25;

// Reservoir water height
int waterHeight = 5;   // min 1, max 10

// Gate opening
int gateOpen = 2;      // min 0, max 6

void drawCuboid(float x, float y, float z, float w, float h, float d) {
    glBegin(GL_QUADS);

    // Front
    glVertex3f(x,     y,     z + d);
    glVertex3f(x + w, y,     z + d);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x,     y + h, z + d);

    // Back
    glVertex3f(x,     y,     z);
    glVertex3f(x,     y + h, z);
    glVertex3f(x + w, y + h, z);
    glVertex3f(x + w, y,     z);

    // Left
    glVertex3f(x, y,     z);
    glVertex3f(x, y,     z + d);
    glVertex3f(x, y + h, z + d);
    glVertex3f(x, y + h, z);

    // Right
    glVertex3f(x + w, y,     z);
    glVertex3f(x + w, y + h, z);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x + w, y,     z + d);

    // Top
    glVertex3f(x,     y + h, z);
    glVertex3f(x,     y + h, z + d);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x + w, y + h, z);

    // Bottom
    glVertex3f(x,     y, z);
    glVertex3f(x + w, y, z);
    glVertex3f(x + w, y, z + d);
    glVertex3f(x,     y, z + d);

    glEnd();
}

void drawGround() {
    glColor3f(0.3, 0.8, 0.3);
    drawCuboid(-15, -1, -8, 30, 1, 16);
}

void drawDam() {
    glColor3f(0.5, 0.5, 0.5);

    float xL0 = 0.5f, xL1 = 1.5f; // upstream face slopes inward with height
    float xR = 4.0f;
    float y0 = 0.0f, y1 = 8.0f;
    float z0 = -4.0f, z1 = 4.0f;

    glBegin(GL_QUADS);
    // Front
    glVertex3f(xL0, y0, z1); glVertex3f(xR, y0, z1); glVertex3f(xR, y1, z1); glVertex3f(xL1, y1, z1);
    // Back
    glVertex3f(xL0, y0, z0); glVertex3f(xL1, y1, z0); glVertex3f(xR, y1, z0); glVertex3f(xR, y0, z0);
    // Upstream inclined face (touches reservoir)
    glVertex3f(xL0, y0, z0); glVertex3f(xL0, y0, z1); glVertex3f(xL1, y1, z1); glVertex3f(xL1, y1, z0);
    // Downstream face
    glVertex3f(xR, y0, z0); glVertex3f(xR, y1, z0); glVertex3f(xR, y1, z1); glVertex3f(xR, y0, z1);
    // Top
    glVertex3f(xL1, y1, z0); glVertex3f(xL1, y1, z1); glVertex3f(xR, y1, z1); glVertex3f(xR, y1, z0);
    // Bottom
    glVertex3f(xL0, y0, z0); glVertex3f(xR, y0, z0); glVertex3f(xR, y0, z1); glVertex3f(xL0, y0, z1);
    glEnd();
}

void drawReservoirWater() {
    glColor3f(0, 0.4, 1);

    float xL = -10.0f;
    float xR0 = 0.5f;
    float xR1 = 0.5f + waterHeight / 8.0f; // matches dam slope: rise 1 per 8 units height
    float y0 = 0.0f, y1 = (float)waterHeight;
    float z0 = -4.0f, z1 = 4.0f;

    glBegin(GL_QUADS);
    // Front
    glVertex3f(xL, y0, z1); glVertex3f(xR0, y0, z1); glVertex3f(xR1, y1, z1); glVertex3f(xL, y1, z1);
    // Back
    glVertex3f(xL, y0, z0); glVertex3f(xL, y1, z0); glVertex3f(xR1, y1, z0); glVertex3f(xR0, y0, z0);
    // Left face
    glVertex3f(xL, y0, z0); glVertex3f(xL, y0, z1); glVertex3f(xL, y1, z1); glVertex3f(xL, y1, z0);
    // Right inclined face (touches dam)
    glVertex3f(xR0, y0, z0); glVertex3f(xR0, y0, z1); glVertex3f(xR1, y1, z1); glVertex3f(xR1, y1, z0);
    // Top surface (tilted near dam)
    glVertex3f(xL, y1, z0); glVertex3f(xL, y1, z1); glVertex3f(xR1, y1, z1); glVertex3f(xR1, y1, z0);
    // Bottom
    glVertex3f(xL, y0, z0); glVertex3f(xR0, y0, z0); glVertex3f(xR0, y0, z1); glVertex3f(xL, y0, z1);
    glEnd();
}

void drawGate() {
    glColor3f(0.2, 0.2, 0.2);
    drawCuboid(1.8, gateOpen, -1.2, 0.3, 2, 2.4);
}

void drawOutflowWater() {
    if (gateOpen > 0) {
        glColor3f(0, 0.5, 1);
        drawCuboid(4, 0, -1, 6, gateOpen / 2 + 1, 2);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        0, 8, camZ,
        0, 3, 0,
        0, 1, 0
    );

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawGround();
    drawReservoirWater();
    drawDam();
    drawGate();
    drawOutflowWater();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Rotate scene
        case 'w': angleX -= 5; break;
        case 's': angleX += 5; break;
        case 'a': angleY -= 5; break;
        case 'd': angleY += 5; break;

        // Water level control
        case 'i': waterHeight += 1; break;
        case 'k': waterHeight -= 1; break;

        // Gate control
        case 'o': gateOpen += 1; break;
        case 'c': gateOpen -= 1; break;

        case 27: exit(0);
    }

    if (waterHeight < 1) waterHeight = 1;
    if (waterHeight > 10) waterHeight = 10;

    if (gateOpen < 0) gateOpen = 0;
    if (gateOpen > 6) gateOpen = 6;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == 3) camZ -= 1;   // zoom in
        if (button == 4) camZ += 1;   // zoom out

        if (camZ < 10) camZ = 10;
        if (camZ > 40) camZ = 40;

        glutPostRedisplay();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("3D Dam Reservoir Simulator");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}