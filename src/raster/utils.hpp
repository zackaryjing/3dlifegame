#pragma once
#include <chrono>
#include <istream>
#include <random>
#include <sstream>


using std::vector;
using std::chrono::system_clock;
using std::string;


class Rand {
    Rand() = default;
    static inline auto dist = std::uniform_real_distribution(0.0f, 1.0f);
    static inline unsigned seed =
            system_clock::now().time_since_epoch().count();
    static inline auto generator = std::mt19937(seed);

public:
    static float gen_float() { return dist(generator); }
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
