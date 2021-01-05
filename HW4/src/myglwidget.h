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

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:
	QTimer* timer;

	GLuint program;
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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

	// Graphics pipeline matrices.
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

	// Lighting
	glm::vec3 light_pos;
};
#endif // MYGLWIDGET_H
