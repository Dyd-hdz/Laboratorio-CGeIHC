/*
Práctica 8: Iluminación 2
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

//para prender-apagar luces
bool prender = true;//variable para saber si la lampara esta prendida o apagada
bool presionarL = false;//para evitar la intermitencia de la luz, que al presionar la tecla
//lea un solo cambio de estado

//para prender la luz del carro
int indiceLuz = 0;//variable para el indice de la luz del carro, ya que cada indice es un color diferente 
bool presionarC = false;//para evitar la intermitencia de la luz, que al presionar la tecla
//lea un solo cambio de estado

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

//para las texturas del dado
Texture dadoOctTexture;

Model Kitt_M;
//Model Llanta_M;
Model Blackhawk_M;

//modelos del carro
Model Cuerpo_M;
Model Llanta_M;
Model Parabrisas_M;
Model Cajuela_M;
Model Parrilla_M;

//modelo de la lampara
Model Lampara_M;

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


//función de calculo de normales por promedio de vértices 
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,


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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};

	GLfloat cubo_vertices[] = {
		// front, de frente
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right, derecho
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.5f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.74f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.74f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.5f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back, atras
		-0.5f, -0.5f, -0.5f,	0.76f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.99f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.99f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.76f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left, izquierdo
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.01f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.24f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.24f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.01f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom, base
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.74f,  0.33f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.5f,	0.33f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.5f,	0.01f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.74f,	0.01f,		0.0f,	1.0f,	0.0f,

		//UP, tapa, parte de arriba
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.5f,  0.67f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.74f,	0.67f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.74f,	0.99f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.5f,	0.99f,		0.0f,	-1.0f,	0.0f,
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		//parte de arriba
		// front
		0, 1, 2,

		// right
		3, 4, 5,

		// back
		6, 7, 8,

		// left
		9, 10, 11,

		//parte de abajo
		// front
		13, 12, 14,

		// right
		16, 15, 17,

		// back
		19, 18, 20,

		// left
		22, 21, 23
	};
	GLfloat octaedro_vertices[] = {
		//caras de la figura

		//parte de arriba del octaedro
		// front, de frente
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, 0.0f,  0.5f,		0.34f,  0.6f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, 0.0f,  0.5f,		0.66f,	0.41f,		0.0f,	0.0f,	-1.0f,	//1
		0.0f,  0.5f,  0.0f,		0.66f,	0.79f,		0.0f,	0.0f,	-1.0f,	//2

		// right, derecho
		//x		y		z		S		T
		0.5f, 0.0f,  0.5f,	    0.67f,  0.41f,		-1.0f,	0.0f,	0.0f, //3
		0.5f, 0.0f,  -0.5f,		0.99f,	0.6f,		-1.0f,	0.0f,	0.0f, //4
		0.0f,  0.5f,  0.0f,		0.67f,	0.79f,		-1.0f,	0.0f,	0.0f, //5

		// back, atras
		0.5f, 0.0f,  -0.5f,		0.99f,  0.61f,		0.0f,	0.0f,	1.0f, //6
		-0.5f, 0.0f,  -0.5f,	0.99f,	0.99f,		0.0f,	0.0f,	1.0f, //7
		0.0f,  0.5f,  0.0f,		0.67f,	0.8f,		0.0f,	0.0f,	1.0f, //8

		// left, izquierdo
		//x		y		z		S		T
		-0.5f, 0.0f,  -0.5f,	0.34f,  0.99f,		1.0f,	0.0f,	0.0f, //9
		-0.5f, 0.0f,  0.5f,		0.34f,	0.61f,		1.0f,	0.0f,	0.0f, //10
		0.0f,  0.5f,  0.0f,		0.66f,	0.8f,		1.0f,	0.0f,	0.0f, //11

		//parte de abajo del octaedro
		// front, de frente
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, 0.0f,  0.5f,		0.66f,  0.4f,		0.0f,	0.0f,	-1.0f,	//12
		0.5f, 0.0f,  0.5f,		0.34f,	0.59f,		0.0f,	0.0f,	-1.0f,	//13
		0.0f,  -0.5f,  0.0f,	0.34f,	0.21f,		0.0f,	0.0f,	-1.0f,	//14

		// right, derecho
		//x		y		z		S		T
		0.5f, 0.0f,  0.5f,	    0.66f,  0.0f,		-1.0f,	0.0f,	0.0f, //15
		0.5f, 0.0f,  -0.5f,		0.66f,	0.39f,		-1.0f,	0.0f,	0.0f, //16
		0.0f,  -0.5f,  0.0f,	0.34f,	0.2f,		-1.0f,	0.0f,	0.0f, //17

		// back, atras
		0.5f, 0.0f,  -0.5f,		0.01f,  0.39f,		0.0f,	0.0f,	1.0f, //18
		-0.5f, 0.0f,  -0.5f,	0.01f,	0.01f,		0.0f,	0.0f,	1.0f, //19
		0.0f,  -0.5f,  0.0f,	0.32f,	0.19f,		0.0f,	0.0f,	1.0f, //20

		// left, izquierdo
		//x		y		z		S		T
		-0.5f, 0.0f,  -0.5f,	0.33f,  0.59f,		1.0f,	0.0f,	0.0f, //21
		-0.5f, 0.0f,  0.5f,		0.01f,	0.4f,		1.0f,	0.0f,	0.0f, //22
		0.0f,  -0.5f,  0.0f,	0.33f,	0.2f,		1.0f,	0.0f,	0.0f, //23
	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);
}


int main()
{
	GLfloat valor;
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearOctaedro();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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

	//carga de la textura para el dado octaedro
	dadoOctTexture = Texture("Textures/dado_octaedro_editado.jpg");
	dadoOctTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	//Llanta_M = Model();
	//Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

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

	//cargado del archivo del modelo .obj del poste
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");

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
		0.4f, 0.3f, //intencidad ambiental (radiacion del sol), intensidad difusa
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.2f, 0.2f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;//*/

	//Declaración de luz puntual, para la lampara
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.2f, 0.2f,
		-3.0f, 4.5f, 6.0, //posicion del "foco" de la lampara
		0.3f, 0.2f, 0.1f);
	pointLightCount++;//*/

	
	//contador de luces spotlight
	unsigned int spotLightCount = 0;
	
	//linterna
	/*spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;//*/

	//luz para el carro - rojo
	spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f, //rojo
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, //posicion temporal
		1.0f, 0.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/

	//luz para el carro - amarillo
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f, //amarillo
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, //posicion temporal
		1.0f, 0.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/

	//luz para el carro - verde
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f, //verde
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  //posicion temporal
		1.0f, 0.0f, 0.0f,  //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/

	//luz para el carro - cian
	spotLights[3] = SpotLight(0.0f, 1.0f, 1.0f, //cian
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  //posicion temporal
		1.0f, 0.0f, 0.0f,  //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/

	//luz para el carro - azul
	spotLights[4] = SpotLight(0.0f, 0.0f, 1.0f, //azul
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  //posicion temporal
		1.0f, 0.0f, 0.0f,  //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/

	//luz para el carro - magenta
	spotLights[5] = SpotLight(1.0f, 0.0f, 1.0f, //magenta
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  //posicion temporal
		1.0f, 0.0f, 0.0f,  //direccion
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;//*/


	//se crean mas luces puntuales y spotlight
	glm::vec3 lowerLight;
	glm::vec3 posLuzCarro;
	glm::vec3 dirLuzCarro;
	glm::vec3 posLuzHel;
	glm::vec3 dirLuzHel;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		/*lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());//*/

		//Para las 6 luces ligadas al carro
		// para la luz del carro, enfrente - luz roja
		posLuzCarro = glm::vec3(7.4f + mainWindow.getmuevex(), 0.17f, -0.453f);
		// la posicion de la parrilla, desde el origen, sin tomar encuenta las traslacion del modelo 
		// padre
		dirLuzCarro = glm::vec3(1.0f, 0.0f, 0.0f); // direccion hacia donde avanza el carro
		spotLights[0].SetFlash(posLuzCarro, dirLuzCarro);

		// para la luz del carro, enfrente - luz amarilla
		spotLights[1].SetFlash(posLuzCarro, dirLuzCarro);//*/

		// para la luz del carro, enfrente - luz verde
		spotLights[2].SetFlash(posLuzCarro, dirLuzCarro);//*/

		// para la luz del carro, enfrente - luz cian
		spotLights[3].SetFlash(posLuzCarro, dirLuzCarro);//*/

		// para la luz del carro, enfrente - luz azul
		spotLights[4].SetFlash(posLuzCarro, dirLuzCarro);//*/

		// para la luz del carro, enfrente - luz magenta
		spotLights[5].SetFlash(posLuzCarro, dirLuzCarro);//*/

		// para detectar el encender-apagar o apagar-encender de la luz
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {//al presionar la tecla P:
			//para que el cambio se haga una sola vez al presionar E
			if (!presionarL) { //al presionar por primera vez
				prender = !prender; //cambia el estado de la luz
				presionarL = true; //bloquea el estado hasta que se suelte la tecla
				//evita la intermitencia de los estados
			}
		}
		else {
			presionarL = false;//reinicio de la variable/bandera de presionar
		}

		
		// para detectar el cambio de luz del carro cuando se presione E
		if (mainWindow.getsKeys()[GLFW_KEY_E]) { //al presionar la tecla E:
			if (!presionarC) {//al presionar por primera vez
				indiceLuz++;//cambia el indice, para una luz diferente
				if (indiceLuz > 5) {//si el indice es 5 (color magenta) 
					indiceLuz = 0;//se reinicia el indice a 0 (color rojo)
				}
				presionarC = true;//bloquea el estado hasta que se suelte la tecla
				//evita la intermitencia de los estados
			}
		}
		else {
			presionarC = false;//reinicio de la variable/bandera de presionar
		}
		
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);//shader para la luz direcional

		//shader para la luz puntual
		if (prender == true) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);//todas las luces puntuales
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 1);//sin la luz de la lampara
		}

		
		SpotLight lucesAux[1];//creamos un arreglo auxiliar para todas las luces spotlihgts
		//este arreglo solo contendra una luz a la vez
		//el arreglo auxiliar en su unico indice 0 (la primer luz), le pasaremos la informacion
		//de la luz de acuerdo a la variable indiceLuz
		//es decir, la informacion de cada luz estara en el indice 0 del arreglo auxiliar
		//una a la vez
		lucesAux[0] = spotLights[indiceLuz];
		//indiceLuz es lo que cambiara, por tanto las luces se iran turnando, cuando indiceLuz
		//sea 0 la informacion del arreglo auxiliar en su indice 0 es la de la luz roja
		//cuando sea 1 la informacion del arreglo auxiliar en su indice 0 es la luz amarilla 
		//y asi sucesivamente

		//el shader solo trabajara con el indice 0 del arreglo auxiliar
		shaderList[0].SetSpotLights(lucesAux, 1);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//dado octaedro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoOctTexture.UseTexture();
		meshList[5]->RenderMesh();//*/
		
		//Instancia del carro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f + mainWindow.getmuevex(), 0.1f, -0.5f));
		//7.4f + mainWindow.getmuevex(), 0.17f, -1.453f

		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		modelaux = model;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.7f, -0.4f, 0.9f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();//*/

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.7f, -0.4f, -0.88f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();//*/

		//Llanta tracera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.4f, 0.9f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();//*/

		//Llanta tracera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.15f, -0.4f, -0.88f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();//*/

		//Parabrisas
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.90f, 0.58f, -0.053f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Parabrisas_M.RenderModel();//*/

		//Cajuela
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, 0.4f, -0.078f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cajuela_M.RenderModel();//*/

		//Parrilla
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.4f, 0.07f, 0.047f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Parrilla_M.RenderModel();//*/

		//Elicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevexE(), 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[4]->RenderMesh();

		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.98f, 6.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
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
