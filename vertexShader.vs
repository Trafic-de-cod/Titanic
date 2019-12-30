#version 400
in layout(location=0) vec3 in_Position;
in layout(location=1) vec3 thisColor;
in layout(location=1) vec2 thisTexCoords;

out vec3 outColor;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(in_Position, 1.0);
  outColor = thisColor;
  TexCoord = vec2(thisTexCoords.x, thisTexCoords.y);
}