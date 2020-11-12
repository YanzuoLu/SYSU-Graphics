#include "myglwidget.h"

float arm_angle = 0.0f;	// 左右手臂角度
float leg_angle = 0.0f;	// 左右腿角度
int arm_move = 0;	// 0表示左臂在运动，1表示右臂在运动
int leg_move = 1;	// 0表示左腿在运动，1表示右腿在运动
int arm_dire = 1;	// 0表示手臂角度在减小，1表示手臂角度在增大
int leg_dire = 1;	// 0表示腿角度在减小，1表示腿角度在增大
float max_angle = -30.0f;	// 手臂和腿摆动最大逆时针角度
float min_angle = 0.0f;	// 手臂和腿摆动最小逆时针角度

float posx = 0.0f;	// 绕正方形闭合曲线运动的x坐标
float posz = 80.0f;	// 绕正方形闭合曲线运动的z坐标
int dire[] = { 1, 0 };	// 坐标在x/z轴上增加1/不变0/减小-1
float max_pos = 80.0f;	// 单一坐标轴上最大坐标值
float min_pos = -80.0f;	// 单一坐标轴上最小坐标值

float angle_step = 1.5f;	// 每一帧摆动手脚的幅度
float pos_step = 0.2f;	// 每一帧位置移动的幅度


/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/
MyGLWidget::MyGLWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
    timer = new QTimer(this); // 实例化一个定时器
    timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
    connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
    delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());  
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glEnable(GL_BLEND);		// 打开混合
    glEnable(GL_DEPTH_TEST);// 打开深度测试 
}


/*###################################################
##  函数: drawCube
##  函数描述： 绘制正方体
##  参数描述： 无
#####################################################*/
void drawCube() {
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
}


/*###################################################
##  函数: drawHead
##  函数描述： 绘制机器人头部
##  参数描述： 机器人头部在坐标系中的坐标(x,y,z)
#####################################################*/
void drawHead(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(20.0f, 20.0f, 20.0f);
    glColor3f(0.72f, 0.57f, 0.55f);
    drawCube();
    glPopMatrix();
}


/*###################################################
##  函数: drawShoulder
##  函数描述： 绘制机器人肩部
##  参数描述： 机器人肩部在坐标系中的坐标(x,y,z)
#####################################################*/
void drawShoulder(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(40.0f, 10.0f, 10.0f);
    glColor4f(0.10f, 0.79f, 0.87f, 1.0f);
    drawCube();
    glPopMatrix();
}


/*###################################################
##  函数: drawTorso
##  函数描述： 绘制机器人躯干
##  参数描述： 机器人躯干在坐标系中的坐标(x,y,z)
#####################################################*/
void drawTorso(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(20.0f, 20.0f, 10.0f);
    glColor4f(0.10f, 0.79f, 0.87f, 1.0f);
    drawCube();
    glPopMatrix();
}


/*###################################################
##  函数: drawArm
##  函数描述： 绘制机器人手臂
##  参数描述： 机器人手臂在坐标系中的坐标(x,y,z)和绕x轴正方向旋转的角度
#####################################################*/
void drawArm(float x, float y, float z, float rotatex) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotatex, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -15.0f, 0.0f);
    glScalef(10.0f, 30.0f, 10.0f);
    glColor4f(0.71f, 0.55f, 0.50f, 1.0f);
    drawCube();
    glPopMatrix();
}


/*###################################################
##  函数: drawLeg
##  函数描述： 绘制机器人腿部
##  参数描述： 机器人腿部在坐标系中的坐标(x,y,z)和绕x轴正方向旋转的角度
#####################################################*/
void drawLeg(float x, float y, float z, float rotatex) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotatex, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glScalef(10.0f, 40.0f, 10.0f);
    glColor4f(0.29f, 0.27f, 0.78f, 1.0f);
    drawCube();
    glPopMatrix();
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL() {
    // Your Implementation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0f, 1.0f, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(200.0f, 200.0f, 400.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(posx, 0.0f, posz);
    if (dire[0] == 1) {
        // 朝向x轴正向
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        posx += pos_step;
        if (posx > max_pos) {
            // 到达最右
            dire[0] = 0;
            dire[1] = -1;
        }
    }
    else if (dire[0] == -1) {
        // 朝向x轴负向
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        posx -= pos_step;
        if (posx < min_pos) {
            // 到达最左
            dire[0] = 0;
            dire[1] = 1;
        }
    }
    else if (dire[1] == 1) {
        // 朝向z轴正向
        posz += pos_step;
        if (posz > max_pos) {
            // 到达最外
            dire[0] = 1;
            dire[1] = 0;
        }
    }
    else if (dire[1] == -1) {
        // 朝向z轴负向
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        posz -= pos_step;
        if (posz < min_pos) {
            // 到达最里
            dire[0] = -1;
            dire[1] = 0;
        }
    }

    drawHead(0.0f, 80.0f, 0.0f);
    drawShoulder(0.0f, 65.0f, 0.0f);
    drawTorso(0.0f, 50.0f, 0.0f);

    if (arm_move == 0) {
        // 左臂运动，右臂不变
        drawArm(15.0f, 60.0f, 0.0f, arm_angle);
        drawArm(-15.0f, 60.0f, 0.0f, 0.0f);
    }
    else {
        // 右臂运动，左臂不变
        drawArm(15.0f, 60.0f, 0.0f, 0.0f);
        drawArm(-15.0f, 60.0f, 0.0f, arm_angle);
    }

    if (arm_dire == 1) {
        // 角度增大（对应逆时针减小）
        arm_angle -= angle_step;
        if (arm_angle < max_angle) {
            // 增加角度达到最大值
            arm_dire = 0;
        }
    }
    else {
        // 角度减小（对应逆时针增大）
        arm_angle += angle_step;
        if (arm_angle > min_angle) {
            // 减小角度达到最大值
            arm_dire = 1;
            // 换手臂
            arm_move = 1 - arm_move;
        }
    }

    if (leg_move == 0) {
        // 左腿运动，右腿不变
        drawLeg(5.0f, 40.0f, 0.0f, leg_angle);
        drawLeg(-5.0f, 40.0f, 0.0f, 0.0f);
    }
    else {
        // 右腿运动，左腿不变
        drawLeg(5.0f, 40.0f, 0.0f, 0.0f);
        drawLeg(-5.0f, 40.0f, 0.0f, leg_angle);
    }

    if (leg_dire == 1) {
        // 角度增大（对应逆时针减小）
        leg_angle -= angle_step;
        if (leg_angle < max_angle) {
            // 增加角度达到最大值
            leg_dire = 0;
        }
    }
    else {
        // 角度减小（对应逆时针增大）
        leg_angle += angle_step;
        if (leg_angle > min_angle) {
            // 减小角度达到最大值
            leg_dire = 1;
            // 换腿
            leg_move = 1 - leg_move;
        }
    }
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}