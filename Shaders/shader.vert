#version 330//.vert su finalidad es decir al programa la posicion de la figura
layout (location =0) in vec3 pos;
//out vec4 vColor;//ya no es necesario por que el color sera uno fijo en los archivos .frag
uniform mat4 model;
uniform mat4 projection;
void main()
{
	gl_Position=projection*model*vec4(pos,1.0f);//le dice al programa la posicion de la figura
	//con los parametros definidos dentro del archivo main
	//vColor=vec4(color,1.0f);
	//vColor=vec4(clamp(pos,0.0f,1.0f),1.0f);//clamp da un degradado de color a la figura dependiendo
	//de su posicion
}