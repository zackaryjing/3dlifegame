#include <exception>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum class SplitMode {
    SkipSameSep,
    KeepSameSep,
};

static inline vector<string>
split(const string &str, const string &separator,
      SplitMode splitMode = SplitMode::KeepSameSep) {
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

template<typename T>
void show_vec(vector<T> vec) {
    for (auto v: vec) {
        cout << v << endl;
    }
}

int main() {
    string test1 = "f 14//64 12/11/64 70/11/64  71//64";
    for (auto p : split(test1," ")) {
        for (auto n : split(p,"/")) {
            cout << n << "|";
        }
        cout << endl;
    }
    show_vec(split(test1, " "));
}
