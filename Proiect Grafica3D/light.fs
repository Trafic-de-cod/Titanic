#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	//ambient
	float ambientStr=0.1;
	vec3 ambient = ambientStr * lightColor;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diffuseStr = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStr * lightColor;

	//result
	vec3 result = (ambient+diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}