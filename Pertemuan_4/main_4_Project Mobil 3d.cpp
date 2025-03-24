#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

/* 	Nama File	: 4_Project Mobil 3d
	Nama		: Moh Yusril Nur Syabani
	Lab			: GKV C1
	Tanggal		: 24 Maret 2025	
*/

// Fungsi pembantu untuk menggambar sebuah cuboid (balok)
void drawCuboid(float length, float height, float width,
                float r, float g, float b)
{
    glColor3f(r, g, b);
    glPushMatrix();
        glScalef(length, height, width);
        glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi untuk menggambar satu roda sebagai torus
void drawWheel(float xOff, float zOff, float wheelRadius, float wheelThickness)
{
    glPushMatrix();
        glTranslatef(xOff, -0.25f, zOff);
        // Rotasi 90 derajat agar torus tegak (posisi roda)
        glRotatef(90, 0, 1, 0);
        glColor3f(0.0f, 0.0f, 0.0f);
        glutSolidTorus(wheelThickness, wheelRadius, 12, 12);
    glPopMatrix();
}

// Variabel global untuk posisi dan orientasi mobil
float carPosX = 0.0f, carPosZ = 0.0f;
float carAngle = 0.0f;  // sudut mobil (dalam derajat)

// Fungsi untuk menggambar mobil 3D
void drawCar()
{
    glPushMatrix();
        // Pindahkan dan rotasi mobil sesuai variabel global
        glTranslatef(carPosX, 0.0f, carPosZ);
        glRotatef(carAngle, 0.0f, 1.0f, 0.0f);

        // 1. BADAN UTAMA MOBIL (warna merah)
        drawCuboid(3.0f, 0.5f, 1.5f, 0.8f, 0.0f, 0.0f);

        // 2. BUMPER DEPAN (warna kuning keabuan)
        glPushMatrix();
            glTranslatef(1.5f, -0.05f, 0.0f);
            drawCuboid(0.3f, 0.4f, 1.5f, 0.8f, 0.8f, 0.0f);
        glPopMatrix();

        // 3. BUMPER BELAKANG (warna abu-abu)
        glPushMatrix();
            glTranslatef(-1.5f, -0.05f, 0.0f);
            drawCuboid(0.3f, 0.4f, 1.5f, 0.5f, 0.5f, 0.5f);
        glPopMatrix();

        // 4. ATAP MOBIL (untuk kabin, masih merah)
        glPushMatrix();
            glTranslatef(0.0f, 0.5f, 0.0f);
            drawCuboid(2.0f, 0.5f, 1.2f, 0.8f, 0.0f, 0.0f);
        glPopMatrix();

        // 5. JENDELA MOBIL (simulasi kaca dengan blending)
        glPushMatrix();
            glTranslatef(0.0f, 0.51f, 0.0f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // Gunakan warna dengan alpha < 1 untuk efek transparan
            glColor4f(0.8f, 0.9f, 1.0f, 0.7f);

            // Jendela depan
            glPushMatrix();
                glTranslatef(0.8f, 0.0f, 0.0f);
                glScalef(0.01f, 0.4f, 1.0f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Jendela belakang
            glPushMatrix();
                glTranslatef(-0.8f, 0.0f, 0.0f);
                glScalef(0.01f, 0.4f, 1.0f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Jendela samping kiri
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, 0.6f);
                glScalef(1.6f, 0.4f, 0.01f);
                glutSolidCube(1.0f);
            glPopMatrix();

            // Jendela samping kanan
            glPushMatrix();
                glTranslatef(0.0f, 0.0f, -0.6f);
                glScalef(1.6f, 0.4f, 0.01f);
                glutSolidCube(1.0f);
            glPopMatrix();

            glDisable(GL_BLEND);
        glPopMatrix();

        // 6. RODA MOBIL
        float wheelRadius = 0.3f;
        float wheelThickness = 0.08f;
        // Roda depan kanan
        drawWheel(1.0f, -0.7f, wheelRadius, wheelThickness);
        // Roda depan kiri
        drawWheel(1.0f, 0.7f, wheelRadius, wheelThickness);
        // Roda belakang kanan
        drawWheel(-1.0f, -0.7f, wheelRadius, wheelThickness);
        // Roda belakang kiri
        drawWheel(-1.0f, 0.7f, wheelRadius, wheelThickness);
    glPopMatrix();
}

// Inisialisasi pencahayaan dan depth testing
void initLightingAndDepth()
{
    // Latar belakang: langit biru
    glClearColor(0.5, 0.8, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Atur properti lampu
    GLfloat lightPos[]      = { 4.0f, 5.0f, 6.0f, 1.0f };
    GLfloat ambientLight[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[]  = { 0.9f, 0.9f, 0.9f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    // Izinkan penggunaan warna untuk material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Pastikan vektor normal selalu ternormalisasi
    glEnable(GL_NORMALIZE);
}

// Fungsi display: menggambar adegan
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Atur kamera dengan gluLookAt(eye, center, up)
    gluLookAt( 5.0, 3.0, 8.0,  // posisi kamera
               0.0, 0.0, 0.0,  // titik yang dilihat
               0.0, 1.0, 0.0); // arah atas

    // Gambar tanah (sebagai alas hijau)
    glPushMatrix();
        glColor3f(0.3f, 0.7f, 0.3f);
        glTranslatef(0.0f, -0.5f, 0.0f);
        glScalef(20.0f, 0.01f, 20.0f);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Gambar mobil
    drawCar();

    glutSwapBuffers();
}

// Fungsi reshape untuk menangani perubahan ukuran jendela
void reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    float ratio = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 100.0);
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback untuk menangani input tombol panah (putar mobil)
void specialKeys(int key, int x, int y)
{
    float turnSpeed = 5.0f; // kecepatan rotasi (dalam derajat)
    switch (key)
    {
        case GLUT_KEY_LEFT:
            carAngle += turnSpeed;
            break;
        case GLUT_KEY_RIGHT:
            carAngle -= turnSpeed;
            break;
    }
    glutPostRedisplay();
}

// Fungsi utama
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Car (Rotasi)");

    initLightingAndDepth();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);  // Callback untuk tombol panah

    glutMainLoop();
    return 0;
}
