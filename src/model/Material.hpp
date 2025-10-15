#include <glm/glm.hpp>
#include <string>

using std::string;

struct Material {
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    string name;

public:
    Material(auto color, auto ambient, auto diffuse, auto specular,
             auto shininess, auto name) :
        color(color), ambient(ambient), diffuse(diffuse), specular(specular),
        shininess(shininess), name(name) {};
    Material() :
        color(glm::vec3(1.0f, 0.5f, 0.31f)),
        ambient(glm::vec3(1.0f, 0.5f, 0.31f)),
        diffuse(glm::vec3(1.0f, 0.5f, 0.31f)),
        specular(glm::vec3(0.5f, 0.5f, 0.5f)), shininess(32.0f), name("") {}

    void setAllSame(glm::vec3 property) {
        color = property;
        ambient = property;
        diffuse = property;
        specular = property;
    }
};
