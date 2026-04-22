/*
Animación:
Sesión 1: Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Sesión 2: Compleja: Por medio de funciones y algoritmos. Textura Animada
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación del carro
float movCocheX;//para que el carro avance en X
float movOffsetX;//incremento de la variable
float movCocheY;//para que el carro avance en Y
float movOffsetY;//incremento de la variable
float movCocheZ;//para que el carro avance em Z
float movOffsetZ;//incremento de la variable
float rotllanta;//para que la llantas giren
float rotllantaOffset;//incremento de la variable
bool avanza = 1;//bandera para que inicie la animacion

float inclinacion;

float giroI;
float giroIOffset;

bool tramo2;
bool tramo3;
bool tramo4;
bool tramo5;
bool tramo6;
bool tramo7;
bool tramo8;
bool tramo9;
bool tramo10;
bool tramo11;
bool tramo12;
bool tramo13;
bool tramo14;
bool tramo15;
bool tramo16;
bool tramo17;
bool tramo18;
bool tramo19;
bool tramo20;
bool tramo21;
bool tramo22;
bool tramo23;
bool tramo24;
bool tramo25;

//para la animacion de la nave
float movNaveX; //para mover en X
float movNaveXoffSet; //incremento en X
float movNaveY; //para mover en Y
float movNaveYoffSet; //incremento en Y
float movNaveZ; //para mover en Z
float movNaveZoffSet; //incremento en Z
float rotHelice;//para que la helice giren
float rotHeliceOffset;//incremento de la variable
float aleta; //para mover la aleta, girara en Y
float aletaOffset; //incremento de la aleta
bool avanzaNX = 1; //la bandera para iniciar la animacion
bool avanzaA = 1; //la bandera para inciiar la animacion de la ala

float inclinacionN;

float giroIN;
float giroIOffsetN;

bool Ntramo2;
bool Ntramo3;
bool Ntramo4;
bool Ntramo5;
bool Ntramo6;
bool Ntramo7;
bool Ntramo8;
bool Ntramo9;
bool Ntramo10;
bool Ntramo11;
bool Ntramo12;
bool Ntramo13;
bool Ntramo14;
bool Ntramo15;
bool Ntramo16;
bool Ntramo17;
bool Ntramo18;
bool Ntramo19;
bool Ntramo20;
bool Ntramo21;
bool Ntramo22;
bool Ntramo23;
bool Ntramo24;
bool Ntramo25;
bool Ntramo26;


float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Model Kitt_M;
//Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model AlaI_M;
Model Helice_M;

//modelos del carro
Model Cuerpo_M;
Model Llanta_M;
Model Parabrisas_M;
Model Cajuela_M;
Model Parrilla_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	//Llanta_M = Model();
	//Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/naveF.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	AlaI_M = Model();
	AlaI_M.LoadModel("Models/alaI.obj");
	Helice_M = Model();
	Helice_M.LoadModel("Models/helice.obj");

	//cargado del archivo del modelo .obj
	Cuerpo_M = Model();
	Cuerpo_M.LoadModel("Models/carro_cuerpo.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/carro_llanta.obj");
	Parabrisas_M = Model();
	Parabrisas_M.LoadModel("Models/carro_parabrisas.obj");
	Cajuela_M = Model();
	Cajuela_M.LoadModel("Models/carro_cajuela.obj");
	Parrilla_M = Model();
	Parrilla_M.LoadModel("Models/carro_parrilla.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz para el carro - azul
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //azul
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, //posicion temporal
		-1.0f, 0.0f, 0.0f, //direccion
		0.2f, 0.045f, 0.0075f,
		30.0f);
	spotLightCount++;//*/

	//luz para la nave
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f, //verde
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, //posicion temporal
		-0.5f, -0.5f, 0.0f, //direccion
		1.5f, 0.22f, 0.20f,
		5.0f);
	spotLightCount++;//*/

	//luz fija
	/*spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;//*/



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//para el carro
	movCocheX = 0.0f;
	movOffsetX = 0.1f;//cambia la velocidad del carro
	movCocheY = 0.0f;
	movOffsetY = 0.005;
	movCocheZ = 0.0f;
	movOffsetZ = 0.03637;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	inclinacion = 0.0;

	giroI = 0.0f;
	giroIOffset = 1.0f;

	tramo2 = 0;
	tramo3 = 0;
	tramo4 = 0;
	tramo5 = 0;
	tramo6 = 0;
	tramo7 = 0;
	tramo8 = 0;
	tramo9 = 0;
	tramo10 = 0;
	tramo11 = 0;
	tramo12 = 0;
	tramo13 = 0;
	tramo14 = 0;
	tramo15 = 0;
	tramo16 = 0;
	tramo17 = 0;
	tramo18 = 0;
	tramo19 = 0;
	tramo20 = 0;
	tramo21 = 0;
	tramo22 = 0;
	tramo23 = 0;
	tramo24 = 0;
	tramo25 = 0;

	//para la nave
	//movNaveX - 182.0f, movNaveY + 30.9691f, movNaveZ - 21.4441f
	movNaveX = -182.0f;
	movNaveXoffSet = 0.1f;//cambia la velocidad del carro
	movNaveY = 27.9691f;
	movNaveYoffSet = 0.005;
	movNaveZ = -21.4441f;
	movNaveZoffSet = 0.03637;
	rotHelice = 0.0f;
	rotHeliceOffset = 10.0f;

	aleta = 0.0f;
	aletaOffset = 0.5f;
	avanzaA = 1;

	inclinacionN = -5.7106;

	giroIN = -25.3261f;
	giroIOffsetN = 1.0f;

	Ntramo2 = 0;
	Ntramo3 = 0;
	Ntramo4 = 0;
	Ntramo5 = 0;
	Ntramo6 = 0;
	Ntramo7 = 0;
	Ntramo8 = 0;
	Ntramo9 = 0;
	Ntramo10 = 0;
	Ntramo11 = 0;
	Ntramo12 = 0;
	Ntramo13 = 0;
	Ntramo14 = 0;
	Ntramo15 = 0;
	Ntramo16 = 0;
	Ntramo17 = 0;
	Ntramo18 = 0;
	Ntramo19 = 0;
	Ntramo20 = 0;
	Ntramo21 = 0;
	Ntramo22 = 0;
	Ntramo23 = 0;
	Ntramo24 = 0;
	Ntramo25 = 0;
	Ntramo26 = 0;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::mat4 modelauxN(1.0);

	//vectores para ligar la luz spotlight del carro
	glm::vec4 poslocalLuzCarro; // para la posicion local de la luz
	glm::vec3 posxmodelLuzCarro; // para multiplicar el vector de la posicion local de la luz con la matriz model
	glm::vec4 dirLuzCarro; // para la direccion de la luz
	glm::vec3 dirxmodelLuzCarro; // para multiplicar el vector de la direccion local de la luz con la matriz model
	
	//vectores para ligar la luz spotlight de la nave
	glm::vec4 poslocalLuzN; // para la posicion local de la luz
	glm::vec3 posxmodelLuzN; // para multiplicar el vector de la posicion local de la luz con la matriz model
	glm::vec4 dirLuzN; // para la direccion de la luz
	glm::vec3 dirxmodelLuzN; // para multiplicar el vector de la direccion local de la luz con la matriz model

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		// si presiona la tecla R, se reinician los parametros del carro para que reinicie la animacion
		if (mainWindow.getsKeys()[GLFW_KEY_R]) {
			movCocheX = 0.0f;
			movCocheY = 0.0f;
			movCocheZ = 0.0f;
			inclinacion = 0.0;
			giroI = 0.0f;
			avanza = true;
			tramo2 = 0;
			tramo3 = 0;
			tramo4 = 0;
			tramo5 = 0;
			tramo6 = 0;
			tramo7 = 0;
			tramo8 = 0;
			tramo9 = 0;
			tramo10 = 0;
			tramo11 = 0;
			tramo12 = 0;
			tramo13 = 0;
			tramo14 = 0;
			tramo15 = 0;
			tramo16 = 0;
			tramo17 = 0;
			tramo18 = 0;
			tramo19 = 0;
			tramo20 = 0;
			tramo21 = 0;
			tramo22 = 0;
			tramo23 = 0;
			tramo24 = 0;
			tramo25 = 0;
		}

		// si presiona la tecla N, se reinician los parametros de la nave
		if (mainWindow.getsKeys()[GLFW_KEY_N]) {
			movNaveX = -182.0f;// reinicia el acumulador a su valor original
			movNaveY = 27.9691f;
			movNaveZ = -21.4441f;
			inclinacionN = -5.7106;
			giroIN = -25.3261f;
			aleta = 0.0f;
			aletaOffset = 5.0f;
			avanzaA = 1;
			avanzaNX = true; // reinicia la bandera a su valor original
			Ntramo2 = 0;
			Ntramo3 = 0;
			Ntramo4 = 0;
			Ntramo5 = 0;
			Ntramo6 = 0;
			Ntramo7 = 0;
			Ntramo8 = 0;
			Ntramo9 = 0;
			Ntramo10 = 0;
			Ntramo11 = 0;
			Ntramo12 = 0;
			Ntramo13 = 0;
			Ntramo14 = 0;
			Ntramo15 = 0;
			Ntramo16 = 0;
			Ntramo17 = 0;
			Ntramo18 = 0;
			Ntramo19 = 0;
			Ntramo20 = 0;
			Ntramo21 = 0;
			Ntramo22 = 0;
			Ntramo23 = 0;
			Ntramo24 = 0;
			Ntramo25 = 0;
			Ntramo26 = 0;
		}

		//para la animacion del carro	
		if (avanza) { //primer tramo de la pista
			if (movCocheX > -20.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				avanza = !avanza;
				tramo2 = !tramo2;
			}//*/
		}

		if (tramo2) { //segundo tramo de la pista, empieza a rotar el carro a la izq
			if (movCocheX > -30.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheZ += movOffsetZ * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
				if (giroI < 15) {
					giroI += giroIOffset * deltaTime;
				}
			}
			else {
				tramo2 = !tramo2;
				tramo3 = !tramo3;
			}
		}//*/

		if (tramo3) { //tercer tramo de la pista, avanza con la rotacion previa
			if (movCocheX > -35.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheZ += movOffsetZ * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
			}
			else {
				tramo3 = !tramo3;
				tramo4 = !tramo4;
			}
		}//*/

		if (tramo4) { //cuarto tramo de la pista, avanza con la rotacion previa y con un cambio de elevacion
			if (movCocheX > -48.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += movOffsetY * deltaTime;
				movCocheZ += movOffsetZ * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);
			}
			else {
				tramo4 = !tramo4;
				tramo5 = !tramo5;
			}
		}//*/

		if (tramo5) { //quinto tramo de la pista, empieza a rotar el carro a la derecha
			if (movCocheX > -53.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += movOffsetY * deltaTime;
				movCocheZ -= (movOffsetZ + 0.00002) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
					
				inclinacion = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);
				if (giroI > -5) {
						giroI -= giroIOffset * deltaTime;
				}
			}
			else {
				tramo5 = !tramo5;
				tramo6 = !tramo6;
			}
		}//*/

		if (tramo6) { //sexto tramo de la pista, avanza con la rotacion previa
			if (movCocheX > -58.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += movOffsetY * deltaTime;
				movCocheZ -= (movOffsetZ + 0.00002) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);
			}
			else {
				tramo6 = !tramo6;
				tramo7 = !tramo7;
			}
		}//*/

		if (tramo7) { //septimo tramo de la pista, rota un poco mas 
			if (movCocheX > -70.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.017) * deltaTime;
				movCocheZ -= (movOffsetZ + 0.06363) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.017), movOffsetX) * (180.0f / 3.141592f);
				if (giroI > -50) {
					giroI -= giroIOffset * deltaTime;
				}
			}
			else {
				tramo7 = !tramo7;
				tramo8 = !tramo8;
			}
		}//*/

		if (tramo8) { //octavo tramo de la pista, avanza con la rotacion previa
			if (movCocheX > -73.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.029) * deltaTime;
				movCocheZ -= (movOffsetZ + 0.06363) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);
			}
			else {
				tramo8 = !tramo8;
				tramo9 = !tramo9;
			}
		}//*/

		if (tramo9) { //noveno tramo de la pista, rota a la izq
			if (movCocheX > -75.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.029) * deltaTime;
				movCocheZ += (movOffsetZ + 0.00958) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;
					
				inclinacion = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);
				if (giroI < -35) {
					giroI += giroIOffset * deltaTime;
				}
			}
			else {
				tramo9 = !tramo9;
				tramo10 = !tramo10;
			}
		}//*/

		if (tramo10) { //decimo tramo de la pista, avanza con la rotacion previa
			if (movCocheX > -78.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.029) * deltaTime;
				movCocheZ -= (movOffsetZ + 0.00958) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);
			}
			else {
				tramo10 = !tramo10;
				tramo11 = !tramo11;
			}
		}//*/

		 if (tramo11) { //undecimo tramo de la pista, sigue rotando a la izq
			if (movCocheX > -80.0f)
			{
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.033) * deltaTime;
				movCocheZ += (movOffsetZ + 0.01026) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.033), movOffsetX) * (180.0f / 3.141592f);
				if (giroI < -10) {
					giroI += giroIOffset * deltaTime;
				}
			}
			else {
				tramo11 = !tramo11;
				tramo12 = !tramo12;
			}
		}//*/

		 if (tramo12) { //duodecimo tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -83.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.033) * deltaTime;
				 movCocheZ -= (movOffsetZ + 0.01026) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.033), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo12 = !tramo12;
				 tramo13 = !tramo13;
			 }
		 }//*/

		if (tramo13) { //13º tramo de la pista, sigue rotando a la izq
			 if (movCocheX > -90.0f)
			 {
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.035) * deltaTime;
				movCocheZ += (movOffsetZ - 0.00958) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.035), movOffsetX) * (180.0f / 3.141592f);
				if (giroI < 5) {
					 giroI += giroIOffset * deltaTime;
				}
			 }
			 else {
				 tramo13 = !tramo13;
				 tramo14 = !tramo14;
			 }
		 }//*/

		 if (tramo14) { //14º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -97.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.037) * deltaTime;
				 movCocheZ += (movOffsetZ - 0.00958) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.037), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo14 = !tramo14;
				 tramo15 = !tramo15;
			 }
		 }//*/

		 if (tramo15) { //15º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -107.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.045) * deltaTime;
				 movCocheZ += (movOffsetZ - 0.00958) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.045), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo15 = !tramo15;
				 tramo16 = !tramo16;
			 }
		 }//*/

		 if (tramo16) { //16º tramo de la pista, rota a la derecha
			 if (movCocheX > -112.0f)
			 {
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.045) * deltaTime;
				movCocheZ -= (movOffsetZ - 0.01874) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.045), movOffsetX) * (180.0f / 3.141592f);
				if (giroI > -5) {
					giroI -= giroIOffset * deltaTime;
				}
			 }
			 else {
				 tramo16 = !tramo16;
				 tramo17 = !tramo17;
			 }
		 }//*/

		 if (tramo17) { //17º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -117.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.030) * deltaTime;
				 movCocheZ -= (movOffsetZ - 0.01874) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.030), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo17 = !tramo17;
				 tramo18 = !tramo18;
			 }
		 }//*/

		 if (tramo18) { //18º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -125.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.023) * deltaTime;
				 movCocheZ -= (movOffsetZ - 0.01874) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.023), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo18 = !tramo18;
				 tramo19 = !tramo19;
			 }
		 }//*/

		 if (tramo19) { //19º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -130.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.018) * deltaTime;
				 movCocheZ -= (movOffsetZ - 0.01874) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.018), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo19 = !tramo19;
				 tramo20 = !tramo20;
			 }
		 }//*/

		 if (tramo20) { //20º tramo de la pista, rota a la derecha
			 if (movCocheX > -140.0f)
			 {
				movCocheX -= movOffsetX * deltaTime;
				movCocheY += (movOffsetY + 0.005) * deltaTime;
				movCocheZ -= (movOffsetZ - 0.00958) * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				inclinacion = atan2((movOffsetY + 0.005), movOffsetX) * (180.0f / 3.141592f);
				if (giroI > -20) {
					giroI -= giroIOffset * deltaTime;
				}
			 }
			 else {
				 tramo20 = !tramo20;
				 tramo21 = !tramo21;
			 }
		 }//*/

		 if (tramo21) { //21º tramo de la pista, avanza con la rotacion previa
			 if (movCocheX > -147.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY + 0.0) * deltaTime;
				 movCocheZ -= (movOffsetZ - 0.00958) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY + 0.0), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo21 = !tramo21;
				 tramo22 = !tramo22;
			 }
		 }//*/

		 if (tramo22) { //22º tramo de la pista, avanza con la rotacion previa, empezando a inclinarse en negativo
			 if (movCocheX > -157.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY - 0.005) * deltaTime;
				 movCocheZ -= (movOffsetZ - 0.00958) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY - 0.005), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo22 = !tramo22;
				 tramo23 = !tramo23;
			 }
		 }//*/

		 if (tramo23) { //23º tramo de la pista, avanza con la rotacion previa, inclinacion negativa y rota un poco mas
			 if (movCocheX > -162.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY - 0.0125) * deltaTime;
				 movCocheZ -= (movOffsetZ + 0.0025) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY - 0.0125), movOffsetX) * (180.0f / 3.141592f);
				 if (giroI > -25) {
					 giroI -= giroIOffset * deltaTime;
				 }
			 }
			 else {
				 tramo23 = !tramo23;
				 tramo24 = !tramo24;
			 }
		 }//*/

		 if (tramo24) { //24º tramo de la pista, avanza con la rotacion previa, inclinacion negativa 
			 if (movCocheX > -167.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY - 0.015) * deltaTime;
				 movCocheZ -= (movOffsetZ + 0.0025) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY - 0.015), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo24 = !tramo24;
				 tramo25 = !tramo25;
			 }
		 }//*/

		 if (tramo25) { //25º tramo de la pista, avanza con la rotacion previa, inclinacion negativa 
			 if (movCocheX > -182.0f)
			 {
				 movCocheX -= movOffsetX * deltaTime;
				 movCocheY += (movOffsetY - 0.015) * deltaTime;
				 movCocheZ -= (movOffsetZ + 0.0025) * deltaTime;
				 rotllanta += rotllantaOffset * deltaTime;

				 inclinacion = atan2((movOffsetY - 0.015), movOffsetX) * (180.0f / 3.141592f);
			 }
			 else {
				 tramo25 = !tramo25;
				 printf("Valor final de movCocheX: %.4f \t Valor final de movCocheY: %.4f \t Valor final de movCocheZ: %.4f + \t Valor final de inclinacion: %.4f \t Valor final de giroI: %.4f\n",
					 movCocheX, movCocheY, movCocheZ, inclinacion, giroI);
			 }
		 }//*/

		
		  //para la animacion de la nave
		if (avanzaNX) {
			if (movNaveX < -167.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet + 0.015) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.0025) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY - 0.015), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				avanzaNX = !avanzaNX; //cambio de estado de la bandera del movimiento en X
				Ntramo2 = !Ntramo2;
			}
		}//*/

		if (Ntramo2) {//segundo tramo de la nave, avanza con rotacion previa
			if (movNaveX < -162.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet + 0.015) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.0025) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY - 0.015), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo2 = !Ntramo2;
				Ntramo3 = !Ntramo3;
			}
		}//*/

		if (Ntramo3) {
			if (movNaveX < -157.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet + 0.0125) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.0025) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY - 0.0125), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo3 = !Ntramo3;
				Ntramo4 = !Ntramo4;
			}
		}//*/

		if (Ntramo4) {
			if (movNaveX < -147.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet + 0.005) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY - 0.005), movOffsetX) * (180.0f / 3.141592f);
				if (giroIN < -20) {
					giroIN += giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo4 = !Ntramo4;
				Ntramo5 = !Ntramo5;
			}
		}//*/

		if (Ntramo5) {
			if (movNaveX < -140.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet + 0.0) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.0), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo5 = !Ntramo5;
				Ntramo6 = !Ntramo6;
			}
		}//*/

		if (Ntramo6) {
			if (movNaveX < -130.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.005) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.005), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo6 = !Ntramo6;
				Ntramo7 = !Ntramo7;
			}
		}//*/

		if (Ntramo7) {
			if (movNaveX < -125.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.0018) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.01874) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.0018), movOffsetX) * (180.0f / 3.141592f);
				if (giroIN < 5) {
					giroIN += giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo7 = !Ntramo7;
				Ntramo8 = !Ntramo8;
			}
		}//*/

		if (Ntramo8) {
			if (movNaveX < -117.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.023) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.01874) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.023), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo8 = !Ntramo8;
				Ntramo9 = !Ntramo9;
			}
		}//*/

		if (Ntramo9) {
			if (movNaveX < -112.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.03) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.01874) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.03), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo9 = !Ntramo9;
				Ntramo10 = !Ntramo10;
			}
		}//*/

		if (Ntramo10) {
			if (movNaveX < -107.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.045) * deltaTime;
				movNaveZ += (movNaveZoffSet - 0.01874) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.045), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo10 = !Ntramo10;
				Ntramo11 = !Ntramo11;
			}
		}//*/

		if (Ntramo11) {
			if (movNaveX < -97.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.045) * deltaTime;
				movNaveZ -= (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.045), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo11 = !Ntramo11;
				Ntramo12 = !Ntramo12;
			}
		}//*/

		if (Ntramo12) {
			if (movNaveX < -90.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.037) * deltaTime;
				movNaveZ -= (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.037), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo12 = !Ntramo12;
				Ntramo13 = !Ntramo13;
			}
		}//*/

		if (Ntramo13) {
			if (movNaveX < -83.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.035) * deltaTime;
				movNaveZ -= (movNaveZoffSet - 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.035), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo13 = !Ntramo13;
				Ntramo14 = !Ntramo14;
			}
		}//*/

		if (Ntramo14) {
			if (movNaveX < -80.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.033) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.01026) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.033), movOffsetX) * (180.0f / 3.141592f);
				if (giroIN > -10) {
					giroIN -= giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo14 = !Ntramo14;
				Ntramo15 = !Ntramo15;
			}
		}//*/

		if (Ntramo15) {
			if (movNaveX < -78.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.033) * deltaTime;
				movNaveZ -= (movNaveZoffSet + 0.01026) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.033), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo15 = !Ntramo15;
				Ntramo16 = !Ntramo16;
			}
		}//*/

		if (Ntramo16) {
			if (movNaveX < -75.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.029) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);
				if (giroIN > -35) {
					giroIN -= giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo16 = !Ntramo16;
				Ntramo17 = !Ntramo17;
			}
		}//*/

		if (Ntramo17) {
			if (movNaveX < -73.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.029) * deltaTime;
				movNaveZ -= (movNaveZoffSet + 0.00958) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo17 = !Ntramo17;
				Ntramo18 = !Ntramo18;
			}
		}//*/

		if (Ntramo18) {
			if (movNaveX < -70.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.029) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.06363) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.029), movOffsetX) * (180.0f / 3.141592f);
				if (giroIN > -50) {
					giroIN -= giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo18 = !Ntramo18;
				Ntramo19 = !Ntramo19;
			}
		}//*/

		if (Ntramo19) {
			if (movNaveX < -58.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += (movNaveYoffSet - 0.017) * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.06363) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2((movOffsetY + 0.017), movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo19 = !Ntramo19;
				Ntramo20 = !Ntramo20;
			}
		}//*/

		if (Ntramo20) {
			if (movNaveX < -53.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += movNaveYoffSet * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.00002) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);
				if (giroIN < -5) {
					giroIN += giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo20 = !Ntramo20;
				Ntramo21 = !Ntramo21;
			}
		}//*/

		if (Ntramo21) {
			if (movNaveX < -48.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += movNaveYoffSet * deltaTime;
				movNaveZ += (movNaveZoffSet + 0.00002) * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo21 = !Ntramo21;
				Ntramo22 = !Ntramo22;
			}
		}//*/

		if (Ntramo22) {
			if (movNaveX < -35.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY += movNaveYoffSet * deltaTime;
				movNaveZ -= movNaveZoffSet * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				inclinacionN = atan2(movOffsetY, movOffsetX) * (180.0f / 3.141592f);
				if (giroIN < 15) {
					giroIN += giroIOffsetN * deltaTime;
				}

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo22 = !Ntramo22;
				Ntramo23 = !Ntramo23;
			}
		}//*/

		if (Ntramo23) {
			if (movNaveX < -30.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveZ -= movNaveZoffSet * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo23 = !Ntramo23;
				Ntramo24 = !Ntramo24;
			}
		}//*/

		if (Ntramo24) {
			if (movNaveX < -20.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveZ -= movNaveZoffSet * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo24 = !Ntramo24;
				Ntramo25 = !Ntramo25;
			}
		}//*/

		if (Ntramo25) {
			if (movNaveX < -18.5f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo25 = !Ntramo25;
				Ntramo26 = !Ntramo26;
			}
		}//*/

		if (Ntramo26) {
			if (movNaveX < 0.0f) {//la nave va hacia delante X positiva
				movNaveX += movNaveXoffSet * deltaTime;
				movNaveY -= 0.1 * deltaTime;
				rotHelice += rotHeliceOffset * deltaTime;

				if (avanzaA) { //aleta, gira una cierta cantidad de grados
					if (aleta < 45) {
						aleta += aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
				else {
					if (aleta > 0) { //aleta, gira una cierta cantidad de grados
						aleta -= aletaOffset * deltaTime;
					}
					else {
						avanzaA = !avanzaA;
					}
				}
			}
			else {
				Ntramo26 = !Ntramo26;
				//printf("Valor final de movNaveX: %.4f \t Valor final de movNaveY: %.4f \t Valor final de movNaveZ: %.4f + \t Valor final de inclinacion: %.4f \t Valor final de giroI: %.4f\n",
					//movNaveX, movNaveY, movNaveZ, inclinacionN, giroIN);
			}
		}//*/

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();

		//Instancia del coche, Cuerpo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCocheX - 0.0f, movCocheY - 0.8f, movCocheZ + 2.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroI * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinacion), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		//Llanta tracera derecha - 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.35f, 0.9f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta tracera izquierda - 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.35f, -0.88f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha - 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.68f, -0.35f, 0.9f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera izquierda - 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.68f, -0.35f, -0.88f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();//*/

		//Parabrisas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.90f, 0.58f, -0.053f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Parabrisas_M.RenderModel();//*/

		//Cajuela
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, 0.4f, -0.07f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cajuela_M.RenderModel();//*/

		//Parrilla
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.4f, 0.07f, 0.03f));

		//ligamos la luz al modelo de la parrilla de forma jerarquica
		poslocalLuzCarro = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //posicion local de la luz, en el origen de la parrilla
		posxmodelLuzCarro = glm::vec3(model * poslocalLuzCarro); //multipliacion del vector de la luz por la matriz model (contiene jerarquicamente todas las TG del modelo)
		dirLuzCarro = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f); //direccion de la luz, en sentido de X positiva
		dirxmodelLuzCarro = glm::vec3(model * dirLuzCarro); //multipliacion del vector de la luz por la matriz model (contiene jerarquicamente todas las TG del modelo)
		spotLights[1].SetFlash(posxmodelLuzCarro, dirxmodelLuzCarro); //liga la posicion y la direccion a la luz

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Parrilla_M.RenderModel();//*/

		//nave, por default su orientacion es hacia X negativa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movNaveX - 0.0, movNaveY -0.0f, movNaveZ + 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroIN * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(inclinacionN), glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxN = model;

		//ligamos la luz al modelo de la nave de forma jerarquica
		poslocalLuzN = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		posxmodelLuzN = glm::vec3(model * poslocalLuzN);
		dirLuzN = glm::vec4(-0.75f, -1.0f, 0.0f, 0.0f);
		dirxmodelLuzN = glm::vec3(model * dirLuzN);
		spotLights[2].SetFlash(posxmodelLuzN, dirxmodelLuzN);

		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_M.RenderModel();

		//aleta derecha
		model = modelauxN;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::rotate(model, -aleta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();//*/

		//aleta izq
		model = modelauxN;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, +aleta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaI_M.RenderModel();//*/

		//helice derecha
		model = modelauxN;
		model = glm::translate(model, glm::vec3(-0.746f, -0.602f, -0.7094f));
		model = glm::rotate(model, -rotHelice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();//*/

		//helice izq
		model = modelauxN;
		model = glm::translate(model, glm::vec3(-0.746f, -0.602f, 0.7094f));
		model = glm::rotate(model, -rotHelice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();//*/


		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}