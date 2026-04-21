#include <GL/glut.h>
#include <cstdlib>
#include <cmath>

int angleX = 20;
int angleY = -30;

int camZ = 30;

int waterHeight = 5;

int gateOpen = 2;

GLuint groundTexture = 0;
GLuint hillTexture = 0;
GLuint damTexture = 0;

void setNormalFromTriangle(
    float ax, float ay, float az,
    float bx, float by, float bz,
    float cx, float cy, float cz
) {
    float ux = bx - ax;
    float uy = by - ay;
    float uz = bz - az;
    float vx = cx - ax;
    float vy = cy - ay;
    float vz = cz - az;

    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;

    float len = sqrtf(nx * nx + ny * ny + nz * nz);
    if (len < 1e-6f) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        return;
    }

    glNormal3f(nx / len, ny / len, nz / len);
}

void beginTexturedDraw(GLuint textureId, float sScale, float tScale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    GLfloat sPlane[] = {sScale, 0.0f, 0.0f, 0.0f};
    GLfloat tPlane[] = {0.0f, 0.0f, tScale, 0.0f};
    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
}

void endTexturedDraw() {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void createNoiseTexture(GLuint& textureId, unsigned char baseR, unsigned char baseG, unsigned char baseB, int variation) {
    const int size = 64;
    unsigned char data[size][size][3];

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int noise = rand() % (2 * variation + 1) - variation;
            int checker = ((x / 8 + y / 8) % 2 == 0) ? 8 : -8;

            int r = (int)baseR + noise + checker;
            int g = (int)baseG + noise + checker;
            int b = (int)baseB + noise + checker;

            if (r < 0) r = 0; if (r > 255) r = 255;
            if (g < 0) g = 0; if (g > 255) g = 255;
            if (b < 0) b = 0; if (b > 255) b = 255;

            data[y][x][0] = (unsigned char)r;
            data[y][x][1] = (unsigned char)g;
            data[y][x][2] = (unsigned char)b;
        }
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, size, size, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void initTextures() {
    srand(7);
    createNoiseTexture(groundTexture, 120, 92, 62, 18);
    createNoiseTexture(hillTexture, 88, 130, 70, 20);
    createNoiseTexture(damTexture, 150, 150, 145, 12);
}

void setSpecular(float intensity, float shininess) {
    GLfloat spec[] = {intensity, intensity, intensity, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void drawCuboid(float x, float y, float z, float w, float h, float d) {
    glBegin(GL_QUADS);

    
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x,     y,     z + d);
    glVertex3f(x + w, y,     z + d);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x,     y + h, z + d);

    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x,     y,     z);
    glVertex3f(x,     y + h, z);
    glVertex3f(x + w, y + h, z);
    glVertex3f(x + w, y,     z);

    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x, y,     z);
    glVertex3f(x, y,     z + d);
    glVertex3f(x, y + h, z + d);
    glVertex3f(x, y + h, z);

    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x + w, y,     z);
    glVertex3f(x + w, y + h, z);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x + w, y,     z + d);

    
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x,     y + h, z);
    glVertex3f(x,     y + h, z + d);
    glVertex3f(x + w, y + h, z + d);
    glVertex3f(x + w, y + h, z);

    
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x,     y, z);
    glVertex3f(x + w, y, z);
    glVertex3f(x + w, y, z + d);
    glVertex3f(x,     y, z + d);

    glEnd();
}

void drawGround() {
    glColor3f(0.45f, 0.35f, 0.25f); 
    setSpecular(0.08f, 8.0f);
    beginTexturedDraw(groundTexture, 0.12f, 0.12f);
    drawCuboid(-40, -1, -50, 74, 1, 100);
    endTexturedDraw();
}

