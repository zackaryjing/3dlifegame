#pragma once
#include <chrono>
#include <istream>
#include <random>
#include <sstream>
#include "glm/glm.hpp"


using std::string;
using std::vector;
using std::chrono::system_clock;


class Rand {
    Rand() = default;
    static inline auto dist = std::uniform_real_distribution(0.0f, 1.0f);
    static inline unsigned seed =
            system_clock::now().time_since_epoch().count();
    static inline auto generator = std::mt19937(seed);

public:
    static float genFloat() { return dist(generator); }
    static glm::vec3 genVec3(const float min, const float max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution dis(min, max);
        return glm::vec3(dis(gen), dis(gen), dis(gen));
    }
    static glm::vec3 getNorm() {
        const float u = 2 * M_PI * genFloat();
        const float v = 2 * genFloat() - 1;
        const float r = sqrt(1 - v * v);
        return glm::vec3{r * cos(u), v, r * sin(u)};
    }
};

enum class SplitMode {
    SkipSameSep, // count multiple continuous sep as one
    KeepSameSep, // count continuous sep as single one
};

class Str {
public:
    static vector<string> split(const string &str, const string &separator,
                                SplitMode splitMode = SplitMode::SkipSameSep) {
        if (separator.empty()) {
            throw std::invalid_argument("Seperator can't be empty");
        }
        vector<string> res;
        size_t m = separator.size();
        size_t n = str.size();
        if (m == 1) {
            char sep = separator[0];
            size_t base = 0;
            for (size_t i = 0; i < n; ++i) {
                if (str[i] == sep) {
                    if (i > base) {
                        res.push_back(str.substr(base, i - base));
                    } else if (splitMode == SplitMode::KeepSameSep) {
                        res.push_back("");
                    }
                    base = i + 1;
                }
            }
            if (str[n - 1] != sep) {
                res.push_back(str.substr(base, n - base));
            } else if (splitMode == SplitMode::KeepSameSep) {
                res.push_back("");
            }
        } else {
            size_t base = 0;
            while (base < n) {
                size_t pos = str.find(separator, base);
                if (pos == string::npos) {
                    break;
                }
                if (pos > base) {
                    res.push_back(str.substr(base, pos - base));
                } else if (splitMode == SplitMode::KeepSameSep) {
                    res.push_back("");
                }
                base = pos + m;
            }
            if (base < n) {
                res.push_back(str.substr(base, n - base));
            } else if (splitMode == SplitMode::KeepSameSep) {
                res.push_back("");
            }
        }
        return res;
    }
    static vector<string> splitWithSpace(const string &s) {
        std::istringstream iss(s);
        vector<string> res;
        string word;
        while (iss >> word) {
            res.push_back(word);
        }
        return res;
    }
};
