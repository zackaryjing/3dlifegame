#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

struct Material {
    vec3 color;// also used as diffuse, ambient
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool useTexture;
    sampler2D texDiffuse;
    sampler2D texSpecular;
};

uniform Material material;
uniform vec3 viewPos;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    ambient = material.ambient * light.ambient;
    diffuse = material.diffuse * diff * light.diffuse;
    specular = material.specular * spec * light.specular;
    if (material.useTexture) {
        ambient *= vec3(texture(material.texDiffuse, TexCoord));
        diffuse *= vec3(texture(material.texDiffuse, TexCoord));
        specular *= vec3(texture(material.texSpecular, TexCoord));
    }
    return (ambient + diffuse + specular);
}

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    ambient = material.ambient * light.ambient;
    diffuse = material.diffuse * diff * light.diffuse;
    specular = material.specular * spec * light.specular;
    if (material.useTexture) {
        ambient *= vec3(texture(material.texDiffuse, TexCoord));
        diffuse *= vec3(texture(material.texDiffuse, TexCoord));
        specular *= vec3(texture(material.texSpecular, TexCoord));
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 noraml, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    if (theta > light.outerCutOff) {
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        ambient = material.ambient * light.ambient;
        diffuse = material.diffuse * diff * light.diffuse;
        specular = material.specular * spec * light.specular;
        if (material.useTexture) {
            ambient *= vec3(texture(material.texDiffuse, TexCoord));
            diffuse *= vec3(texture(material.texDiffuse, TexCoord));
            specular *= vec3(texture(material.texSpecular, TexCoord));
        }
        diffuse *= intensity;
        specular *= intensity;
    } else {
        if (material.useTexture) {
            ambient = material.ambient * vec3(texture(material.texDiffuse, TexCoord)) * light.ambient;
        } else {
            ambient = material.ambient * light.ambient;
        }
    }
    return (ambient + diffuse + specular);
}


void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);
    result += CalcDirLight(dirLight, norm, viewDir);
    for (int i = 0;i < NR_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    }
    result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
