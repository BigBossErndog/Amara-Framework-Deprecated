   namespace Amara { 
    bool vectorContains(std::vector<nlohmann::json> list, nlohmann::json f) {
        for (nlohmann::json obj: list) {
            if (obj.is_null() && f.is_null()) return true;
            if (obj.is_string() && f.is_string() && ((std::string)f).compare(obj) == 0) return true;
            if (obj.is_number() && f.is_number() && ((float)obj == (float)f)) return true;
        }
        return false;
    }

    void vectorAppend(std::vector<nlohmann::json>& list1, std::vector<nlohmann::json> list2) {
        list1.insert(std::end(list1), std::begin(list2), std::end(list2));
    }
}