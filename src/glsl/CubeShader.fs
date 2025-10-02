#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    vec3 color;// also used as diffuse, ambient
    vec3 specular;
    float shininess;
    bool useTexture;
    sampler2D texDiffuse;
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
uniform vec3 viewPos;

void main() {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient;
    vec3 diffuse;
    if (material.useTexture) {
        ambient = vec3(texture(material.texDiffuse, TexCoord)) * light.ambient;
        diffuse = vec3(texture(material.texDiffuse, TexCoord)) * diff * light.diffuse;
    } else {
        ambient = material.color * light.ambient;
        diffuse = material.color * diff * light.diffuse;
    }
    vec3 specular = material.specular * light.specular * spec;
    vec3 result = (ambient + diffuse + specular) * material.color * light.color;
    FragColor = vec4(result, 1.0);
}
