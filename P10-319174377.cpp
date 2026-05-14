/*
Animación por Keyframes
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

//para la nave
float rotHelice;//para que la helice giren
float rotHeliceOffset;//incremento de la variable
float aleta; //para mover la aleta, girara en Y
float aletaOffset; //incremento de la aleta//*/
bool bandera = 0;

//para que giren los engranajes
/*float rotEngranaje;
float rotEngranajeOffset;
float rotEngranaje2;
float rotEngranaje2Offset;
float rotEngranaje3;
float rotEngranaje3Offset;
float rotEngranaje4;
float rotEngranaje4Offset;
float rotEngranaje5;
float rotEngranaje5Offset;//*/

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
Model Llanta_M;
Model Pista_M;
//Model Nave_M;
//Model Ala_M;
Model Aeolipile_base_M;
Model Aeolipile_M;

//modelos de la nave
Model Nave_M;
Model Ala_M;
Model AlaI_M;
Model Helice_M;

//maquina steampunk
Model Engranaje1_M;
Model Engranaje2_M;
Model ManecillaH_M;
Model ManecillaV_M;
Model ArcoG_M;
Model ArcoN_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//INPUT DE KEYFRAMES
void inputKeyframes(bool* keys);
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
float ciclo3, ciclo4, ciclo5, ciclo6, ciclo7, ciclo8, ciclo9, ciclo10;

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
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



///////////////////////////////KEYFRAMES/////////////////////
bool animacion = false;

//NEW// Keyframes
float posXavion = 0.0, posYavion = 2.0, posZavion = 0;//colocar aqui la posicion de la nave
float	movAvion_x = 0.0f, movAvion_y = 0.0f;//son las variables acumulativas
float giroAvion = 0;//es la variable acumulativa


//variables acumulativas
float rotEngranaje = 0.0f;
float rotEngranaje2 = 0.0f;
float rotEngranaje3 = 0.0f;
float rotEngranaje4 = 0.0f;
float rotEngranaje5 = 0.0f;

#define MAX_FRAMES 100//este numero sea un colchon aproximado a numero de frames
int i_max_steps = 90;//afecta la velocidad de la animacion
int i_curr_steps = 30;//el numero de frames
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX, variables secundaria
	float movAvion_yInc;		//Variable para IncrementoY, variable secundaria
	float giroAvion;
	float giroAvionInc;

	//variables acumulativas
	float rotEngranaje;
	float rotEngranaje2;
	float rotEngranaje3;
	float rotEngranaje4;
	float rotEngranaje5;

	//variables de incrementos
	float rotEngranajeOffset;
	float rotEngranaje2Offset;
	float rotEngranaje3Offset;
	float rotEngranaje4Offset;
	float rotEngranaje5Offset;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;//para que las rotaciones se acumulen

	KeyFrame[FrameIndex].rotEngranaje = rotEngranaje;
	KeyFrame[FrameIndex].rotEngranaje2 = rotEngranaje2;
	KeyFrame[FrameIndex].rotEngranaje3 = rotEngranaje3;
	KeyFrame[FrameIndex].rotEngranaje4 = rotEngranaje4;
	KeyFrame[FrameIndex].rotEngranaje5 = rotEngranaje5;
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;

	rotEngranaje = KeyFrame[0].rotEngranaje;
	rotEngranaje2 = KeyFrame[0].rotEngranaje2;
	rotEngranaje3 = KeyFrame[0].rotEngranaje3;
	rotEngranaje4 = KeyFrame[0].rotEngranaje4;
	rotEngranaje5 = KeyFrame[0].rotEngranaje5;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

	KeyFrame[playIndex].rotEngranajeOffset = (KeyFrame[playIndex + 1].rotEngranaje - KeyFrame[playIndex].rotEngranaje) / i_max_steps;
	KeyFrame[playIndex].rotEngranaje2Offset = (KeyFrame[playIndex + 1].rotEngranaje2 - KeyFrame[playIndex].rotEngranaje2) / i_max_steps;
	KeyFrame[playIndex].rotEngranaje3Offset = (KeyFrame[playIndex + 1].rotEngranaje3 - KeyFrame[playIndex].rotEngranaje3) / i_max_steps;
	KeyFrame[playIndex].rotEngranaje4Offset = (KeyFrame[playIndex + 1].rotEngranaje4 - KeyFrame[playIndex].rotEngranaje4) / i_max_steps;
	KeyFrame[playIndex].rotEngranaje5Offset = (KeyFrame[playIndex + 1].rotEngranaje5 - KeyFrame[playIndex].rotEngranaje5) / i_max_steps;
}


