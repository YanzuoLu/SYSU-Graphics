#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "objUtils.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions{
	Q_OBJECT

public:
	MyGLWidget(QWidget* parent = nullptr);
	~MyGLWidget();

	GLuint createShader(GLenum type, const char* shaderSrc);
	std::string loadShader(const std::string& fileName);
	void initObj(std::string obj_path);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:
	QTimer* timer;

	GLuint program;

	std::vector<Vertex> obj;
	GLuint obj_buffer;

	GLuint viewId;
	GLuint modelId;
	GLuint projectionId;
	GLuint normalId;
	GLuint lightId;
	GLuint lightMatId;
	GLuint ambientMatId;
	GLuint diffuseMatId;
	GLuint specularMatId;
	GLuint specularPowerId;

	// Camera vars.
	//float camera_angle_x = 90.0;
	//float camera_angle_y = 0.0;
	//double camera_scale = 5.0;

	// Graphics pipeline matrices.
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

	// Lighting
	glm::vec3 light_pos;
};
#endif // MYGLWIDGET_H
