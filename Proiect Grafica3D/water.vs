#version 400
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Color;

out vec3 outColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
   outColor=in_Color;
   gl_Position = projection * view * vec4(in_Position, 1.0);
}