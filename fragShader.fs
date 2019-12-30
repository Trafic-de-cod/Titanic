#version 400
out vec4 out_Color;
in vec3 outColor;
in vec2 TexCoord;
uniform sampler2D texture1;
void main()
{
  out_Color = texture(texture1, TexCoord);
}