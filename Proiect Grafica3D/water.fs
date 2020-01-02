#version 400

out vec4 out_Color;
in vec3 outColor;

void main()
{
  out_Color = vec4(outColor, 1.0);
}