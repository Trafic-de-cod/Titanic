#version 330 core
out vec4 TitanicFragColor;

in vec2 TitanicTexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    TitanicFragColor = texture(texture_diffuse1, TitanicTexCoords);
}