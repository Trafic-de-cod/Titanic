#version 400
layout(location=0) in vec3 in_Position;
layout(location=2) in vec2 thisTexCoords;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * transform * vec4(in_Position, 1.0);
  TexCoord = vec2(thisTexCoords.x, thisTexCoords.y);
}