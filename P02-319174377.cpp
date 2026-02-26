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
static const char* vShaderColor = "shaders/shadercolor.vert";//para los objetos color vertice
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* fShaderRojo = "shaders/shaderRojo.frag";//instanciamos los archivos .frag en el main
static const char* fShaderVerde = "shaders/shaderVerde.frag";
static const char* fShaderAzul = "shaders/shaderAzul.frag";
static const char* fShaderCafe = "shaders/shaderCafe.frag";
static const char* fShaderVerdeObs = "shaders/shaderVerdeObs.frag";

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

	//letra L
	GLfloat vertices_letraL[] = {
		//X			Y			Z			R		G		B
		-.9f,	 0.3f,		0.0f,			1.0f,	0.647f,	0.0f,//primer triangulo de la letra L
		-.7f,	 0.3f,		0.0f,			1.0f,	0.647f,	0.0f,
		-.9f,	-0.3f,		0.0f,			1.0f,	0.647f,	0.0f,

		-.7f,	.3f,		0.0f,			1.0f,	0.647f,	0.0f,//segundo triangulo de la letra L
		-.9f,	-.3f,		0.0f,			1.0f,	0.647f,	0.0f,
		-.7f,	-.3f,		0.0f,			1.0f,	0.647f,	0.0f,

		-.7f,	-.3f,		0.0f,			1.0f,	0.647f,	0.0f,//tercertriangulo de la letra L
		-.7f,	-.1f,		0.0f,			1.0f,	0.647f,	0.0f,
		-.5f,	-.3f,		0.0f,			1.0f,	0.647f,	0.0f,

		-.7f,	-.1f,		0.0f,			1.0f,	0.647f,	0.0f,//cuarto triangulo de la letra L
		-.5f,	-.3f,		0.0f,			1.0f,	0.647f,	0.0f,
		-.5f,	-.1f,		0.0f,			1.0f,	0.647f,	0.0f,
	};

	MeshColor* letraL = new MeshColor();
	letraL->CreateMeshColor(vertices_letraL, 72);//12 vertices para la letra L, por tanto 12x6=72
	meshColorList.push_back(letraL);

	//letra E
	GLfloat vertices_letraE[] = {
		//X			Y			Z			R		G		B
		-.3f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,//primer triangulo de la letra E
		-.2f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,
		-.3f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,//segundo triangulo de la letra E
		-.3f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f,
		-.2f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,//tercer triangulo de la letra E
		-.2f,	.2f,		0.0f,			0.529f, 0.808f, 0.922f,
		.1f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	.2f,		0.0f,			0.529f, 0.808f, 0.922f,//cuarto triangulo de la letra E
		.1f,	.3f,		0.0f,			0.529f, 0.808f, 0.922f,
		.1f,	.2f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	-.1f,		0.0f,			0.529f, 0.808f, 0.922f,//quinto triangulo de la letra E
		-.2f,	.1f,		0.0f,			0.529f, 0.808f, 0.922f,
		.0f,	-.1f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	.1f,		0.0f,			0.529f, 0.808f, 0.922f,//sexto triangulo de la letra E
		.0f,	-.1f,		0.0f,			0.529f, 0.808f, 0.922f,
		.0f,	.1f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	-.2f,		0.0f,			0.529f, 0.808f, 0.922f,//septimo triangulo de la letra E
		-.2f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f,
		.1f,	-.2f,		0.0f,			0.529f, 0.808f, 0.922f,

		-.2f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f,//octavo triangulo de la letra E
		.1f,	-.2f,		0.0f,			0.529f, 0.808f, 0.922f,
		.1f,	-.3f,		0.0f,			0.529f, 0.808f, 0.922f
	};
	MeshColor* letraE = new MeshColor();
	letraE->CreateMeshColor(vertices_letraE, 144);//24 vertices para la letra E, por tanto 24x6=144
	meshColorList.push_back(letraE);

	//letra H
	GLfloat vertices_letraH[] = {
		//X			Y			Z			R		G		B
		.3f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,//primer triangulo de la letra H
		.5f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,
		.3f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,

		.5f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,//segundo triangulo de la letra H
		.3f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,
		.5f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,

		.5f,	.1f,		0.0f,			0.486f, 0.733f, 0.231f,//tercer triangulo de la letra H
		.5f,	-.1f,		0.0f,			0.486f, 0.733f, 0.231f,
		.7f,	.1f,		0.0f,			0.486f, 0.733f, 0.231f,

		.5f,	-.1f,		0.0f,			0.486f, 0.733f, 0.231f,//cuarto triangulo de la letra H
		.7f,	.1f,		0.0f,			0.486f, 0.733f, 0.231f,
		.7f,	-.1f,		0.0f,			0.486f, 0.733f, 0.231f,

		.7f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,//quinto triangulo de la letra H
		.7f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,
		.9f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,

		.7f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,//sexto triangulo de la letra H
		.9f,	.3f,		0.0f,			0.486f, 0.733f, 0.231f,
		.9f,	-.3f,		0.0f,			0.486f, 0.733f, 0.231f,
	};
	MeshColor* letraH = new MeshColor();
	letraH->CreateMeshColor(vertices_letraH, 108);//18 vertices para la letra H, por tanto 18x6=108
	meshColorList.push_back(letraH);
}


