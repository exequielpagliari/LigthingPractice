#version 460 core

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoord;

out vec4 FragColor;
in vec3 FragPos;
uniform sampler2D tex;

uniform float ambientStrength = 0.1;
uniform float specularStrength = 0.1;
uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform vec3 lightColor = vec3(0.5, 0.5, 0.5);
uniform vec3 viewPos = vec3(0.0,0.0,0.0);


void main() {
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir =  reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir,reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  vec3 result = (ambientStrength + diffuse + specular);
    FragColor = texture(tex, texCoord) * vec4(result, 1.0);
}
