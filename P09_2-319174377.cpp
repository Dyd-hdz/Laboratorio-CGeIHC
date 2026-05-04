/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

float toffsetfuegou = 0.0f;
float toffsetfuegov = 0.0f;

float animacionX = 0.0f;
float animacionY = 0.0f;
float animacionZ = 0.0f;
float timerAnimacion = 0.0f;

float animacionHX = 0.0f;
float animacionHY = 0.0f;
float animacionHZ = 0.0f;
float timerAnimacionHumo = 0.0f;
float incrementoHX = 0.0f;

float timerEspera = 0.0f;
bool bandera = 0;//para activar/desactivar la textura del fuego
bool banderaA = 0;//para activar/dedsactivar el giro de la esfera y la textura del humo
bool banderaT = 0;//para activar/desactivar el timer de espera
bool banderaH = 0;//para activar/descativar la textura del humo
bool banderaI = 0;//para activar desactivar el timer del humo
bool banderaB = 0;//para que empiece la rotacion del brazo

float giroEsfera;//acumulador
float giroEsferaOffset;//incremento

float rotBrazo;
float rotBrazoOffset;

//variables para la animacion de tiro parabolico
bool esferaActiva = false;//bandera para saber que la esfera ya fue lanzada
glm::vec3 posEsfera = glm::vec3(17.57f, 3.7f, 1.5f);//posicion de la esfera cuando el brazo ya esta rotado en 95º
glm::vec3 velEsfera = glm::vec3(0.0f, 0.0f, 0.0f);//la velocidad
float gravedad = 9.81f;//constante de la gravedad
float coefRestitucion = 0.7f;//constante e
int contadorRebotes = 0;//contador de rebotes
int maxRebotes = 5;//el numero maximo de robotes
bool yaDisparado = false;//para que el inicio de la animacion de la esfera solo se haga una sola vez


int numero1 = 0;
float timer = 0; //variable que acumula el tiempo

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

//variable para la textura del fuego
Texture fuego;
Texture humo;

Model Kitt_M;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model Aeolipile_base_M;
Model Aeolipile_M;

//modelos de la catapulta y esfera
Model Catapulta_Cuerpo;
Model Catapulta_Brazo;
Model Catapulta_Esfera;

//modelos de engranajes
Model Engranaje1_M;
Model Engranaje2_M;

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

//funcion de la colicion con el piso
void actualizarFisicaEsfera(float dt) {
	if (!esferaActiva) return;

	velEsfera.y -= gravedad * dt;//aplicamos la gravedad al eje vertical

	posEsfera += velEsfera * dt;//actualizamos la posicion segun la velocidad

	//para ver si la esfera ya esta al nivel del piso
	if (posEsfera.y <= -1.5f) {
		if (contadorRebotes < maxRebotes) {//para ver si aun no se han dado los rebotes definidos (5)
			posEsfera.y = -1.5f;//posiciona directamente en el piso, para que no se hunda la esfera
			velEsfera.y = -velEsfera.y * coefRestitucion;//la esfera sube por el signo - de velEsfera, coefRestitucion (e)
			velEsfera.x *= 0.9f;//friccion para que se detenga gradualmente
			contadorRebotes++;
		}
		else {//si ya se dieron los 5 rebotes
			posEsfera.y = -1.5f;//se mantiene la posicion en el piso
			velEsfera = glm::vec3(0.0f);//la velocidad es 0
			esferaActiva = false;//desactivamos la bandera del inicio de la animacion del rebote
		}
	}
}

