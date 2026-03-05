//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*>meshColorList; //para poder definir la piramide con un color predefinido
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Pirámide triangular, pyraminx base
void CrearPyraminxP()
{
	unsigned int indices_pyraminxP[] = {
	0,1,2,//cara frontal
	1,3,2,//base
	3,0,2,//lateral derecha
	1,0,3//lateral izquierda

	};
	GLfloat vertices_pyraminxP[] = {
		// x,     y,      z
		0.0f,    0.5f,   0.0f,    // vértice superior
		-0.5f,  -0.33f,  0.28f,   // vértice base 1
		0.5f,  -0.33f,  0.28f,   // vértice base 2
		0.0f,  -0.33f, -0.57f    // vértice base 3

	};
	Mesh* pyraminxP = new Mesh();
	pyraminxP->CreateMesh(vertices_pyraminxP, indices_pyraminxP, 12, 12);
	meshList.push_back(pyraminxP);
}

// Pirámide triangular, pyraminx
void CrearPyraminx()
{
	unsigned int indices_pyraminx[] = {
			/*0,1,2,//cara frontal
			1,3,2,//base
			3,0,2,//lateral derecha
			1,0,3//lateral izquierda*/

			0,1,2,
			3,4,5,
			6,7,8,
			9,10,11
	};
	GLfloat vertices_pyraminx[] = {
		// x,     y,      z
		/*0.0f,    0.5f,   0.0f,    // vértice superior
		-0.5f,  -0.33f,  0.28f,   // vértice base 1
		 0.5f,  -0.33f,  0.28f,   // vértice base 2
		 0.0f,  -0.33f, -0.57f    // vértice base 3*/

		//para hacer independientes las caras y asi poder cambiar de color de una cara de la piramide por si sola
		//cada cara tendra un color predefinido
		// --- Cara Frontal ---
		//x		y		z		 r			 g			 b		
        0.0f,  0.5f,   0.0f,	0.0f,		0.0f,		1.0f,   // Vértice 0
        -0.5f, -0.33f, 0.28f,	0.0f,		0.0f,		1.0f,// Vértice 1
        0.5f,  -0.33f, 0.28f,   0.0f,		0.0f,		1.0f,// Vértice 2

        // --- Cara Base ---
        -0.5f, -0.33f, 0.28f,   1.0f,		1.0f,		0.0f,// Vértice 3 
        0.0f,  -0.33f, -0.57f,  1.0f,		1.0f,		0.0f,// Vértice 4 
        0.5f,  -0.33f, 0.28f,   1.0f,		1.0f,		0.0f,// Vértice 5 

        // --- Cara Lateral Derecha ---
        0.0f,  0.5f,   0.0f,    1.0f,		0.0f,		0.0f,// Vértice 6 
        0.5f,  -0.33f, 0.28f,   1.0f,		0.0f,		0.0f,// Vértice 7 
        0.0f,  -0.33f, -0.57f,  1.0f,		0.0f,		0.0f,// Vértice 8 

        // --- Cara Lateral Izquierda ---
        -0.5f, -0.33f, 0.28f,   0.0f,		1.0f,		0.0f,// Vértice 9 
        0.0f,  0.5f,   0.0f,    0.0f,		1.0f,		0.0f,// Vértice 10 
        0.0f,  -0.33f, -0.57f,  0.0f,		1.0f,		0.0f// Vértice 11 

	};
	MeshColor* pyraminx = new MeshColor();
	//pyraminx->CreateMesh(vertices_pyraminx, indices_pyraminx, 72, 12);//para la definicion de la piramide sin color predefinido
	pyraminx->CreateMeshColor(vertices_pyraminx, 72); //para la defincion de la piramide con color predefinido
	//meshList.push_back(pyraminx);//para la definicion de la piramide sin color
	meshColorList.push_back(pyraminx);//para la definicion de la piramide con color
}

//Pyraminx para la cara frontal
void CrearPyraminx1()
{
	unsigned int indices_Pyraminx1[] = {
			0,1,2
	};

	GLfloat vertices[] = {
		// x,     y,      z
		0.0f,    0.5f,   0.0f,
		-0.5f,  -0.33f,  0.28f,
		 0.5f,  -0.33f,  0.28f

	};
	Mesh* Pyraminx1 = new Mesh();
	Pyraminx1->CreateMesh(vertices, indices_Pyraminx1, 9, 3);
	meshList.push_back(Pyraminx1);
}

//Pyraminx para la cara derecha
void CrearPyraminx2()
{
	unsigned int indices_Pyraminx2[] = {
			0,1,2
	};

	GLfloat vertices[] = {
		// x,     y,      z
		0.0f,    0.5f,   0.0f,
		-0.5f,  -0.33f,  0.28f,
		0.0f,  -0.33f, -0.57f

	};
	Mesh* Pyraminx2 = new Mesh();
	Pyraminx2->CreateMesh(vertices, indices_Pyraminx2, 9, 3);
	meshList.push_back(Pyraminx2);
}

