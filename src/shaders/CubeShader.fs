#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

uniform Material material;
uniform Light light;
uniform sampler2D ourTexture;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = material.ambient * light.ambient * light.color;

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 norm = normalize(normal);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse * light.diffuse) * light.color;
    vec3 specular = material.specular * light.specular * spec * light.color;
    vec3 result = (ambient + diffuse + specular) * material.color;
    FragColor = vec4(result, 1.0);
}
