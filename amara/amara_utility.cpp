namespace Amara { 
    template <class T> bool vector_contains(std::vector<T> list, T f) {
        for (T obj: list) 
            if (obj == f) return true;
        return false;
    }
    
    template <class T> void vector_append(std::vector<T>& list1, std::vector<T> list2) {
        for (T element: list2) list1.push_back(element);
    }

    bool json_has(const nlohmann::json& data, const std::string& key) {
        return (data.find(key) != data.end()) ? true : false;
    }

    bool json_is(const nlohmann::json& data, const std::string& key) {
        return json_has(data, key) && data[key].is_boolean() && data[key];
    }

    bool json_erase(nlohmann::json& data, std::string key) {
        if (json_has(data, key)) {
            data.erase(key);
            return true;
        }
        return false;
    }

    bool string_equal(const std::string& str1, const std::string& str2) {
        return str1.compare(str2) == 0;
    }

    float fixed_range(float num, float min, float max) {
        if (num < min) return min;
        if (num > max) return max;
        return num;
    }

    float abs_mod(float num, float den) {
        while (num < 0) num += den;
        float result = fmod(num, den);
        return result;
    }

    SDL_Color getPixelFromSurface(SDL_Surface* gSurface, int gx, int gy) {
        int bpp = gSurface->format->BytesPerPixel;
        Uint8* p = (Uint8*)gSurface->pixels + gy*gSurface->pitch + gx*bpp;
        Uint32 pixelData;

        switch (bpp) {
            case 1:
                pixelData = *p;
                break;
            case 2:
                pixelData = *(Uint16 *)p;
                break;
            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    pixelData = p[0] << 16 | p[1] << 8 | p[2];
                else
                    pixelData = p[0] | p[1] << 8 | p[2] << 16;
                break;
            case 4:
                pixelData = *(Uint32*)p;
                break;
            default:
                pixelData = 0;       /* shouldn't happen, but avoids warnings */
        }

        SDL_Color rgba;
        SDL_GetRGBA(pixelData, gSurface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
        return rgba;
    }

    SDL_Surface* duplicate_surface(SDL_Surface* original) {
        if (!original) {
            return NULL; // Handle null input
        }
    
        // Create a new surface with the same format, width, height, and depth
        SDL_Surface* duplicate = SDL_CreateRGBSurfaceWithFormat(
            0, original->w, original->h, original->format->BitsPerPixel, original->format->format);
    
        if (!duplicate) {
            SDL_Log("Failed to create duplicate surface: %s\n", SDL_GetError());
            return NULL;
        }
    
        // Copy pixel data from the original surface to the new one
        if (SDL_BlitSurface(original, NULL, duplicate, NULL) < 0) {
            SDL_Log("Failed to blit surface: %s\n", SDL_GetError());
            SDL_FreeSurface(duplicate);
            return NULL;
        }
    
        return duplicate;
    }

    SDL_BlendMode AMARA_BLENDMODE_MASK = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD
    );

    SDL_BlendMode AMARA_BLENDMODE_ERASER = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD
    );

    SDL_BlendMode AMARA_BLENDMODE_NONE = SDL_BLENDMODE_NONE;
    SDL_BlendMode AMARA_BLENDMODE_BLEND = SDL_BLENDMODE_BLEND;

    SDL_Color AMARA_BLACK = { 0, 0, 0, 255 };
    SDL_Color AMARA_WHITE = { 255, 255, 255, 255 };

    FloatVector2 OriginVector = { 0, 0 };
}