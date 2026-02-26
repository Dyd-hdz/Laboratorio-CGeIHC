//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;
float angulo1 = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras() 
{
	//3 verticecs -> triangulo
	//6 vertices -> cuadrado (ya que son dos triangulos juntos)
	GLfloat vertices_letras[] = {//triangulo azul, con comentarios para cuadrado azul	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f, //primer vertice
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f, //segundo vertice
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f, //tercer vertice
			/*1.0f,	1.0f,		0.5f,			.0f,	0.0f,	1.0f, //cuarto vertice
			-1.0f,  1.0f,		0.5f,			.0f,	0.0f,	1.0f, //quinto vertice
			-1.0f,	-1.0f,		0.5f,			.0f,	0.0f,	1.0f,*/ //sexto vertice
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);//18 para triangulo, 36 para un cuadrado
	meshColorList.push_back(letras);

	//triangulo rojo
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	//cuadrado verde
	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);//36 para cuadrado
	meshColorList.push_back(cuadradoverde);

	//triangulo verde
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	//cuadrado rojo
	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);//36 para cuadrado
	meshColorList.push_back(cuadradorojo);

	//cuadrado cafe
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);//36 para cuadrado
	meshColorList.push_back(cuadradocafe);

	//triangulo azul
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);//proyeccion ortogonal
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		
		//para dibujar la casa a traves de perspective lineas 245-352											
		//para dibujar el triangulo azul, el primero techo lado izq
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		/*shaderList[1].useShader();//shaderlist[1]->para las objetos color vertices
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);//reseteamos la matriz a la identidad, para hacer correctamente las transformacion
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, -5.0f));//para la posicion de la figura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();//finalmente dibujamos como el indice es 0 del meshColorList: el triangulo azul

		//para dibujar el triangulo azul, el segundo techo lado derecho, rotamos el triangulo azul 180º y ajustamos el tamaño a travez de Z
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -4.0f));//tomar en cuenta que cuando se rota la figura sufre 
		//un encojimiento debido al eje Z, ajustar el valor de este eje para que coincidan los tamaños de los triangulo
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//para rotar el triangulo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();//dibujo del triangulo azul

		//para dibujar el cuadrado rojo
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));//para hacer mas grande o mas chica la figura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();//dibujo del cuadrado rojo

		//para dibujar el cuadrado verde 1, ventana izq
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.4f, -0.4f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado verde 2, ventana derecha
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.4f, -0.4f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado verde 3, puerta
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.34f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado cafe 1, tronco izq
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.6f, -1.43f, -3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();//dibujo del cuadrado cafe

		//para dibujar el cuadrado cafe 2, tronco der
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.6f, -1.43f, -3.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();//dibujo del cuadrado cafe

		//para dibujar el triangulo verde 1, pino izq
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.7f, -1.865f, -7.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();//dibujo del triangulo verde

		//para dibujar el triangulo verde 2, pino derecho
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.7f, -1.865f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();//dibujo del triangulo verde*/

		
		//para dibujar la casa a traves de ortogonal linas 355-466										
		//para dibujar el triangulo azul, el primero techo lado izq
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();//shaderlist[1]->para las objetos color vertices
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);//reseteamos la matriz a la identidad, para hacer correctamente las transformacion
		model = glm::translate(model, glm::vec3(-1.5f, 2.968f, -5.0f));//para la posicion de la figura
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));//para hacer mas grande o mas chica la figura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();//finalmente dibujamos como el indice es 0 del meshColorList: el triangulo azul

		//para dibujar el triangulo azul, el segundo techo lado derecho, rotamos el triangulo azul 180º
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 2.968f, -5.0f));//tomar en cuenta que cuando se rota la figura sufre 
		//un encojimiento debido al eje Z, ajustar el valor de este eje para que coincidan los tamaños de los triangulo
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//para rotar el triangulo
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));//para hacer mas grande o mas chica la figura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();//dibujo del triangulo azul

		//para dibujar el cuadrado rojo
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.78f, -4.0f));
		model = glm::scale(model, glm::vec3(4.5f, 6.5f, 4.5f));//para hacer mas grande o mas chica la figura
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();//dibujo del cuadrado rojo

		//para dibujar el cuadrado verde 1, ventana izq
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, -0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado verde 2, ventana derecha
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado verde 3, puerta
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -4.2f, -1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();//dibujo del cuadrado verde

		//para dibujar el cuadrado cafe 1, tronco izq
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.7f, -4.38f, -1.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();//dibujo del cuadrado cafe

		//para dibujar el cuadrado cafe 2, tronco der
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.7f, -4.38f, -1.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();//dibujo del cuadrado cafe

		//para dibujar el triangulo verde 1, pino izq
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.7f, -2.05f, -1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();//dibujo del triangulo verde

		//para dibujar el triangulo verde 2, pino derecho
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.7f, -2.05f, -1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.7f, 1.0f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();//dibujo del triangulo verde*/

		
		//lineas para dibujar el cubo y rotarlo
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		/*shaderList[0].useShader(); //shaderlist[0]-> para el las piramides y cubos
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.0005;//->para la rotacion de la figura, se sustituyo en la funcion glm::rotate segundo parametro
		Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);//reseteamos la matriz a la identidad, para hacer correctamente las transformaciones
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));//para acercar o alejar del eje Z en este caso, para posicionar la figura
		model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f));//para rotar
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//finalmente dibujamos como es indice 1 el meshlist: es el cubo*/

		//lineas para dibujar la piramide que rota
		/*shaderList[0].useShader(); //shaderlist[0]-> para el las piramides y cubos
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo1 += 0.0005;//->para la rotacion de la figura, se sustituyo en la funcion glm::rotate segundo parametro
		Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);//reseteamos la matriz a la identidad, para hacer correctamente las transformaciones
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));//para acercar o alejar del eje Z en este caso, para posicionar la figura
		model = glm::rotate(model, angulo1, glm::vec3(0.0f, 1.0f, 0.0f));//para rotar
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();//finalmente dibujamos como es indice 0 el meshlist: es la piramide*/


		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/