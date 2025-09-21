#include <string>
#include <vector>
#include <sstream>

//transforms a vector of strings into an array representation 
std::string vectorToJson(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < vec.size(); ++i) {
        oss << "\"" << vec[i] << "\"";
        if (i + 1 < vec.size()) {
            oss << ",";
        }
    }

    oss << "]";
    return oss.str();
}
