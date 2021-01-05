#include "myglwidget.h"

#include <chrono> 

using namespace std;
using namespace glm;
constexpr float PI = 3.1415926;

using namespace chrono;

int sampling_times = 0;
double average_time = 0.0;


/*###################################################
##  函数: loadShader
##  函数描述：读取shader文件中的内容
##  参数描述：shader文件名
#####################################################*/
string MyGLWidget::loadShader(const string& fileName) {
	ifstream file;
	file.open(fileName.c_str());

	string output, line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		cerr << "loadShader " << fileName << "failed." << endl;
	}

	return output;
}


/*###################################################
##  函数: createShader
##  函数描述：创建一个特定类型的shader
##  参数描述：shader类型以及shader文件内容
#####################################################*/
GLuint MyGLWidget::createShader(GLenum type, const char* shaderSrc) {
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		cerr << "create shader failed." << endl;
	}

	glShaderSource(shader, 1, &shaderSrc, NULL);

	GLint compiled;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char* error = (char*)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, error);
			cerr << "compile shader failed: " << error << endl;
			free(error);
		}
	}

	return shader;
}


/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/
MyGLWidget::MyGLWidget(QWidget* parent)
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
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glViewport(0, 0, width(), height());
	glEnable(GL_DEPTH_TEST);// 打开深度测试 

	/* **************************************** shader **************************************** */
	//Your Implementation
	program = glCreateProgram();
	if (!program) {
		cerr << "create program failed." << endl;
	}

	GLuint vertexShader = createShader(GL_VERTEX_SHADER, loadShader("phong.vs").c_str());
	GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, loadShader("phong.fs").c_str());

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	GLint linked;
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char* error = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, error);
			cerr << "link program failed: " << error << endl;
			free(error);
		}
	}

	GLint valid;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
	if (valid == GL_FALSE) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char* error = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, error);
			cerr << "validate program failed: " << error << endl;
			free(error);
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/* **************************************** shader **************************************** */

	ifstream ifile("sphere.obj");
	auto ret = LoadOBJ(ifile);
	vertices = ret.first;
	indices = ret.second;
	ifile.close();
	cout << vertices.size() << endl << indices.size() << endl;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Vertices
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16); // Normals
	glEnableVertexAttribArray(1);

	viewId = glGetUniformLocation(program, "view");
	modelId = glGetUniformLocation(program, "model");
	projectionId = glGetUniformLocation(program, "projection");
	normalId = glGetUniformLocation(program, "normal");
	lightId = glGetUniformLocation(program, "light_world");
	lightMatId = glGetUniformLocation(program, "light_mat");
	ambientMatId = glGetUniformLocation(program, "ambient_mat");
	diffuseMatId = glGetUniformLocation(program, "diffuse_mat");
	specularMatId = glGetUniformLocation(program, "specular_mat");
	specularPowerId = glGetUniformLocation(program, "specular_power");
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
	auto start = system_clock::now();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glUseProgram(program);

	// Camera.
	//vec3 position = vec3(cos(camera_angle_x) * camera_scale, 0, sin(camera_angle_x) * camera_scale);
	vec3 position = vec3(0.0f, 0.0f, 5.0f);
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);

	// Matrices.
	projection = perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	view = lookAt(position, center, up);
	model = mat4(1.0);

	// light.
	light_pos = glm::vec3(1.0f, 0.0f, 3.0f);
	glUniform3f(lightId, light_pos.x, light_pos.y, light_pos.z);
	glUniform3f(lightMatId, 1.0, 1.0, 1.0);

	// Matrices.
	mat3 normal_matrix = mat3(transpose(inverse(view * model)));
	glUniformMatrix4fv(modelId, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix3fv(normalId, 1, GL_FALSE, &normal_matrix[0][0]);

	// Shader Vars.
	glUniform3f(ambientMatId, 0.5, 0, 0);
	glUniform3f(diffuseMatId, 0.5, 0, 0);
	glUniform3f(specularMatId, 0.5, 0.5, 0.5);
	glUniform1f(specularPowerId, 5.0);

	// Drawing.
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	sampling_times += 1;
	auto sec = double(duration.count()) * microseconds::period::num / microseconds::period::den;
	average_time = average_time * (sampling_times - 1) / sampling_times + sec / sampling_times;
	cout << "Average " << average_time << " seconds." << endl;
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