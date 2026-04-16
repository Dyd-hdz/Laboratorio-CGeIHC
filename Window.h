#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; } //para el carro
	GLfloat getmuevexE() { return muevexE; } //para el helicoptero
	GLfloat getmuevexyP() { return muevexyP; } //para el pez en diagonal 1
	GLfloat getmuevenxyP() { return muevenxyP; } //para el pez en diagonal 2
	GLfloat getpezX() { return pezX; }
	GLfloat getpezY() { return pezY; }
	GLfloat getpezZ() { return pezZ; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex; //para el carro
	GLfloat muevexE; //para el helicoptero
	GLfloat muevexyP; //para mover en diagonal 1 al pez
	GLfloat muevenxyP; //para mover en diagonal 2 al pez
	GLfloat pezX;//para mover la direccion de la luz spotlight del pez
	GLfloat pezY;
	GLfloat pezZ;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

