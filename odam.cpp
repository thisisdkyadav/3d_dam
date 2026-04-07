#include <GL/glut.h>
#include <cstdlib>

int angleX = 20;
int angleY = -30;

int camZ = 30;

int waterHeight = 5;

int gateOpen = 2;

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
    glColor3f(0.45f, 0.35f, 0.25f); // Riverbed dirt/brown color
    drawCuboid(-24, -1, -33, 42, 1, 66);
}

void drawDam() {
    glColor3f(0.62f, 0.62f, 0.60f);

    float xL0 = 0.5f, xL1 = 1.5f;
    float xR = 4.0f;
    float y0 = 0.0f, y1 = 8.0f;
    
    // Inclined sides for the dam
    float z0_y0 = -9.0f, z1_y0 = 9.0f;
    float z0_y1 = -18.0f, z1_y1 = 18.0f;

    glBegin(GL_QUADS);
    // Top
    glVertex3f(xL0, y1, z1_y1); glVertex3f(xR, y1, z1_y1); glVertex3f(xR, y1, z0_y1); glVertex3f(xL0, y1, z0_y1);
    // Bottom
    glVertex3f(xL0, y0, z0_y0); glVertex3f(xR, y0, z0_y0); glVertex3f(xR, y0, z1_y0); glVertex3f(xL0, y0, z1_y0);
    // Front facing water
    glVertex3f(xL0, y0, z1_y0); glVertex3f(xL1, y1, z1_y1); glVertex3f(xL1, y1, z0_y1); glVertex3f(xL0, y0, z0_y0);
    // Back block (spillway slope)
    glVertex3f(xR, y0, z0_y0); glVertex3f(xR, y1, z0_y1); glVertex3f(xR, y1, z1_y1); glVertex3f(xR, y0, z1_y0);
    
    // Left side (inclined)
    glVertex3f(xL0, y0, z0_y0); glVertex3f(xL1, y1, z0_y1); glVertex3f(xR, y1, z0_y1); glVertex3f(xR, y0, z0_y0);
    // Right side (inclined)
    glVertex3f(xR, y0, z1_y0); glVertex3f(xR, y1, z1_y1); glVertex3f(xL1, y1, z1_y1); glVertex3f(xL0, y0, z1_y0);
    glEnd();
}

void drawReservoirWater() {
    glColor3f(0.10f, 0.34f, 0.66f);

    float xL = -24.0f;
    float xR0 = 0.5f;
    float xR1 = 0.5f + waterHeight / 8.0f;
    float y0 = 0.0f, y1 = (float)waterHeight;
    
    float z0_y0 = -9.0f, z1_y0 = 9.0f;
    float z0_y1 = -9.0f - (waterHeight * 9.0f / 8.0f);
    float z1_y1 = 9.0f + (waterHeight * 9.0f / 8.0f);

    glBegin(GL_QUADS);
    // Top
    glVertex3f(xL, y1, z1_y1); glVertex3f(xR1, y1, z1_y1); glVertex3f(xR1, y1, z0_y1); glVertex3f(xL, y1, z0_y1);
    // Bottom
    glVertex3f(xL, y0, z0_y0); glVertex3f(xL, y0, z1_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR0, y0, z0_y0);
    // Front face against dam
    glVertex3f(xR0, y0, z0_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR1, y1, z1_y1); glVertex3f(xR1, y1, z0_y1);
    // Back face (far water edge)
    glVertex3f(xL, y0, z1_y0); glVertex3f(xL, y0, z0_y0); glVertex3f(xL, y1, z0_y1); glVertex3f(xL, y1, z1_y1);
    
    // Left edge (inclined)
    glVertex3f(xL, y1, z0_y1); glVertex3f(xR1, y1, z0_y1); glVertex3f(xR0, y0, z0_y0); glVertex3f(xL, y0, z0_y0);
    // Right edge (inclined)
    glVertex3f(xL, y0, z1_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR1, y1, z1_y1); glVertex3f(xL, y1, z1_y1);
    glEnd();
}

void drawSingleGate(float zCenter, float gateWidth) {
    glColor3f(0.26f, 0.28f, 0.30f);
    
    float gateThickness = 0.2f;
    float gateHeight = 2.0f;
    
    float xStart = 4.0f;
    float yOffset = gateOpen * 0.5f;
    float z0 = zCenter - gateWidth / 2.0f;
    
    drawCuboid(xStart, yOffset, z0, gateThickness, gateHeight, gateWidth);
}

void drawGates() {
    drawSingleGate(-6.0f, 2.7f);
    drawSingleGate(-3.0f, 2.7f);
    drawSingleGate(0.0f, 2.7f);
    drawSingleGate(3.0f, 2.7f);
    drawSingleGate(6.0f, 2.7f);
}

void drawSupportWalls() {
    glColor3f(0.52f, 0.52f, 0.50f);
    
    float yBottom = 0.0f, yTop = 4.0f;
    float xStart = 4.0f, xEnd = 6.0f;
    float thickness = 0.3f;
    
    float wallZPositions[] = {-7.5f, -4.5f, -1.5f, 1.5f, 4.5f, 7.5f};
    
    for (int i = 0; i < 6; i++) {
        float z = wallZPositions[i];
        drawCuboid(xStart, yBottom, z - thickness/2.0f, xEnd - xStart, yTop, thickness);
    }
}

