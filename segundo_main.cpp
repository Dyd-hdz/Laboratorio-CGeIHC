#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

#include <stdlib.h> // para rand y srand, funciones aleatorias
#include <time.h>   // para time

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

//creacion de variables que seran usadas para el cambio de color en la pantalla
float var1 = 0;
float var2 = 0;
float var3 = 0;

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,0.0f,1.0f);	 			\n\
}";

//color es para el color de los triangulos, por defecto aqui son para todos los que dibujemos sea el mismo color

void CrearTriangulo()
{
	GLfloat vertices[] = {
		//X, Y, Z por el momento z siempre es 0
		-.9f, .3f,0.0f,//primer triangulo de L
		-.7f,.3f,0.0f,
		-.9f,-.3f, 0.0f,
		
		-.7f,.3f,0.0f,//segundo triangulo de L
		-.9f,-.3f, 0.0f,
		-.7f,-.3f,0.0f,

		-.7f,-.3f,0.0f,//tercer triangulo de L
		-.7f,-.1f, 0.0f,
		-.5f, -.3f,0.0f,
		
		-.7f,-.1f, 0.0f,//cuarto triangulo de L
		-.5f, -.3f,0.0f,
		-.5f,-.1f,0.0f,

		-.3f,.3f,0.0f,//primer triangulo de E
		-.2f,.3f,0.0f,
		-.3f,-.3f,0.0f,

		-.2f,.3f,0.0f,//segundo triangulo de E
		-.3f,-.3f,0.0f,
		-.2f,-.3f,0.0f,

		-.2f,.3f,0.0f,//tercer triangulo de E
		-.2f, .2f,0.0f,
		.1f,.3f,0.0f,

		-.2f, .2f,0.0f,//cuarto triangulo de E
		.1f,.3f,0.0f,
		.1f,.2f,0.0f,

		-.2f,-.1f,0.0f,//quinto triangulo de E
		-.2f,.1f,0.0f,
		.0f,-.1f,0.0f,

		-.2f,.1f,0.0f,//sexto triangulo de E
		.0f,-.1f,0.0f,
		.0f,.1f,0.0f,

		-.2f,-.2f,0.0f,//septimo triangulo de E
		-.2f,-.3f,0.0f,
		.1f,-.2f,0.0f,

		-.2f,-.3f,0.0f,//octavo triangulo de E
		.1f,-.2f,0.0f,
		.1f,-.3f,0.0f,

		.3f,.3f,0.0f,//primer triangulo de H
		.5f,.3f,0.0f,
		.3f,-.3f,0.0f,

		.5f,.3f,0.0f,//segundo triangulo de H
		.3f,-.3f,0.0f,
		.5f,-.3f,0.0f,

		.5f,.1f,0.0f,//tercer triangulo de H
		.5f,-.1f,0.0f,
		.7f,.1f,0.0f,

		.5f,-.1f,0.0f,//cuarto triangulo de H
		.7f,.1f,0.0f,
		.7f,-.1f,0.0f,

		.7f,.3f,0.0f,//quinto triangulo de H
		.7f,-.3f,0.0f,
		.9f,.3f,0.0f,

		.7f,-.3f,0.0f,//sexto triangulo de H
		.9f,.3f,0.0f,
		.9f,-.3f,0.0f
	};

	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0);
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

	//inicializacion de la funcion aleatoria
	srand(time(NULL)); //para generar numeros aleatorios, en base al tiempo
	int ultimoIntervalo = -1; //variable para controlar el cambio del intervalos
	//inicializado en -1 para que cambie a la primera comparacion con el uintervaloactual

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		int intervaloActual = (int)(glfwGetTime() / 2);//hacemos que el intervalor sea cada 2 segundos
		//ya que crea escalones de tiempo cada 2 seg
		//(int)glfwGettime()->hacemos que guarde un segundo por segundo, gracias al casteo int, cambia de valor de uno en uno
		//(int)glfwGettime()/2->al dividir entre dos, forzamos a que el cambio sea cada 2 seg, el cambio de valor igualmente sera de uno en uno

		//si el intervalo actual cambia
		if (intervaloActual != ultimoIntervalo)//si el intervaloactual es diferente al ultimointervalo
		{
			//para generar valores aleatorios entre 0.0 y 1.0 (rango del RGB)
			var1 = (rand() % 101) / 100.0f;
			//rand() devuelve un numero aleatorio grande
			//al operarlo con el modulo 101, obligamos a que el valor sea entre 0-100, ya que es el residuo de la division
			//finalmente al dividirlo este rango de valores puede darnos valores de 0.0-1.0
			
			var2 = (rand() % 101) / 100.0f;
			var3 = (rand() % 101) / 100.0f;

			ultimoIntervalo = intervaloActual;//actualiza ultimointervalo al valor del intervaloactual
		}

		//Limpiar la ventana
		glClearColor(var1,var2,var3,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//para el dibujo de las letas
		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,54);//3 un tringulo, 6 dos tringulos, 9 3 triangulos, etc
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}