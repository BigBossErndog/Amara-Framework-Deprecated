namespace Amara { 
    template <class T> bool vectorContains(std::vector<T> list, T f) {
        for (T obj: list) 
            if (obj == f) return true;
        return false;
    }

    template <class T> void vectorAppend(std::vector<T>& list1, std::vector<T> list2) {
        for (T element: list2) list1.push_back(element);
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
}