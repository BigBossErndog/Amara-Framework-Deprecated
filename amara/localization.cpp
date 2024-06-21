#pragma once
#include "game.hpp"

class Localization {
public:
    static GameProperties* properties;
    static AssetManager* assets;
	static Loader* load;
    static json lib;

	static string folderPath;
	static vector<string> fileKeys;

    static void init(GameProperties* gProperties) {
        properties = gProperties;
        assets = properties->assets;
		load = properties->loader;
		fileKeys.clear();
    }

    static void add(json data) {
        lib.update(data);
    }
	static void clearFiles() {
		fileKeys.clear();
	}
	static void setFiles(vector<string> list) {
		fileKeys = list;
	}
    static void record(vector<string> list) {
        for (string key: list) {
            add(assets->getJSON(key));
			fileKeys.push_back(key);
        }
    }
	static void fetch() {
		for (string key: fileKeys) {
            add(assets->getJSON(key));
        }
		if (properties->testing && assets->has("testData")) {
			add(assets->getJSON("testData"));
		}
	}
	static void setLanguage(string lang) {
		string folderPath = string("assets/localization/") + lang;
		string recPath = load->currentBasePath;

		if (properties->testing) load->json("testData", "assets/data/testData.json");

		load->setBasePath(folderPath);

		load->json("textboxConfig", "/configs/textboxes.json", true);

		load->fromJSON(folderPath + string("/loadFonts.json"));
		load->fromJSON("assets/load/loadLocalization.json");
		load->fromJSON("assets/load/loadCutscenes.json");
		load->fromJSON("assets/load/loadCutscenes_Pry.json");

		load->setBasePath(recPath);
		fetch();
	}

    static json get(string key) {
        if (lib.find(key) != lib.end()) {
            return lib[key];
        }
		SDL_Log("Localization: Could not find word/phrase associated with \"%s\"", key.c_str());
        return nullptr;
    }

    static bool has(string key) {
        if (lib.find(key) != lib.end()) {
            return true;
        }
        return false;
    }

	static bool is(string key) {
		if (has(key) && get(key).is_boolean()) {
			return get(key);
		}
		return false;
	}

	static void set(string key, json val) {
		lib[key] = val;
	}
	static void setGet(string key, string valKey) {
		set(key, get(valKey));
	}

	static string fix(string phrase) {
		string fixed;
		string parsing;
		char c;
		StringParser parser = StringParser(phrase);
		
		while (!parser.finished()) {
			c = parser.next();
			if (c == '$') {
				parser.parseUntil('(');
				parsing = parser.parseUntil(')');
				json found = get(parsing);
				if (found.is_string()) fixed += found;
				else if (found.is_number()) {
					float value = found;
					fixed += to_string(value);
				}
			}
			else if (c == '\\') {
				c = parser.next();
				if (c == 'n') {
					fixed += '\n';
				}
				else if (c == '\'') {
					fixed += '"';
				}
			}
			else {
				fixed += c;
			}
		}

		return fixed;
	}

	static string fixGet(string key) {
		return fix(get(key));
	}
};
GameProperties* Localization::properties = nullptr;
AssetManager* Localization::assets = nullptr;
Loader* Localization::load = nullptr;
json Localization::lib = json::object();
string Localization::folderPath = "";
vector<string> Localization::fileKeys = {};