void drawDam() {
    glColor3f(0.62f, 0.62f, 0.60f);
    setSpecular(0.30f, 32.0f);
    beginTexturedDraw(damTexture, 0.25f, 0.12f);

    float xL0 = 0.5f, xL1 = 1.5f;
    float xR = 4.0f;
    float y0 = 0.0f, y1 = 8.0f;
    
    
    float z0_y0 = -9.0f, z1_y0 = 9.0f;
    float z0_y1 = -18.0f, z1_y1 = 18.0f;

    glBegin(GL_QUADS);
    
    setNormalFromTriangle(xL0, y1, z1_y1, xR, y1, z1_y1, xR, y1, z0_y1);
    glVertex3f(xL0, y1, z1_y1); glVertex3f(xR, y1, z1_y1); glVertex3f(xR, y1, z0_y1); glVertex3f(xL0, y1, z0_y1);
    
    setNormalFromTriangle(xL0, y0, z0_y0, xR, y0, z0_y0, xR, y0, z1_y0);
    glVertex3f(xL0, y0, z0_y0); glVertex3f(xR, y0, z0_y0); glVertex3f(xR, y0, z1_y0); glVertex3f(xL0, y0, z1_y0);
    
    setNormalFromTriangle(xL0, y0, z1_y0, xL1, y1, z1_y1, xL1, y1, z0_y1);
    glVertex3f(xL0, y0, z1_y0); glVertex3f(xL1, y1, z1_y1); glVertex3f(xL1, y1, z0_y1); glVertex3f(xL0, y0, z0_y0);
    
    setNormalFromTriangle(xR, y0, z0_y0, xR, y1, z0_y1, xR, y1, z1_y1);
    glVertex3f(xR, y0, z0_y0); glVertex3f(xR, y1, z0_y1); glVertex3f(xR, y1, z1_y1); glVertex3f(xR, y0, z1_y0);
    
    
    setNormalFromTriangle(xL0, y0, z0_y0, xL1, y1, z0_y1, xR, y1, z0_y1);
    glVertex3f(xL0, y0, z0_y0); glVertex3f(xL1, y1, z0_y1); glVertex3f(xR, y1, z0_y1); glVertex3f(xR, y0, z0_y0);
    
    setNormalFromTriangle(xR, y0, z1_y0, xR, y1, z1_y1, xL1, y1, z1_y1);
    glVertex3f(xR, y0, z1_y0); glVertex3f(xR, y1, z1_y1); glVertex3f(xL1, y1, z1_y1); glVertex3f(xL0, y0, z1_y0);
    glEnd();

    endTexturedDraw();
}

void drawDamTopDetails() {
    glDisable(GL_TEXTURE_2D);

    
    const float xL = 0.5f;
    const float xR = 4.0f;
    const float yTop = 8.0f;
    const float zMin = -18.0f;
    const float zMax = 18.0f;

    
    glColor3f(0.42f, 0.43f, 0.45f);
    setSpecular(0.28f, 28.0f);
    drawCuboid(
        xL,
        yTop + 0.02f,
        zMin,
        (xR - xL),
        0.18f,
        (zMax - zMin)
    );

    
    glColor3f(0.76f, 0.76f, 0.74f);
    setSpecular(0.36f, 38.0f);
    const float railZ0 = zMin;
    const float railLen = (zMax - zMin);
    const float postSize = 0.07f;
    drawCuboid(xL + 0.18f, yTop + 0.60f, railZ0, postSize, 0.07f, railLen);
    drawCuboid(xR - 0.25f, yTop + 0.60f, railZ0, postSize, 0.07f, railLen);

    for (int i = 0; i < 12; i++) {
        float z = zMin + i * ((zMax - zMin - postSize) / 11.0f);
        drawCuboid(xL + 0.18f, yTop + 0.24f, z, postSize, 0.36f, postSize);
        drawCuboid(xR - 0.25f, yTop + 0.24f, z, postSize, 0.36f, postSize);
    }

    
    glColor3f(0.90f, 0.79f, 0.22f);
    setSpecular(0.08f, 6.0f);
    float centerX = (xL + xR) * 0.5f;
    for (int i = 0; i < 8; i++) {
        float z = zMin + 2.6f + i * 4.0f;
        drawCuboid(centerX - 0.06f, yTop + 0.23f, z, 0.12f, 0.02f, 1.7f);
    }
}