//Pyraminx para la cara izq
void CrearPyraminx3()
{
	unsigned int indices_Pyraminx3[] = {
			2,0,1
	};

	GLfloat vertices[] = {
		// x,     y,      z
		0.0f,    0.5f,   0.0f,  
		 0.5f,  -0.33f,  0.28f, 
		 0.0f,  -0.33f, -0.57f 

	};
	Mesh* Pyraminx3 = new Mesh();
	Pyraminx3->CreateMesh(vertices, indices_Pyraminx3, 9, 3);
	meshList.push_back(Pyraminx3);
}

/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(15, 1.0f);//índice 2 en MeshList, el primer parameto es para ver el cilindro mas redondo
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearPyraminx();//MeshColorList para la piramide con colores predefinidos
	CrearPyraminxP();//indice 5 en el MeashList
	CrearPyraminx1();//indice 6 en el MeshList
	CrearPyraminx2();//indice 7 en el MeshList
	CrearPyraminx3();//indice 8 en el Mesh List
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);
	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;

	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	glm::vec3 borde = glm::vec3(0.0f, 0.0f, 0.0f);//para el color del borde de figuras predefinidas con color
	glm::vec3 bordeS = glm::vec3(0.0f, 0.0f, 0.0f);//para el color del borde de figuras predefinidas sin color

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//para dibujar figura con un color solido
		//shaderList[0].useShader();
		//uniformModel = shaderList[0].getModelLocation();
		//uniformProjection = shaderList[0].getProjectLocation();
		//uniformView = shaderList[0].getViewLocation();
		//uniformColor = shaderList[0].getColorLocation();

		//para la piramide triangular base, el modelo padre
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		glm::mat4 modelBase = glm::mat4(1.0);
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, -3.0f));
		modelBase = glm::scale(modelBase, glm::vec3(5.0f, 5.0f, 5.0f));
		//modelBase = glm::rotate(modelBase, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//para girar la piramide padre 120 lateral izq, 240 lateral derecha
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[5]->RenderMesh();
		
		//para la piramide triangular base, el modelo padre
		/*glm::mat4 modelBase = glm::mat4(1.0);//modelo padre, contendra a los cubos verdes, que son las ventanas y puerta
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, -3.0f));
		modelBase = glm::scale(modelBase, glm::vec3(5.0f,5.0f,5.0f));
		//modelBase = glm::rotate(modelBase, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBase));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();*/

		//la cara frontal
		//1er renglon
		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-.34f, -.22f, 0.186f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);

		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde = shaderList[0].getModelLocation();
		GLuint uColorBorde = shaderList[0].getColorLocation();
		
		glm::mat4 modelBorde = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));//hace el dibujo del bordo mas grueso al hacer mas grande el dibujo de la figura
		//que en este caso esta en modo de linea
		
		glUniformMatrix4fv(uModelBorde, 1, GL_FALSE, glm::value_ptr(modelBorde));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde, 1, glm::value_ptr(borde));
		
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal

		
		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.0f, -.22f, 0.186f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();// */
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde1 = shaderList[0].getModelLocation();
		GLuint uColorBorde1 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde1 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde1, 1, GL_FALSE, glm::value_ptr(modelBorde1));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde1, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.34f, -.22f, 0.186f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();// */
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde2 = shaderList[0].getModelLocation();
		GLuint uColorBorde2 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde2 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde2, 1, GL_FALSE, glm::value_ptr(modelBorde2));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde2, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx1, modelo hijo
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.17f, -.166f, 0.2828f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[6]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS = shaderList[0].getModelLocation();
		GLuint uColorBordeS = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS, 1, GL_FALSE, glm::value_ptr(modelBordeS));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS, 1, glm::value_ptr(bordeS));
		meshList[6]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx1, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.17f, -.166f, 0.2828f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[6]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS1 = shaderList[0].getModelLocation();
		GLuint uColorBordeS1 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS1 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS1, 1, GL_FALSE, glm::value_ptr(modelBordeS1));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS1, 1, glm::value_ptr(bordeS));
		meshList[6]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//2do renglon
		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-.17f, 0.060525f, 0.09095f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde3 = shaderList[0].getModelLocation();
		GLuint uColorBorde3 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde3 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde3, 1, GL_FALSE, glm::value_ptr(modelBorde3));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde3, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/
		
		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(.17f, 0.060525f, 0.09095f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde4 = shaderList[0].getModelLocation();
		GLuint uColorBorde4 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde4 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde4, 1, GL_FALSE, glm::value_ptr(modelBorde4));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde4, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx1, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.0f, 0.117, 0.187));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[6]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS2 = shaderList[0].getModelLocation();
		GLuint uColorBordeS2 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS2 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS2, 1, GL_FALSE, glm::value_ptr(modelBordeS2));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS2, 1, glm::value_ptr(bordeS));
		meshList[6]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//3er renglon
		//para las piramides pequeñas, las piezas del pyraminx, modelo hijo
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.0f, 0.34f, 0.0f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde5 = shaderList[0].getModelLocation();
		GLuint uColorBorde5 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde5 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde5, 1, GL_FALSE, glm::value_ptr(modelBorde5));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde5, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//cara lateral izq
		//1er renglon
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.001f, -.22f, -0.38f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();// */
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde6 = shaderList[0].getModelLocation();
		GLuint uColorBorde6 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde6 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde6, 1, GL_FALSE, glm::value_ptr(modelBorde6));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde6, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.17f, -.22f, -0.102f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde7 = shaderList[0].getModelLocation();
		GLuint uColorBorde7 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde7 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde7, 1, GL_FALSE, glm::value_ptr(modelBorde7));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde7, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx3, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.335f, -.165f, -0.005f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[8]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS3 = shaderList[0].getModelLocation();
		GLuint uColorBordeS3 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS3 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS3, 1, GL_FALSE, glm::value_ptr(modelBordeS3));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS3, 1, glm::value_ptr(bordeS));
		meshList[8]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx3, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.17f, -.165f, -0.285f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[8]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS4 = shaderList[0].getModelLocation();
		GLuint uColorBordeS4 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS4 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS4, 1, GL_FALSE, glm::value_ptr(modelBordeS4));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS4, 1, glm::value_ptr(bordeS));
		meshList[8]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/
		
		//2do renglon
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.0f, 0.06f, -0.19f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde8 = shaderList[0].getModelLocation();
		GLuint uColorBorde8 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde8 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde8, 1, GL_FALSE, glm::value_ptr(modelBorde8));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde8, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx3, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-0.17f, .115f, -0.105f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[8]->RenderMesh(); //dibuja cubo y pirámide triangular*/

		//cara lateral derecha
		//1er renglon
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.17f, -.22f, -0.1f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBorde9 = shaderList[0].getModelLocation();
		GLuint uColorBorde9 = shaderList[0].getColorLocation();
		glm::mat4 modelBorde9 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBorde9, 1, GL_FALSE, glm::value_ptr(modelBorde9));
		borde = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBorde9, 1, glm::value_ptr(borde));
		meshColorList[0]->RenderMeshColor();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx2, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.335f, -.165f, -0.005f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[7]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS5 = shaderList[0].getModelLocation();
		GLuint uColorBordeS5 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS5 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS5, 1, GL_FALSE, glm::value_ptr(modelBordeS5));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS5, 1, glm::value_ptr(bordeS));
		meshList[7]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//para las piramides pequeñas, invertida, las piezas del pyraminx2, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.17f, -.165f, -0.285f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[7]->RenderMesh(); //dibuja cubo y pirámide triangular*/
		//para el contorno de la piramide (pieza)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//cambia el modo de dibujo a lineas
		glLineWidth(5.0f);//para el grosor del contorno
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.0f, -1.0f);
		shaderList[0].useShader();//el shaderlisto[0]-> para poner el color que deseemos a la hora de dibujar
		GLuint uModelBordeS6 = shaderList[0].getModelLocation();
		GLuint uColorBordeS6 = shaderList[0].getColorLocation();
		glm::mat4 modelBordeS6 = glm::scale(model, glm::vec3(1.025f, 1.025f, 1.025f));
		glUniformMatrix4fv(uModelBordeS6, 1, GL_FALSE, glm::value_ptr(modelBordeS6));
		bordeS = glm::vec3(1.0f, 1.0f, 1.0f);//el color
		glUniform3fv(uColorBordeS6, 1, glm::value_ptr(bordeS));
		meshList[7]->RenderMesh();//el dibujo del contorno
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//cambia el modo de dibujo normal*/

		//2do renglon
		//para las piramides pequeñas, invertida, las piezas del pyraminx2, modelo hijo
		shaderList[0].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(0.17f, .115f, -0.105f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);//color
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[7]->RenderMesh(); //dibuja cubo y pirámide triangular*/

		//base de la piramide
		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(-.17f, -.22f, 0.087f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/

		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(.17f, -.22f, 0.087f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/

		shaderList[1].useShader();//shaderlist[1]->para la piramide con un color predefinido
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		model = modelBase; //modelo hijo
		model = glm::translate(model, glm::vec3(.0005f, -.22f, -0.2f));
		model = glm::scale(model, glm::vec3(0.34f, 0.34f, 0.34f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 1.0f, 0.0f);//color
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[5]->RenderMesh(); //dibuja cubo y pirámide triangular
		meshColorList[0]->RenderMeshColor();//*/

		/*//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.
		model = glm::mat4(1.0f);
		color=glm::vec3(0.0f,1.0f,0.0f);
		//Opcional duplicar esta traslación inicial para posicionar en -Z a los objetos en el mismo punto
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMeshGeometry();
		
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		