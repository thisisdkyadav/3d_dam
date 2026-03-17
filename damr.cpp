#include <GL/glut.h>
#include <cstdlib>

// ---------------------------
// Global controls
// ---------------------------
int angleX = 18;
int angleY = -35;
int camZ = 32;

int waterLevel = 8;   // 2 to 12
int gateOpen = 3;     // 0 to 6

// ---------------------------
// Utility: draw a cuboid
// ---------------------------
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

// ---------------------------
// Ground and channel
// ---------------------------
void drawGround() {
    glColor3f(0.35, 0.65, 0.30);
    drawCuboid(-24, -1, -14, 48, 1, 28);
}

void drawRiverChannel() {
    glColor3f(0.45, 0.35, 0.20);
    drawCuboid(6, -0.8, -4, 16, 0.3, 8);

    glColor3f(0.0, 0.45, 0.95);
    drawCuboid(6, -0.5, -3, 16, 0.2, 6);
}

// ---------------------------
// Terrain walls / hills
// ---------------------------
void drawLeftHill() {
    glColor3f(0.45, 0.30, 0.18);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(-20, 0, -14);
    glVertex3f(-12, 0, -14);
    glVertex3f(-10, 8, -14);
    glVertex3f(-18, 6, -14);

    // Back face
    glVertex3f(-20, 0, 14);
    glVertex3f(-12, 0, 14);
    glVertex3f(-10, 8, 14);
    glVertex3f(-18, 6, 14);

    // Left side
    glVertex3f(-20, 0, -14);
    glVertex3f(-20, 0, 14);
    glVertex3f(-18, 6, 14);
    glVertex3f(-18, 6, -14);

    // Right side
    glVertex3f(-12, 0, -14);
    glVertex3f(-12, 0, 14);
    glVertex3f(-10, 8, 14);
    glVertex3f(-10, 8, -14);

    // Top slope
    glVertex3f(-18, 6, -14);
    glVertex3f(-10, 8, -14);
    glVertex3f(-10, 8, 14);
    glVertex3f(-18, 6, 14);

    glEnd();
}

void drawRightHill() {
    glColor3f(0.45, 0.30, 0.18);
    glBegin(GL_QUADS);

    // Front face
    glVertex3f(10, 0, -14);
    glVertex3f(20, 0, -14);
    glVertex3f(18, 7, -14);
    glVertex3f(12, 5, -14);

    // Back face
    glVertex3f(10, 0, 14);
    glVertex3f(20, 0, 14);
    glVertex3f(18, 7, 14);
    glVertex3f(12, 5, 14);

    // Left side
    glVertex3f(10, 0, -14);
    glVertex3f(10, 0, 14);
    glVertex3f(12, 5, 14);
    glVertex3f(12, 5, -14);

    // Right side
    glVertex3f(20, 0, -14);
    glVertex3f(20, 0, 14);
    glVertex3f(18, 7, 14);
    glVertex3f(18, 7, -14);

    // Top slope
    glVertex3f(12, 5, -14);
    glVertex3f(18, 7, -14);
    glVertex3f(18, 7, 14);
    glVertex3f(12, 5, 14);

    glEnd();
}

// ---------------------------
// Reservoir water
// ---------------------------
void drawReservoirWater() {
    glColor3f(0.0, 0.4, 1.0);
    drawCuboid(-18, 0, -10, 15, waterLevel, 20);
}

// ---------------------------
// Dam body: trapezoidal gravity dam
// Wider at base, narrower at top
// ---------------------------
void drawDamBody() {
    glColor3f(0.60, 0.60, 0.62);

    glBegin(GL_QUADS);

    // Front face (downstream face, slightly sloped)
    glVertex3f(-3, 0, -10);
    glVertex3f( 5, 0, -10);
    glVertex3f( 3, 12, -10);
    glVertex3f(-1, 12, -10);

    // Back face (upstream face)
    glVertex3f(-5, 0, 10);
    glVertex3f( 3, 0, 10);
    glVertex3f( 1, 12, 10);
    glVertex3f(-3, 12, 10);

    // Left face
    glVertex3f(-5, 0, 10);
    glVertex3f(-3, 0, -10);
    glVertex3f(-1, 12, -10);
    glVertex3f(-3, 12, 10);

    // Right face
    glVertex3f(3, 0, 10);
    glVertex3f(5, 0, -10);
    glVertex3f(3, 12, -10);
    glVertex3f(1, 12, 10);

    // Top face
    glVertex3f(-3, 12, 10);
    glVertex3f(1, 12, 10);
    glVertex3f(3, 12, -10);
    glVertex3f(-1, 12, -10);

    // Bottom face
    glVertex3f(-5, 0, 10);
    glVertex3f(3, 0, 10);
    glVertex3f(5, 0, -10);
    glVertex3f(-3, 0, -10);

    glEnd();
}

// ---------------------------
// Spillway opening and gate
// ---------------------------
void drawSpillwayFrame() {
    glColor3f(0.48, 0.48, 0.50);

    // Left pillar
    drawCuboid(-0.8, 0, -2, 1.0, 7, 4);

    // Right pillar
    drawCuboid(1.2, 0, -2, 1.0, 7, 4);

    // Top beam
    drawCuboid(-0.8, 7, -2, 3.0, 1.0, 4);
}