void drawReservoirWater() {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.10f, 0.34f, 0.66f, 0.80f);
    setSpecular(0.65f, 96.0f);

    float xL = -40.0f;
    float xR0 = 0.5f;
    float xR1 = 0.5f + waterHeight / 8.0f;
    float y0 = 0.0f, y1 = (float)waterHeight;
    
    float z0_y0 = -9.0f, z1_y0 = 9.0f;
    float z0_y1 = -9.0f - (waterHeight * 9.0f / 8.0f);
    float z1_y1 = 9.0f + (waterHeight * 9.0f / 8.0f);

    glBegin(GL_QUADS);
    
    setNormalFromTriangle(xL, y1, z1_y1, xR1, y1, z1_y1, xR1, y1, z0_y1);
    glVertex3f(xL, y1, z1_y1); glVertex3f(xR1, y1, z1_y1); glVertex3f(xR1, y1, z0_y1); glVertex3f(xL, y1, z0_y1);
    
    setNormalFromTriangle(xL, y0, z0_y0, xL, y0, z1_y0, xR0, y0, z1_y0);
    glVertex3f(xL, y0, z0_y0); glVertex3f(xL, y0, z1_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR0, y0, z0_y0);
    
    setNormalFromTriangle(xR0, y0, z0_y0, xR0, y0, z1_y0, xR1, y1, z1_y1);
    glVertex3f(xR0, y0, z0_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR1, y1, z1_y1); glVertex3f(xR1, y1, z0_y1);
    
    setNormalFromTriangle(xL, y0, z1_y0, xL, y0, z0_y0, xL, y1, z0_y1);
    glVertex3f(xL, y0, z1_y0); glVertex3f(xL, y0, z0_y0); glVertex3f(xL, y1, z0_y1); glVertex3f(xL, y1, z1_y1);
    
    
    setNormalFromTriangle(xL, y1, z0_y1, xR1, y1, z0_y1, xR0, y0, z0_y0);
    glVertex3f(xL, y1, z0_y1); glVertex3f(xR1, y1, z0_y1); glVertex3f(xR0, y0, z0_y0); glVertex3f(xL, y0, z0_y0);
    
    setNormalFromTriangle(xL, y0, z1_y0, xR0, y0, z1_y0, xR1, y1, z1_y1);
    glVertex3f(xL, y0, z1_y0); glVertex3f(xR0, y0, z1_y0); glVertex3f(xR1, y1, z1_y1); glVertex3f(xL, y1, z1_y1);
    glEnd();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void drawSingleGate(float zCenter, float gateWidth) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.26f, 0.28f, 0.30f);
    setSpecular(0.45f, 40.0f);
    
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
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.52f, 0.52f, 0.50f);
    setSpecular(0.26f, 22.0f);
    
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
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glColor4f(0.34f, 0.70f, 0.92f, 0.78f);
        setSpecular(0.55f, 72.0f);
        
        float y0 = 0.0f;
        float outflowHeight = gateOpen / 2.0f + 0.5f;
        float x0 = 4.0f;
        float x1 = 34.0f;
        
        float z0_y0 = -9.0f, z1_y0 = 9.0f;
        
        
        float z0_y1 = -9.0f - (outflowHeight * 9.0f / 8.0f);
        float z1_y1 = 9.0f + (outflowHeight * 9.0f / 8.0f);

        glBegin(GL_QUADS);
        
        setNormalFromTriangle(x0, outflowHeight, z1_y1, x1, outflowHeight, z1_y1, x1, outflowHeight, z0_y1);
        glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x0, outflowHeight, z0_y1);
        
        setNormalFromTriangle(x0, y0, z0_y0, x1, y0, z0_y0, x1, y0, z1_y0);
        glVertex3f(x0, y0, z0_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, y0, z1_y0); glVertex3f(x0, y0, z1_y0);
        
        setNormalFromTriangle(x0, y0, z0_y0, x0, y0, z1_y0, x0, outflowHeight, z1_y1);
        glVertex3f(x0, y0, z0_y0); glVertex3f(x0, y0, z1_y0); glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x0, outflowHeight, z0_y1);
        
        setNormalFromTriangle(x1, y0, z1_y0, x1, y0, z0_y0, x1, outflowHeight, z0_y1);
        glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x1, outflowHeight, z1_y1);
        
        
        setNormalFromTriangle(x0, y0, z0_y0, x1, y0, z0_y0, x1, outflowHeight, z0_y1);
        glVertex3f(x0, y0, z0_y0); glVertex3f(x1, y0, z0_y0); glVertex3f(x1, outflowHeight, z0_y1); glVertex3f(x0, outflowHeight, z0_y1);
        
        setNormalFromTriangle(x1, y0, z1_y0, x0, y0, z1_y0, x0, outflowHeight, z1_y1);
        glVertex3f(x1, y0, z1_y0); glVertex3f(x0, y0, z1_y0); glVertex3f(x0, outflowHeight, z1_y1); glVertex3f(x1, outflowHeight, z1_y1);
        glEnd();

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

