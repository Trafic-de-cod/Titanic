#version 330 core
out vec4 out_Color;
in vec2 TexCoord;
uniform sampler2D texture1;
void main()
{
  out_Color = texture(texture1, TexCoord);
}