void animate(void)
{
	//Movimiento del objeto // barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			rotEngranaje += KeyFrame[playIndex].rotEngranajeOffset;
			rotEngranaje2 += KeyFrame[playIndex].rotEngranaje2Offset;
			rotEngranaje3 += KeyFrame[playIndex].rotEngranaje3Offset;
			rotEngranaje4 += KeyFrame[playIndex].rotEngranaje4Offset;
			rotEngranaje5 += KeyFrame[playIndex].rotEngranaje5Offset;

			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////




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
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	//Nave_M = Model();
	//Nave_M.LoadModel("Models/nave.obj");
	//Ala_M = Model();
	//Ala_M.LoadModel("Models/ala.obj");
	Aeolipile_base_M = Model();
	Aeolipile_base_M.LoadModel("Models/Aeolipile_base.obj");
	Aeolipile_M = Model();
	Aeolipile_M.LoadModel("Models/Aeolipile.obj");

	//carga de los archivos del modelo de la nave
	Nave_M = Model();
	Nave_M.LoadModel("Models/naveF.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	AlaI_M = Model();
	AlaI_M.LoadModel("Models/alaI.obj");
	Helice_M = Model();
	Helice_M.LoadModel("Models/helice.obj");

	//maquina steampunk
	Engranaje1_M = Model();
	Engranaje1_M.LoadModel("Models/engranaje_1.obj");
	Engranaje2_M = Model();
	Engranaje2_M.LoadModel("Models/engranaje_2.obj");
	ManecillaH_M = Model();
	ManecillaH_M.LoadModel("Models/manecilla_h.obj");
	ManecillaV_M = Model();
	ManecillaV_M.LoadModel("Models/manecilla_v.obj");
	ArcoG_M = Model();
	ArcoG_M.LoadModel("Models/arco_gris.obj");
	ArcoN_M = Model();
	ArcoN_M.LoadModel("Models/arco_negro.obj");

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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelauxN(1.0);
	glm::mat4 modelauxE(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset(0.0f, 0.0f);

	rotHelice = 0.0f;
	rotHeliceOffset = 10.0f;
	aleta = 0.0f;
	aletaOffset = 0.5f;

	//Keyframes
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;

	KeyFrame[0].rotEngranaje = 0.0f;
	KeyFrame[0].rotEngranaje2 = 0.0f;
	KeyFrame[0].rotEngranaje3 = 0.0f;
	KeyFrame[0].rotEngranaje4 = 0.0f;
	KeyFrame[0].rotEngranaje5 = 0.0f;

	
	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;

	KeyFrame[1].rotEngranaje = 180.0f;
	KeyFrame[1].rotEngranaje2 = 360.0f;
	KeyFrame[1].rotEngranaje3 = 90.0f;
	KeyFrame[1].rotEngranaje4 = 45.0f;
	KeyFrame[1].rotEngranaje5 = 10.0f;

	
	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;

	KeyFrame[2].rotEngranaje = 360.0f;
	KeyFrame[2].rotEngranaje2 = 720.0f;
	KeyFrame[2].rotEngranaje3 = 180.0f;
	KeyFrame[2].rotEngranaje4 = 90.0f;
	KeyFrame[2].rotEngranaje5 = 20.0f;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[3].rotEngranaje = 540.0f;
	KeyFrame[3].rotEngranaje2 = 1080.0f;
	KeyFrame[3].rotEngranaje3 = 270.0f;
	KeyFrame[3].rotEngranaje4 = 135.0f;
	KeyFrame[3].rotEngranaje5 = 30.0f;


	KeyFrame[4].movAvion_x = 3.0f;
	KeyFrame[4].movAvion_y = -2.0f;
	KeyFrame[4].giroAvion = 180.0f;

	KeyFrame[4].rotEngranaje = 720.0f;
	KeyFrame[4].rotEngranaje2 = 1440.0f;
	KeyFrame[4].rotEngranaje3 = 360.0f;
	KeyFrame[4].rotEngranaje4 = 180.0f;
	KeyFrame[4].rotEngranaje5 = 40.0f;


	KeyFrame[5].movAvion_x = 0.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].giroAvion = 180;

	KeyFrame[5].rotEngranaje = 900.0f;
	KeyFrame[5].rotEngranaje2 = 1800.0f;
	KeyFrame[5].rotEngranaje3 = 450.0f;
	KeyFrame[5].rotEngranaje4 = 225.0f;
	KeyFrame[5].rotEngranaje5 = 50.0f;

	//menu principal
	printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para incrementar las rotaciones en los 4 engranajes y las manecillas\n6.-Presiona 2 para habilitar la el incremento en la rotacion\n");

	//inicializacion de las variables que van hacer girar a los engranajes
	/*rotEngranaje = 0;
	rotEngranajeOffset = 5.0f;
	rotEngranaje2 = 0;
	rotEngranaje2Offset = 4.0f;
	rotEngranaje3 = 0;
	rotEngranaje3Offset = 2.0f;
	rotEngranaje4 = 0;
	rotEngranaje4Offset = 0.5f;
	rotEngranaje5 = 0;
	rotEngranaje5Offset = 0.05f;//*/

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		//¿Cómo haces para que el coche no se salga del piso?
		movCoche -= movOffset * deltaTime;
		rotllanta += rotllantaOffset * deltaTime;


		//incrementos 
		/*rotEngranaje += rotEngranajeOffset * deltaTime;
		rotEngranaje2 += rotEngranaje2Offset * deltaTime;
		rotEngranaje3 += rotEngranaje3Offset * deltaTime;
		rotEngranaje4 += rotEngranaje4Offset * deltaTime;
		rotEngranaje5 += rotEngranaje5Offset * deltaTime;//*/


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Pista_M.RenderModel();


		//Aquí va la nave con jerarquía de modelos, completar, nave
		//cuerpo
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		model = glm::rotate(model, (giroAvion + 180) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxN = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_M.RenderModel();

		//aleta derecha
		model = modelauxN;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.25f));
		//model = glm::rotate(model, -aleta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();//*/

		//aleta izq
		model = modelauxN;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.25f));
		//model = glm::rotate(model, +aleta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AlaI_M.RenderModel();//*/

		//helice derecha
		model = modelauxN;
		model = glm::translate(model, glm::vec3(-0.373f, -0.301f, -0.3547f));
		model = glm::rotate(model, -rotHelice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -rotHelice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();//*/

		//helice izq
		model = modelauxN;
		model = glm::translate(model, glm::vec3(-0.373f, -0.301f, 0.3547f));
		model = glm::rotate(model, -rotHelice * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helice_M.RenderModel();//*/


		//MAQUINA STEAMPUNK
		//arco negro, modelo padre
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.9f, -0.5f, 1.55f));
		modelauxE = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArcoN_M.RenderModel();//*/

		//arco gris, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArcoG_M.RenderModel();//*/

		//primer engranaje, engranaje 1, modelo hijo
		/**/model = modelauxE;
		model = glm::translate(model, glm::vec3(-0.2f, 1.7f, -0.12f));
		modelauxE = model;
		model = glm::rotate(model, rotEngranaje * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranaje1_M.RenderModel();//*/

		//segundo engranaje, engranaje 2, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, rotEngranaje2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranaje2_M.RenderModel();//*/

		//tercer engranaje, engranaje 2, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(0.1f, -1.4f, 0.0f));
		modelauxE = model;
		model = glm::rotate(model, rotEngranaje3 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranaje2_M.RenderModel();//*/

		//cuarto engranaje, engranaje 1, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.12f));
		modelauxE = model;
		model = glm::rotate(model, -rotEngranaje4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Engranaje1_M.RenderModel();//*/

		//manecilla horizontal, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.15f));
		model = glm::rotate(model, -rotEngranaje4 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManecillaH_M.RenderModel();//*/

		//manecilla vertical, modelo hijo
		model = modelauxE;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -rotEngranaje5 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ManecillaV_M.RenderModel();//*/

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

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
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

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		toffsetnumerocambiau += 0.25;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
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
		meshList[6]->RenderMesh();

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
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		glDisable(GL_BLEND);





		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
			habilitaranimacion++;
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			//printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)//reinicioFrame sirve para accesar una sola vez al guardoFrame
		{
			guardoFrame = 0;
			reinicioFrame++;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}

	//PARA LA ANIAMCION DE LA MAQUINA STEAMPUNK
	//para el engranaje mas grande, el segundo engranaje
	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			rotEngranaje += 180;
			rotEngranaje2 += 360;
			rotEngranaje3 += 90;
			rotEngranaje4 += 45;
			rotEngranaje5 += 10;

			printf("\nrotacion en rotEngranaje: %f\t rotacion en rotEngranaje2: %f\n", rotEngranaje, rotEngranaje2);
			printf("rotacion en rotEngranaje3: %f\t rotacion en rotEngranaje4: %f\n", rotEngranaje3, rotEngranaje4);
			printf("rotacion en rotEngranaje5: %f\n", rotEngranaje5);
			ciclo++;
			ciclo2 = 0;
			printf("Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_2])//Para que se pueda resetear el movimiento del ciclo 1
	{
		if (ciclo2 < 1)//ciclo2 sirve para accesar una sola vez al ciclo1
		{
			ciclo = 0;
			ciclo2++;//para que entre una sola vez a la estructura, si no entraria en cada ciclo de reloj
			printf("Ya puedes modificar tu variable presionando la tecla 1\n");
		}
	}
}