void CreateShaders()
{
	//indice del shaderList[-]: 0
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	//indice del shaderList[-]: 1
	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//indice del shaderList[-]:2
	//agregamos los nuevos shader a la lista shaderList
	//para el .frag con color fijo Rojo
	Shader* shader3 = new Shader();
	shader3->CreateFromFiles(vShader, fShaderRojo);//archivo .vert y .frag, el archivo .vert va ser constante
	//ya que este archivo no cambia ya que solo se encarga de la posicion del objeto, el .frag se encarga del color
	shaderList.push_back(*shader3);

	//indice del shaderList[-]: 3
	//para el .frag con color fijo Verde
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shader4);

	//indice del shaderList[-]: 4
	//para el .frag con color fijo Azul
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shader5);

	//indice del shaderList[-]: 5
	//para el .frag con color fijo Cafe
	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shader6);

	//indice del shaderList[-]: 6
	//para el .frag con color fijo Verde Obscuro
	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShader, fShaderVerdeObs);
	shaderList.push_back(*shader7);
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
	//glm::mat4 projection = glm::perspective(glm::radians(30.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


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
		
		
		//para dibujar la letra L
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		/*shaderList[1].useShader();//shaderlist[1]->para las objetos color vertices
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);//reseteamos la matriz a la identidad, para hacer correctamente las transformacion
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -5.0f));//para la posicion de la figura
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor();//letra L

		//para dibujar la letra E
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[8]->RenderMeshColor();//letra E

		//para dibujar la letra H
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -5.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[9]->RenderMeshColor();//letra H*/

		
		//para dibujar el cubo rojo
		shaderList[2].useShader(); //shaderlist[2] -> color rojo
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		//angulo += 0.0005;//->para la rotacion de la figura, se sustituyo en la funcion glm::rotate segundo parametro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.0f));
		model = glm::scale(model, glm::vec3(5.0f, 6.0f, 5.0f));
		//model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f));//para rotar de forma continua
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color rojo

		//lineas para dibujar la piramide azul, techo
		shaderList[4].useShader(); //shaderlist[4]-> color azul
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(6.0f, 3.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();//piramide color azul

		//lineas para dibujar el cubo verde 1, ventana izq
		shaderList[3].useShader(); //shaderlist[3]-> color verde
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.3f, -1.0f, -1.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color verde

		//lineas para dibujar el cubo verde 2, ventana derecha
		shaderList[3].useShader(); //shaderlist[3]-> color verde
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.3f, -1.0f, -1.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color verde

		//lineas para dibujar el cubo verde 3, puerta
		shaderList[3].useShader(); //shaderlist[3]-> color verde
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -4.1f, -1.5f));
		model = glm::scale(model, glm::vec3(1.5f, 1.9f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color verde

		//lineas para dibujar el cubo cafe 1, tronco izq
		shaderList[5].useShader(); //shaderlist[5]-> color cafe
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.8f, -4.4f, -1.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color cafe

		//lineas para dibujar el cubo cafe 2, tronco der
		shaderList[5].useShader(); //shaderlist[5]-> color cafe
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.8f, -4.4f, -1.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();//cubo color cafe

		//lineas para dibujar la piramide verde 1, hojas verde izq
		shaderList[6].useShader(); //shaderlist[5]-> color verde obs
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.8f, -2.3f, -0.1f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();//oiramide color verde obscuro

		//lineas para dibujar la piramide verde 2, hojas verde der
		shaderList[6].useShader(); //shaderlist[5]-> color verde obs
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.8f, -2.3f, -0.1f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();//oiramide color verde obscuro

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