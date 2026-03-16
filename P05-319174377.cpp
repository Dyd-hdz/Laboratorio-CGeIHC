/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//creacion de instancias para los modelos
Model goddard_cuerpo_M;
Model goddard_cola_M;
Model goddard_mandibula_M;
Model goddard_pataD_M;
Model goddard_pataT_M;

Model carro_cuerpo_M;
Model carro_llantaD_M;
Model carro_llantaI_M;
Model carro_cajuela_M;


Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	goddard_cuerpo_M = Model();//inicializacion del modelo goddard cuerpo (cuerpo y cabeza)
	goddard_cuerpo_M.LoadModel("Models/goddard_cuerpo.obj");

	goddard_cola_M = Model();//inicializacion del modelo goddard cola
	goddard_cola_M.LoadModel("Models/goddard_cola.obj");

	goddard_mandibula_M = Model();//inicializacion del modelo goddard mandibula inferior
	goddard_mandibula_M.LoadModel("Models/goddard_mandibula.obj");

	goddard_pataD_M = Model();//inicializacion del modelo goddard pata delantera
	goddard_pataD_M.LoadModel("Models/goddard_pata_delantera.obj");

	goddard_pataT_M = Model();//inicializacion del modelo goddard pata delantera
	goddard_pataT_M.LoadModel("Models/goddard_pata_tracera.obj");

	carro_cuerpo_M = Model();//inicializacion del modelo del carro cuerpo
	carro_cuerpo_M.LoadModel("Models/carro_cuerpo.obj");

	carro_llantaD_M = Model();//inicializacion del modelo llanta derecha del carro
	carro_llantaD_M.LoadModel("Models/llanta_derecha.obj");

	carro_llantaI_M = Model();//inicializacion del modelo llanta izquierda del carro
	carro_llantaI_M.LoadModel("Models/llanta_izquierda.obj");

	carro_cajuela_M = Model();//inicializacion del modelo cajuela del carro
	carro_cajuela_M.LoadModel("Models/carro_cajuela.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//para mover el carro
	float Ucarro = 0.0f; //ubicacion del carro
	float mover = 0.1f; //la velocidad en que se mueve

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.55f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//se movera al oprimir P (hacia delante) y O (hacia atras)
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {
			Ucarro += mover * deltaTime;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			Ucarro -= mover * deltaTime;
		}

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//para el modelo de Goddard 231-293
		//Goddard cuerpo, modelo padre
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_cuerpo_M.RenderModel();//*/
		
		//goddard mandibula, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.8f, -0.15f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, -1.0f));//tecla f, agrega una rotacion independiente en Z
		//model = glm::rotate(model, glm::radians(210.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_mandibula_M.RenderModel();//*/

		//goddard cola, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(-2.27f, -0.05f, 0.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_cola_M.RenderModel();//*/

		//goddard pata delantera derecha, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.4f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));//tecla g, agrega una rotacion independiente en Z
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_pataD_M.RenderModel();//*/

		//goddard pata delantera izquierda, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, -0.85f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));//tecla h, agrega una rotacion independiente en Z
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_pataD_M.RenderModel();//*/

		//goddard pata tracera derecha, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -1.14f, 0.4f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));//tecla j, agrega una rotacion independiente en Z
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_pataT_M.RenderModel();//*/

		//goddard pata tracera izquierda, modelo hijo
		/*model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, -1.14f, -0.85f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));//tecla k, agrega una rotacion independiente en Z
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		goddard_pataT_M.RenderModel();//*/

		//para el modelo del carro
		//modelo del cuerpo del carro
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::translate(model, glm::vec3(Ucarro, 0.16f, 0.0f));
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_cuerpo_M.RenderModel();//*/

		//llanta derecha delantera, modelo hijo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.7f, -0.4f, 0.9f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con P, hacia delante
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con O, hacia atras
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_llantaD_M.RenderModel();//*/

		//llanta izquierda delantera, modelo hijo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.7f, -0.4f, -0.88f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con P, hacia delante
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con O, hacia atras
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_llantaI_M.RenderModel();//*/

		//llanta derecha tracera, modelo hijo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.4f, 0.9f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con P, hacia delante
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con O, hacia atras
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_llantaD_M.RenderModel();//*/

		//llanta izquierda delantera, modelo hijo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.4f, -0.88f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con P, hacia delante
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));//para rotar con O, hacia atras
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_llantaI_M.RenderModel();//*/

		//cajuela, modelo hijo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, 0.4f, -0.078f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro_cajuela_M.RenderModel();//*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
