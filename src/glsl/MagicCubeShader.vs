#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 faceColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 colors[7] = vec3[](
vec3(0.0, 1.0, 0.0),
vec3(1.0, 0.0, 0.0),
vec3(0.0, 0.0, 1.0),
vec3(1.0, 0.647, 0.0),
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 0.0),
vec3(0.1, 0.1, 0.1)
);

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    float fidx = floor(aTexCoord.x * 6.0);
    int idx = int(clamp(fidx, 0.0, 6.0));
    faceColor = colors[idx];
    fragPos = vec3(model * vec4(aPos, 1.0));
}
