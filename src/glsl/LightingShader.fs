#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

struct Light {
    vec3 color;
};

uniform Light light;

void main()
{
    FragColor = vec4(light.color, 1.0);
}
