#version 330 core
layout (location = 0) in vec3 aPos;     // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;   // the color variable has attribute position 1

out vec3 ourColor; // output color to the fragment shader

uniform float xOffset;

void main()
{
    vec3 tempPos = vec3(aPos.x + xOffset, aPos. y, aPos.z);
    gl_Position = vec4(tempPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}