void drawGate() {
    glColor3f(0.20, 0.20, 0.22);

    // Gate rises upward as gateOpen increases
    float gateBottom = gateOpen;
    drawCuboid(0.2, gateBottom, -1.6, 1.0, 4.0, 3.2);
}

// ---------------------------
// Sloping spillway water
// ---------------------------
void drawSpillwayWater() {
    if (gateOpen == 0) return;

    glColor3f(0.0, 0.55, 1.0);

    float startY = gateOpen + 1.5f;
    float thickness = 2.2f + gateOpen * 0.25f;

    glBegin(GL_QUADS);

    // Top surface of flowing water
    glVertex3f(1.2f, startY, -1.2f);
    glVertex3f(5.5f, 1.0f, -1.2f);
    glVertex3f(5.5f, 1.0f,  1.2f);
    glVertex3f(1.2f, startY,  1.2f);

    // Bottom surface
    glVertex3f(1.2f, startY - 0.4f, -1.2f);
    glVertex3f(5.5f, 1.0f - 0.4f, -1.2f);
    glVertex3f(5.5f, 1.0f - 0.4f,  1.2f);
    glVertex3f(1.2f, startY - 0.4f,  1.2f);

    // Left side
    glVertex3f(1.2f, startY - 0.4f, -1.2f);
    glVertex3f(5.5f, 1.0f - 0.4f, -1.2f);
    glVertex3f(5.5f, 1.0f, -1.2f);
    glVertex3f(1.2f, startY, -1.2f);

    // Right side
    glVertex3f(1.2f, startY - 0.4f, 1.2f);
    glVertex3f(5.5f, 1.0f - 0.4f, 1.2f);
    glVertex3f(5.5f, 1.0f, 1.2f);
    glVertex3f(1.2f, startY, 1.2f);

    // Front face
    glVertex3f(1.2f, startY - 0.4f, -1.2f);
    glVertex3f(1.2f, startY - 0.4f,  1.2f);
    glVertex3f(1.2f, startY,  1.2f);
    glVertex3f(1.2f, startY, -1.2f);

    // End face
    glVertex3f(5.5f, 1.0f - 0.4f, -1.2f);
    glVertex3f(5.5f, 1.0f - 0.4f,  1.2f);
    glVertex3f(5.5f, 1.0f,  1.2f);
    glVertex3f(5.5f, 1.0f, -1.2f);

    glEnd();

    // Extra splash / thicker stream near channel
    glColor3f(0.0, 0.48, 0.95);
    drawCuboid(5.5f, 0.2f, -thickness / 2, 2.5f, 0.6f, thickness);
}

// ---------------------------
// Dam top road / railing strip
// ---------------------------
void drawTopRoad() {
    glColor3f(0.25, 0.25, 0.25);
    drawCuboid(-2.7f, 12.1f, -10.0f, 3.6f, 0.25f, 20.0f);

    glColor3f(0.8, 0.8, 0.8);
    drawCuboid(-2.75f, 12.35f, -10.0f, 0.1f, 0.35f, 20.0f);
    drawCuboid( 0.85f, 12.35f, -10.0f, 0.1f, 0.35f, 20.0f);
}

// ---------------------------
// Display
// ---------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
        0, 10, camZ,
        0, 4, 0,
        0, 1, 0
    );

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawGround();
    drawLeftHill();
    drawRightHill();
    drawReservoirWater();
    drawDamBody();
    drawSpillwayFrame();
    drawGate();
    drawSpillwayWater();
    drawRiverChannel();
    drawTopRoad();

    glutSwapBuffers();
}

// ---------------------------
// Keyboard controls
// ---------------------------
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w': angleX -= 5; break;
        case 's': angleX += 5; break;
        case 'a': angleY -= 5; break;
        case 'd': angleY += 5; break;

        case 'i': waterLevel += 1; break;
        case 'k': waterLevel -= 1; break;

        case 'o': gateOpen += 1; break;
        case 'c': gateOpen -= 1; break;

        case 27: exit(0);
    }

    if (waterLevel < 2) waterLevel = 2;
    if (waterLevel > 12) waterLevel = 12;

    if (gateOpen < 0) gateOpen = 0;
    if (gateOpen > 6) gateOpen = 6;

    glutPostRedisplay();
}

// ---------------------------
// Mouse zoom
// ---------------------------
void mouse(int button, int state, int, int) {
    if (state == GLUT_DOWN) {
        if (button == 3) camZ -= 1;
        if (button == 4) camZ += 1;

        if (camZ < 18) camZ = 18;
        if (camZ > 50) camZ = 50;

        glutPostRedisplay();
    }
}

// ---------------------------
// Init
// ---------------------------
void init() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.70f, 0.88f, 1.0f, 1.0f); // sky color

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

    glMatrixMode(GL_MODELVIEW);
}

// ---------------------------
// Main
// ---------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 700);
    glutCreateWindow("3D Dam Reservoir Simulator");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}