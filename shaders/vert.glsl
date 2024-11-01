#version 410 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rotation;


out vec3 vertexColor; // specify a color output to the fragment shader
//the order in which you multiply is important here, rotation first makes the obj squished
void main()
{
    gl_Position = projection * view * model * rotation * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = aColor; // set the output variable to a dark-red color
}