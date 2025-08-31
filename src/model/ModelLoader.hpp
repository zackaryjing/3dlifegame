#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Model.hpp"

#ifndef MODEL_DIR
#define MODEL_DIR "../../assets/model/"
#endif

using std::ifstream;
using std::string;

class ModelLoader {
private:
public:
    static inline void init() {}
    static inline Model load(string &path) {
        ifstream file(MODEL_DIR);
    }
};