//funcion lanzar esfera
void lanzarEsfera() {
	esferaActiva = true;
	posEsfera = glm::vec3(17.57f, 3.7f, 1.5f);//estas son las coordenadas donde llega la esfera despues de
	//la rotacion de 95º del brazo, de aqui parte la animacion del tiroparabolico y rebote
	contadorRebotes = 0;//como va a comenzar, se inicializa a 0 el contador de rebotes

	//velocidad y angulo inicial del lanzamiento
	float velocidadInicial = 8.0f;
	float anguloLanzamiento = 45.0f * toRadians;

	//descomposicion vectorial
	velEsfera.x = velocidadInicial * cos(anguloLanzamiento); //Vx = V0 *  cos(theta)
	velEsfera.y = velocidadInicial * sin(anguloLanzamiento); //Vy = V0 * seno(theta)
	velEsfera.z = 0.0f;
}

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

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

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

	//carga de la textura fuego
	//fuego = Texture("Textures/fuego_editado.tga");
	fuego = Texture("Textures/fuego_practica_editado.tga");
	fuego.LoadTextureA();
	humo = Texture("Textures/humo_practica_editado.tga");
	humo.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");

	//carga del modelo .obj de la catapulta
	Catapulta_Cuerpo = Model();
	Catapulta_Cuerpo.LoadModel("Models/catapulta_cuerpo.obj");
	Catapulta_Brazo = Model();
	Catapulta_Brazo.LoadModel("Models/catapulta_brazo.obj");
	Catapulta_Esfera = Model();
	Catapulta_Esfera.LoadModel("Models/catapulta_esfera.obj");

	//maquina steampunk
	Engranaje1_M = Model();
	Engranaje1_M.LoadModel("Models/engranaje_1.obj");
	Engranaje2_M = Model();
	Engranaje2_M.LoadModel("Models/engranaje_2.obj");

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

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxH(1.0);
	glm::mat4 modelauxC(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


	giroEsfera = 0.0f;
	giroEsferaOffset = 5.0f;

	rotBrazo = 0.0f;
	rotBrazoOffset = 10.0f;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Justo después de calcular el deltaTime
		actualizarFisicaEsfera(deltaTime * 0.1f); // El 0.1f es para ajustar la escala del tiempo

		angulovaria += 0.5f*deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}
	
		if (banderaB) {//para la rotacion del brazo
			if (rotBrazo < 95.0f) {//si la angulo es menor a 95º
				rotBrazo += rotBrazoOffset * deltaTime;//se incrementa
			}
			else {//si el angulo es mayor a 95º (ya llego a su posicion inicial para que inicie la animacion del
				//tiro parabolico de la esfera)
				if (!yaDisparado) {//para que se lance una sola vez la esfera
					lanzarEsfera();//activamos animacion del tiro parabolico de la esfera
					yaDisparado = true; //evitamos que se repitan los disparos
				}
				banderaB = 0;//desactivamos la bandera de la rotacion del brazo
			}
		}

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

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


		//reiniciar variables antes de que sean enviadas al shader

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



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Aquí va la nave con jerarquía de modelos, completar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		
		//timer de espera, el tiempo que tiene que esperar para que se prenda el fuego y despues, que empiece a girar la esfera
		if (mainWindow.getsKeys()[GLFW_KEY_R]) {
			banderaT = 1;//para activar el timer general
			bandera = 1;//activa la animacion del fuego

			//al iniciar la animacion general, se resetean todos los valores de la animacion del tiro parabolico
			//para que se haga dicha animacion en cada reinicio de la animacion general
			esferaActiva = false;
			yaDisparado = false;
			contadorRebotes = 0;     
			rotBrazo = 0.0f;         
			banderaB = 0;
		}

		if (banderaT) {//activa el timer general
			timerEspera++;
		}

		//de 0 a 1500 se solo se activa el fuego
		
		//empieza a girar el aelopile y sale el humo, el humo va incremetandose
		if (timerEspera >= 1500)//de 1500 a 3000 se activa:
		{
			banderaA = 1;//el giro del aelopile
			banderaI = 1;//el timer del humo
		}

		//3000 es el punto maximo del humo del aelopile
		//se apaga el fuego, deja de girar el aelopile y el humo empieza a decreser
		if (timerEspera > 3000) {//de 3000  a 4500 se:
			//banderaT = 0;
			bandera = 0;//desactiva el fuego
			banderaA = 0;//desactiva el giro del aelopile
			banderaH = 1;//para que se empiece a ver el humo
			banderaB = 1;
			//timerEspera = 0;
		}

		//se apaga todo, se reinicia los estados
		if (timerEspera > 4500) {//despues de 4500 se desactiva:
			banderaH = 0;//que se pueda ver el humo
			banderaT = 0;//el timer general
			banderaI = 0;//el timer del humo
			timerEspera = 0;//se reinicia el timer general
			banderaB = 0;
			rotBrazo = 0.0f;
		}
		
		
		//AEOLIPILE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_base_M.RenderModel();

		//para la animacion de la esfera
		if (banderaA) {//para que empiece a girar la esfera en un determinado tiempo, despues del fuego prenda
			giroEsfera += giroEsferaOffset * deltaTime;//incremento
		}
		//giroEsfera += giroEsferaOffset * deltaTime;//incremento

		//esfera
		model = glm::translate(model, glm::vec3(0.15f, 5.0f, -0.1f));
		model = glm::rotate(model, giroEsfera * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));//gira la esfera
		modelauxH = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aeolipile_M.RenderModel();

		//Catapulta_cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.0f, 0.5f, 1.5f));
		modelauxC = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Catapulta_Cuerpo.RenderModel();

		//Catapulta_brazo
		model = modelauxC;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -rotBrazo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -95 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxC = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Catapulta_Brazo.RenderModel();

		//Catapulta_esfera
		if (!esferaActiva and rotBrazo < 95.0f) {//si la bandera de la esta desactivada y si la rotacion es menor a 95º
			//es decir este se activa cuando la esfera esta en su posicion inicial y hasta cuando el brazo este rotando
			model = modelauxC;//usa las TG del modelo padre (del cuerpo y del brazo)
			model = glm::translate(model, glm::vec3(-3.25f, 0.5f, 0.0f));//posicion de la esfera, la "cuchara" del brazo
		}
		else {//cuando ya se activo la bandera de la esfera y el rotacion es mayor a 95º
			model = glm::mat4(1.0);//se separa la esfera de los otros modelos, ya no esta jerarquizado
			model = glm::translate(model, posEsfera);//la posicion ahora sera calculada por las funciones
			//acualizarFisicaEsfera y lanzarEsfera
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Catapulta_Esfera.RenderModel();

		/*Añadir para ejercicio
		1.- Agregar que el número cambiante sea a una velocidad visible.
		Acomodar jerarquía del Aelopile
		2.- Giro del Aelopile
		3.-Fuego
			- Editar Textura para el fuego
			- Blending para el fuego
			- Animación del fuego
		
		Añadir para el reporte
		1.- Humo
			- Editar Textura para el humo
			- Blending para el humo
			- Animación del humo: no sale y sale
		2.- Brazo/palanca que lance la esfera metálica
			-El humo inicia el movimiento del brazo palanca
			- Animación del brazo
		3.- Esfera metálica
			- Movimiento de la esfera metálica por medio de física básica: tiro parabólico y rebote
		*/

		//Modelos con blending al final para que no afecten a los demás objetos, aunque también se pueden renderizar al inicio pero con blending  activado y desactivado


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
		
		//textura con movimiento de las flechas
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		
		//planos para los 8 fuegos, 0º, 45º, 90º, 135º, 180º, 225º, 270º y 315º
		//textura con movimiento del fuego
		toffsetfuegou += 0.000;//incremento, para darle movimiento a la textura
		toffsetfuegov = 0.06835;

		
		//para que no se desborde la variable
		if (toffsetfuegou > 1.0)
			toffsetfuegou = 0.0;
		//if (toffsetv > 1.0)
			//toffsetv = 0;
		
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetfuegou, toffsetfuegov);

		/*if (toffsetfuegou > 0.0 and toffsetfuegou < 0.23243) { //en un rango de la textura
			toffset = glm::vec2(toffsetfuegou, toffsetfuegov);
		}
		else {
			toffsetfuegou = 0.0;
		}//*/

		//para la animacion de la textura, para hacer mas grande y pequeño la textura
		//las variables seran pasadas a la TG scale para hacer mas grande y mas chica la imagen en un cierto rango de tiempo
		timerAnimacion++;
		if (timerAnimacion < 100)//si tamer es mayor o igual a 50
		{
			animacionX += 0.005f; //incrementamos estas variables para darle animacion a la textura de fuego
			animacionY += 0.005f;
			animacionZ += 0.005f;
			//timerAnimacion = 0;//reinicio del timer
		}
		if (timerAnimacion < 200 and timerAnimacion > 100) {
			animacionX -= 0.005f; //decrementamos estas variables para darle animacion a la textura de fuego
			animacionY -= 0.005f;
			animacionZ -= 0.005f;
		}
		if (timerAnimacion > 200) {//para reiniciar los parametros
			animacionX = 0.0f;
			animacionY = 0.0f;
			animacionZ = 0.0f;
			timerAnimacion = 0.0f; //reinicio del timer
		}

		//para el fuego de 0º
		model = glm::mat4(1.0);
		//10.8f, -0.2f, 1.5f
		model = glm::translate(model, glm::vec3(10.2f, -0.35f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f + animacionX, 1.5f + animacionY, 1.5f + animacionZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		if (bandera) {//despues del tiempo de espera, se activa la textura
			fuego.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();//*/
		}
		//fuego.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();//*/

		//para el fuego de 90º
		model = glm::mat4(1.0);
		//10.0f, -0.2f, 2.3f
		model = glm::translate(model, glm::vec3(10.2f, -0.35f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f + animacionX, 1.5f + animacionY, 1.5f + animacionZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		if (bandera) {//despues del tiempo de espera, se activa la textura
			fuego.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
		}//*/
		//fuego.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();//*/

		
		if (banderaI) {//para que empiece el contador, cuando el aelopile empiece a girar
			timerAnimacionHumo++;
		}

		//de 0 a 1500
		if (timerAnimacionHumo < 1500 and banderaI)//incrementa el tiempo igual cuando esta prendido el fuego
		{
			incrementoHX += 0.00045f;
			animacionHX += 0.0009f;
			animacionHY += 0.0009f;
			animacionHZ += 0.0009f;
		}
		
		//de 1500 a 3000
		if (timerAnimacionHumo > 1500 and timerAnimacionHumo <= 3000) {//decrementra el tiempo igual despues de que se apago el fuego
			incrementoHX -= 0.00045f;
			animacionHX -= 0.0009f;
			animacionHY -= 0.0009f;
			animacionHZ -= 0.0009f;
		}

		if (timerAnimacionHumo > 3000) {//para reiniciar los parametros
			incrementoHX = 0.0f;
			animacionHX = 0.0f;
			animacionHY = 0.0f;
			animacionHZ = 0.0f;
			timerAnimacionHumo = 0.0f; //reinicio del timer
		}
		
		//para el humo, tubo superior
		model = modelauxH;
		model = glm::translate(model, glm::vec3(0.18f + incrementoHX, 0.62f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f + animacionHX, 0.1f + animacionHY, 0.1f + animacionHZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//para que el humo empiece a salir cuando empiece a girar el aelopile
		if (banderaA or banderaH) {
			humo.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
		}//*/
		/*humo.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();//*/

		//para el humo, tubo superior rotado 90º en X
		model = modelauxH;
		model = glm::translate(model, glm::vec3(0.18f + incrementoHX, 0.62f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f + animacionHX, 0.1f + animacionHY, 0.1f + animacionHZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		if (banderaA or banderaH) {
			humo.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
		}//*/
		//humo.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();

		//para el humo, tubo inferior
		model = modelauxH;
		model = glm::translate(model, glm::vec3(-0.175f - incrementoHX, -0.57f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f + animacionHX, 0.1f + animacionHY, 0.1f + animacionHZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		if (banderaA or banderaH) {
			humo.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
		}//*/
		/*humo.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();//*/

		//para el humo, tubo inferior rotado 90º en X
		model = modelauxH;
		model = glm::translate(model, glm::vec3(-0.175f - incrementoHX, -0.57f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f + animacionHX, 0.1f + animacionHY, 0.1f + animacionHZ));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		
		if (banderaA or banderaH) {
			humo.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[4]->RenderMesh();
		}
		//humo.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();
		

		//número cambiante 1-4
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		
		timer++;

		if (timer >= 100)//si tamer es mayor o igual a 100
		{
			toffsetnumerocambiau += 0.25f; //cambio de numero
			timer = 0;//reinicio del timer

			if (toffsetnumerocambiau >= 1.0f)//si el offset es mayor a 1 (4), se reinicia el offset para el 1
			{
				toffsetnumerocambiau = 0.0f;
			}
		}
		
		//toffsetnumerocambiau += 0.25;
		//if (toffsetnumerocambiau > 1.0)
			//toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();//*/

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();//*/
		
		/*if (numero1 < 100) {
			Numero1Texture.UseTexture();
			numero1++;
		}
		if(numero1 >= 100 ) {
			Numero2Texture.UseTexture();
			numero1++;
		}
		if (numero1 > 200) {
			numero1 = 0;
		}//*/
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();//*/


		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
