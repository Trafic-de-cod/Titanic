#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
	//ambient
	float ambientStr=0.2;
	vec3 ambient = ambientStr * lightColor;

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diffuseStr = 0.5 * max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStr * lightColor;
	//view direction
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = spec*lightColor;
	//result
	vec3 result = (ambient+diffuse+specular) * objectColor;
	FragColor = vec4(result, 1.0);
}