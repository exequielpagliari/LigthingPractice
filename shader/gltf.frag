#version 460 core

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoord;

out vec4 FragColor;
in vec3 FragPos;
uniform sampler2D tex;

uniform float ambientStrength = 0.1;
uniform vec3 lightPos = vec3(1.0, 1.0, 1.0);
uniform vec3 lightColor = vec3(0.5, 0.5, 0.5);

void main() {
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  vec3 result = (ambientStrength + diffuse);
  FragColor = texture(tex, texCoord) * vec4(result, 1.0);
}
