#version 460 core

layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex;

uniform float ambientStrength = 0.1;
uniform vec3 lightPos = vec3(1.0, 1.0, 1.0);
uniform vec3 lightColor = vec3(0.5, 0.5, 0.5);

void main() {
    FragColor = texture(tex, texCoord) *
    vec4(lightColor * ambientStrength, 1.0);
}
