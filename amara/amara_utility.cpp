namespace Amara { 
    template <class T> bool vector_contains(std::vector<T> list, T f) {
        for (T obj: list) 
            if (obj == f) return true;
        return false;
    }
    
    template <class T> void vector_append(std::vector<T>& list1, std::vector<T> list2) {
        for (T element: list2) list1.push_back(element);
    }

    bool json_has(nlohmann::json data, std::string key) {
        return (data.find(key) != data.end()) ? true : false;
    }

    bool json_is(nlohmann::json data, std::string key) {
        return json_has(data, key) && data[key].is_boolean() && data[key];
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

    SDL_BlendMode AMARA_BLENDMODE_MASK = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD
    );

    SDL_BlendMode AMARA_BLENDMODE_UNDERMASK = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD
    );
}