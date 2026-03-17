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

void drawSingleGate(float zCenter, float gateWidth) {
    glColor3f(0.1, 0.1, 0.8);  // Blue color to make gates visible
    
    // Gate positioned on support walls on downstream side
    // Opens upward (gate moves up as gateOpen increases)
    float gateThickness = 0.2f;
    float gateHeight = 2.0f;
    
    float xStart = 4.0f;   // Start exactly at downstream face of dam
    float yOffset = gateOpen * 0.5f;  // vertical offset when opening
    float z0 = zCenter - gateWidth / 2.0f;
    
    drawCuboid(xStart, yOffset, z0, gateThickness, gateHeight, gateWidth);
}

void drawGates() {
    // Draw 2 gates
    // Support wall inner edges are precisely at -1.85 to -0.15, and 0.15 to 1.85
    // Centers are -1.0 and 1.0, widths of exactly 1.7f will fit securely between walls
    drawSingleGate(-1.0f, 1.7f);   // Left gate
    drawSingleGate(1.0f, 1.7f);    // Right gate
}

void drawSupportWalls() {
    glColor3f(0.4, 0.4, 0.4);
    
    // 3 vertical support walls extending outside the dam on downstream side
    float yBottom = 0.0f, yTop = 4.0f;
    float xStart = 4.0f, xEnd = 8.0f;  // extending exactly from the dam's face
    float thickness = 0.3f;
    
    // Wall positions at different z coordinates
    float wallZPositions[] = {-2.0f, 0.0f, 2.0f};
    
    for (int i = 0; i < 3; i++) {
        float z = wallZPositions[i];
        drawCuboid(xStart, yBottom, z - thickness/2.0f, xEnd - xStart, yTop, thickness);
    }
}

void drawOutflowWater() {
    if (gateOpen > 0) {
        glColor3f(0, 0.5, 1);
        
        // Water flowing out from both gates on downstream side
        float outflowHeight = gateOpen / 2.0f + 0.5f;
        float outflowX = 4.0f;  // Starting exactly under gates
        float outflowLength = 6.0f; // flowing stream length
        float outflowDepth = 1.7f;  // match the width of gates perfectly
        
        // Left outflow (z spans -1.85 to -0.15 to precisely match gate inside walls)
        drawCuboid(outflowX, 0, -1.85f, outflowLength, outflowHeight, outflowDepth);
        
        // Right outflow (z spans 0.15 to 1.85 to precisely match gate inside walls)
        drawCuboid(outflowX, 0, 0.15f, outflowLength, outflowHeight, outflowDepth);
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
    drawGates();
    drawSupportWalls();
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

        // Gate control (gate height is 2, max liftable to 2x = 4 units)
        case 'o': gateOpen += 1; break;
        case 'c': gateOpen -= 1; break;

        case 27: exit(0);
    }

    if (waterHeight < 1) waterHeight = 1;
    if (waterHeight > 10) waterHeight = 10;

    if (gateOpen < 0) gateOpen = 0;
    if (gateOpen > 4) gateOpen = 4;  // max 2x gate height

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