#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

struct Material {
    vec3 color;
    vec3 ambient;
};

struct Light {
    vec3 ambient;
    vec3 color;
};

uniform Material material;
uniform Light light;
uniform sampler2D ourTexture;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = material.ambient * light.ambient * light.color;
    vec3 result = ambient * material.color;
    FragColor = vec4(result, 1.0);
}
