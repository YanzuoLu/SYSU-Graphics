#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
    :QOpenGLWidget(parent),
    scene_id(1)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    if (scene_id==0) {
        scene_0();
    }
    else {
        scene_1();
    }
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
    //Press 0 or 1 to switch the scene
    if (e->key() == Qt::Key_0) {
        scene_id = 0;
        update();
    }
    else if (e->key() == Qt::Key_1) {
        scene_id = 1;
        update();
    }
}

void MyGLWidget::scene_0()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(50.0f, 50.0f, 0.0f);
    
    //draw a diagonal "I"
    glPushMatrix();
    glColor3f(0.839f, 0.153f, 0.157f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-2.5f, -22.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);

    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 45.0f);
    glVertex2f(5.0f, 45.0f);

    glEnd();
    glPopMatrix();	
}

void MyGLWidget::scene_1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);

    //your implementation here, maybe you should write several glBegin
    glPushMatrix();
    //your implementation
    glColor3f(1.0f, 0.0f, 0.0f);

    // character "L"
    glTranslatef(-150.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(100.0f, 0.0f);
    glVertex2f(100.0f, 20.0f);
    glVertex2f(20.0f, 0.0f);
    glVertex2f(20.0f, 20.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(20.0f, 150.0f);
    glVertex2f(0.0f, 150.0f);
    glEnd();

    // charactr "Y"
    glTranslatef(100.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(0.0f, 130.0f);
    glVertex2f(20.0f, 150.0f);
    glVertex2f(35.85f, 100.0f);
    glVertex2f(50.0f, 120.0f);
    glVertex2f(64.14f, 100.0f);
    glVertex2f(80.0f, 150.0f);
    glVertex2f(100.0f, 130.0f);
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(35.85f, 0.0f);
    glVertex2f(35.85f, 100.0f);
    glVertex2f(64.14f, 0.0f);
    glVertex2f(64.14f, 100.0f);
    glEnd();

    // charactr "Z"
    glTranslatef(100.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(100.0f, 0.0f);
    glVertex2f(100.0f, 20.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(20.0f, 20.0f);
    glVertex2f(0.0f, 30.0f);
    glVertex2f(100.0f, 130.0f);
    glVertex2f(80.0f, 130.0f);
    glVertex2f(100.0f, 150.0f);
    glVertex2f(0.0f, 130.0f);
    glVertex2f(0.0f, 150.0f);
    glEnd();

    glPopMatrix();
}