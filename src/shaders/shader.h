#pragma once
#include <iostream>
#include <string>

using std::string;
using std::ifstream;
using std::stringstream;
using std::cerr;
using std::cout;
using std::endl;

class Shader {
public:
    unsigned int ID;
    Shader(const char *vertexPath, const char *fragmentPath);
    void use() const;
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;

    void setFloat(const string &name, const std::initializer_list<float> &value) const;
    void setFloat(const string &name, float value) const;

};