void drawOutflowWater() {
    if (gateOpen > 0) {
        glColor3f(0.34f, 0.70f, 0.92f);
        
        float y0 = 0.0f;
        float outflowHeight = gateOpen / 2.0f + 0.5f;
        float x0 = 4.0f;
        float x1 = 10.0f;
        
        float z0_y0 = -9.0f, z1_y0 = 9.0f;
        
        // Outflow water slope based on hills at its short height
        float z0_y1 = -9.0f - (outflowHeight * 9.0f / 8.0f);
        float z1_y1 = 9.0f + (outflowHeight * 9.0f / 8.0f);

        glBegin(GL_QUADS);
        // Top
        glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x0, outflowHeight, z0_y1);
        // Bottom
        glVertex3f(x0, y0, z0_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, y0, z1_y0); glVertex3f(x0, y0, z1_y0);
        // Front face (against the gates/dam base)
        glVertex3f(x0, y0, z0_y0); glVertex3f(x0, y0, z1_y0); glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x0, outflowHeight, z0_y1);
        // Back edge (flowing out)
        glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x1, outflowHeight, z1_y1);
        
        // Left side (inclined to meet hill)
        glVertex3f(x0, y0, z0_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x0, outflowHeight, z0_y1);
        // Right side (inclined to meet hill)
        glVertex3f(x1, y0, z1_y0); glVertex3f(x0, y0, z1_y0); glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z1_y1);
        glEnd();
    }
}

void drawHills() {
    glColor3f(0.38f, 0.56f, 0.30f); // Hill color (green)

    float x0 = -24.0f, x1 = 18.0f;
    float y0 = 0.0f, y1 = 8.0f;
    
    // Keep hills very slightly outside dam/water planes to avoid z-fighting.
    float sideInset = 0.15f;
    float z0_y0 = -9.0f - sideInset, z1_y0 = 9.0f + sideInset;
    float z0_y1 = -18.0f - sideInset, z1_y1 = 18.0f + sideInset;
    float zOuterL = -33.0f;
    float zOuterR = 33.0f;

    glBegin(GL_QUADS);
    // Left hill slope (touches dam/water at z0_yX)
    glVertex3f(x0, y1, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x1, y1, z0_y1); glVertex3f(x0, y1, z0_y1); // Top flat part of hill
    glVertex3f(x0, y1, z0_y1); glVertex3f(x1, y1, z0_y1); glVertex3f(x1, y0, z0_y0); glVertex3f(x0, y0, z0_y0); // Sloping down to water
    
    // Right hill slope (touches dam/water at z1_yX)
    glVertex3f(x0, y0, z1_y0); glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y1, z1_y1); glVertex3f(x0, y1, z1_y1); // Sloping UP from water
    glVertex3f(x0, y1, z1_y1); glVertex3f(x1, y1, z1_y1); glVertex3f(x1, y1, zOuterR); glVertex3f(x0, y1, zOuterR); // Top flat part of hill
    
    // Left hill outer edge (back face)
    glVertex3f(x0, y0, zOuterL); glVertex3f(x1, y0, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x0, y1, zOuterL);
    // Right hill outer edge (back face)
    glVertex3f(x0, y0, zOuterR); glVertex3f(x1, y0, zOuterR); glVertex3f(x1, y1, zOuterR); glVertex3f(x0, y1, zOuterR);
    
    // Left hill end caps (Front/Back)
    glVertex3f(x0, y0, z0_y0); glVertex3f(x0, y1, z0_y1); glVertex3f(x0, y1, zOuterL); glVertex3f(x0, y0, zOuterL);
    glVertex3f(x1, y0, z0_y0); glVertex3f(x1, y0, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x1, y1, z0_y1);

    // Right hill end caps (Front/Back)
    glVertex3f(x0, y0, z1_y0); glVertex3f(x0, y0, zOuterR); glVertex3f(x0, y1, zOuterR); glVertex3f(x0, y1, z1_y1);
    glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y1, z1_y1); glVertex3f(x1, y1, zOuterR); glVertex3f(x1, y0, zOuterR);
    glEnd();
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
    drawHills();
    drawReservoirWater();
    drawDam();
    drawGates();
    drawSupportWalls();
    drawOutflowWater();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': angleX -= 5; break;
        case 's': angleX += 5; break;
        case 'a': angleY -= 5; break;
        case 'd': angleY += 5; break;

        case 'i': waterHeight += 1; break;
        case 'k': waterHeight -= 1; break;

        case 'o': gateOpen += 1; break;
        case 'c': gateOpen -= 1; break;

        case 27: exit(0);
    }

    if (waterHeight < 1) waterHeight = 1;
    if (waterHeight > 10) waterHeight = 10;

    if (gateOpen < 0) gateOpen = 0;
    if (gateOpen > 4) gateOpen = 4;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == 3) camZ -= 1;
        if (button == 4) camZ += 1;

        if (camZ < 10) camZ = 10;
        if (camZ > 80) camZ = 80;

        glutPostRedisplay();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glClearColor(0.70f, 0.85f, 0.95f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.6f, 1, 200);

    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / (float)h, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1280, 800);
    glutCreateWindow("3D Dam Reservoir Simulator");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}