void drawHills() {
    glColor3f(0.38f, 0.56f, 0.30f); 
    setSpecular(0.12f, 12.0f);
    beginTexturedDraw(hillTexture, 0.10f, 0.10f);

    float x0 = -40.0f, x1 = 34.0f;
    float y0 = 0.0f, y1 = 8.0f;
    
    
    float sideInset = 0.15f;
    float z0_y0 = -9.0f - sideInset, z1_y0 = 9.0f + sideInset;
    float z0_y1 = -18.0f - sideInset, z1_y1 = 18.0f + sideInset;
    float zOuterL = -50.0f;
    float zOuterR = 50.0f;

    glBegin(GL_QUADS);
    
    setNormalFromTriangle(x0, y1, zOuterL, x1, y1, zOuterL, x1, y1, z0_y1);
    glVertex3f(x0, y1, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x1, y1, z0_y1); glVertex3f(x0, y1, z0_y1); 
    setNormalFromTriangle(x0, y1, z0_y1, x1, y1, z0_y1, x1, y0, z0_y0);
    glVertex3f(x0, y1, z0_y1); glVertex3f(x1, y1, z0_y1); glVertex3f(x1, y0, z0_y0); glVertex3f(x0, y0, z0_y0); 
    
    
    setNormalFromTriangle(x0, y0, z1_y0, x1, y0, z1_y0, x1, y1, z1_y1);
    glVertex3f(x0, y0, z1_y0); glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y1, z1_y1); glVertex3f(x0, y1, z1_y1); 
    setNormalFromTriangle(x0, y1, z1_y1, x1, y1, z1_y1, x1, y1, zOuterR);
    glVertex3f(x0, y1, z1_y1); glVertex3f(x1, y1, z1_y1); glVertex3f(x1, y1, zOuterR); glVertex3f(x0, y1, zOuterR); 
    
    
    setNormalFromTriangle(x0, y0, zOuterL, x1, y0, zOuterL, x1, y1, zOuterL);
    glVertex3f(x0, y0, zOuterL); glVertex3f(x1, y0, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x0, y1, zOuterL);
    
    setNormalFromTriangle(x0, y0, zOuterR, x1, y0, zOuterR, x1, y1, zOuterR);
    glVertex3f(x0, y0, zOuterR); glVertex3f(x1, y0, zOuterR); glVertex3f(x1, y1, zOuterR); glVertex3f(x0, y1, zOuterR);
    
    
    setNormalFromTriangle(x0, y0, z0_y0, x0, y1, z0_y1, x0, y1, zOuterL);
    glVertex3f(x0, y0, z0_y0); glVertex3f(x0, y1, z0_y1); glVertex3f(x0, y1, zOuterL); glVertex3f(x0, y0, zOuterL);
    setNormalFromTriangle(x1, y0, z0_y0, x1, y0, zOuterL, x1, y1, zOuterL);
    glVertex3f(x1, y0, z0_y0); glVertex3f(x1, y0, zOuterL); glVertex3f(x1, y1, zOuterL); glVertex3f(x1, y1, z0_y1);

    
    setNormalFromTriangle(x0, y0, z1_y0, x0, y0, zOuterR, x0, y1, zOuterR);
    glVertex3f(x0, y0, z1_y0); glVertex3f(x0, y0, zOuterR); glVertex3f(x0, y1, zOuterR); glVertex3f(x0, y1, z1_y1);
    setNormalFromTriangle(x1, y0, z1_y0, x1, y1, z1_y1, x1, y1, zOuterR);
    glVertex3f(x1, y0, z1_y0); glVertex3f(x1, y1, z1_y1); glVertex3f(x1, y1, zOuterR); glVertex3f(x1, y0, zOuterR);
    glEnd();

    endTexturedDraw();
}

