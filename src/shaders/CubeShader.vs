#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec3 ourColor;
out vec2 TexCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(model))) * aNormal;
    ourColor = vec3(dot(lightDir, normal)) * 0.5 + 0.5;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
