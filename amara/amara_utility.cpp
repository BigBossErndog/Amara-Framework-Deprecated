#pragma once

bool listContains(std::vector<nlohmann::json> list, nlohmann::json f) {
    for (nlohmann::json obj: list) {
        if (obj.is_null() && f.is_null()) return true;
        if (obj.is_string() && f.is_string() && ((std::string)f).compare(obj) == 0) return true;
        if (obj.is_number() && f.is_number() && ((float)obj == (float)f)) return true;
    }
    return false;
}