float getHillSurfaceY(float z) {
    const float sideInset = 0.15f;
    const float inner = 9.0f + sideInset;
    const float outer = 18.0f + sideInset;
    const float farSide = 50.0f;

    if (z <= -outer && z >= -farSide) return 8.0f;
    if (z >= outer && z <= farSide) return 8.0f;

    if (z < -inner && z > -outer) {
        return ((-z - inner) / (outer - inner)) * 8.0f;
    }

    if (z > inner && z < outer) {
        return ((z - inner) / (outer - inner)) * 8.0f;
    }

    return 0.0f;
}

void drawTree(float x, float y, float z, float scale) {
    glDisable(GL_TEXTURE_2D);

    glColor3f(0.34f, 0.24f, 0.14f);
    setSpecular(0.08f, 8.0f);
    drawCuboid(
        x - 0.12f * scale,
        y,
        z - 0.12f * scale,
        0.24f * scale,
        1.8f * scale,
        0.24f * scale
    );

    glColor3f(0.14f, 0.44f, 0.16f);
    setSpecular(0.12f, 10.0f);
    glPushMatrix();
    glTranslatef(x, y + 1.8f * scale, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.85f * scale, 2.0f * scale, 12, 8);
    glPopMatrix();

    glColor3f(0.11f, 0.36f, 0.14f);
    glPushMatrix();
    glTranslatef(x, y + 2.6f * scale, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.62f * scale, 1.5f * scale, 12, 8);
    glPopMatrix();
}

void drawSideScenery() {
    const float xMin = -36.0f;
    const float xMax = 32.0f;
    const float zNegMin = -49.0f;
    const float zNegMax = -22.0f;
    const float zPosMin = 22.0f;
    const float zPosMax = 49.0f;
    const int treesPerSide = 28;

    
    auto prand01 = [](int seed) {
        float v = sinf(seed * 12.9898f) * 43758.5453f;
        return v - floorf(v);
    };

    for (int side = 0; side < 2; side++) {
        float zMin = (side == 0) ? zNegMin : zPosMin;
        float zMax = (side == 0) ? zNegMax : zPosMax;

        int placed = 0;
        int attempts = 0;
        while (placed < treesPerSide && attempts < 300) {
            int seed = 1300 + side * 700 + attempts * 29;
            float x = xMin + prand01(seed + 3) * (xMax - xMin);
            float z = zMin + prand01(seed + 7) * (zMax - zMin);
            float s = 0.78f + prand01(seed + 13) * 0.42f;
            float y = getHillSurfaceY(z) + 0.02f;

            
            if (y > 0.22f) {
                drawTree(x, y, z, s);
                placed++;
            }

            attempts++;
        }
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

    GLfloat light0Position[] = {18.0f, 28.0f, 24.0f, 1.0f};
    GLfloat light1Position[] = {-20.0f, 10.0f, -26.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawGround();
    drawHills();
    drawSideScenery();
    drawDam();
    drawDamTopDetails();
    drawGates();
    drawSupportWalls();
    drawReservoirWater();
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

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat globalAmbient[] = {0.20f, 0.20f, 0.22f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    GLfloat light0Diffuse[] = {0.95f, 0.92f, 0.85f, 1.0f};
    GLfloat light0Specular[] = {1.0f, 0.98f, 0.92f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);

    GLfloat light1Diffuse[] = {0.25f, 0.33f, 0.45f, 1.0f};
    GLfloat light1Specular[] = {0.10f, 0.12f, 0.18f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);

    initTextures();
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
