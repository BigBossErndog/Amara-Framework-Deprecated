namespace Amara {
    void MakeRect::rectConfigure(nlohmann::json config) {
        if (config.find("width") != config.end()) width = config["width"];
        if (config.find("height") != config.end()) height = config["height"];
        if (config.find("originX") != config.end()) {
            originX = config["originX"];
        }
        if (config.find("originY") != config.end()) {
            originY = config["originY"];
        }
        if (config.find("origin") != config.end()) {
            originX = config["origin"];
            originY = config["origin"];
        }
        if (config.find("originPositionX") != config.end()) {
            originX = config["originPositionX"];
            originX = originX/width;
        }
        if (config.find("originPositionY") != config.end()) {
            originY = config["originPositionY"];
            originY = originY/height;
        }
        if (config.find("originPosition") != config.end()) {
            originX = config["originPosition"];
            setOriginPosition(originX, originX);
        }
        
        if (config.find("scaledWidth") != config.end()) {
            if (rectEntity) rectEntity->scaleX = ((float)config["scaledWidth"])/width;
        }
        if (config.find("scaledHeight") != config.end()) {
            if (rectEntity) rectEntity->scaleY = ((float)config["scaledHeight"])/height;
        }
    }
    
    FloatRect MakeRect::toRect() {
        if (rectEntity) return { left().x, top().y, width*rectEntity->scaleX, height*rectEntity->scaleY };
        return fixedRect();
    }

    MakeRect* MakeRect::setRect(FloatRect rect) {
        if (rectEntity) {
            rectEntity->x = rect.x;
            rectEntity->y = rect.y;
        }
        width = rect.width;
        height = rect.height;
        return this;
    }

    Amara::MakeRect* MakeRect::scaleTo(float gw, float gh) {
        if (rectEntity) {
            rectEntity->scaleX = gw/width;
            rectEntity->scaleY = gh/height;
        }
        return this;
    }

    Amara::MakeRect* MakeRect::scaleToWidth(float gw) { 
        if (rectEntity) {
            rectEntity->scaleX = gw/width;
            rectEntity->scaleY = rectEntity->scaleX;
        }
        return this;
    }
    Amara::MakeRect* MakeRect::scaleToHeight(float gh) {
        if (rectEntity) {
            rectEntity->scaleY = gh/height;
            rectEntity->scaleX = rectEntity->scaleY;
        }
        return this;
    }

    Amara::MakeRect* MakeRect::scaleToFit(float gw, float gh) {
        // maintains aspect ratio
        if (rectEntity) {
            float asp1 = width/height;
            float asp2 = gw/gh;
            if (asp1 > asp2) {
                scaleToWidth(gw);
            }
            else {
                scaleToHeight(gh);
            }
        }
        return this;
    }
    Amara::MakeRect* MakeRect::scaleToFit(FloatRect rect) {
        if (rectEntity) {
            scaleToFit(rect.width, rect.height);
            
            if (width*rectEntity->scaleX*originX > rect.width) {
                rectEntity->x = (rect.x+rect.width)/2.0 - width*rectEntity->scaleX*originX;
            }
            else {
                if (rectEntity->x - width*rectEntity->scaleX*originX < rect.x) rectEntity->x = rect.x + width*rectEntity->scaleX*originX;
                if (rectEntity->x + width*rectEntity->scaleX*(1-originX) > rect.x + rect.width) {
                    rectEntity->x = rect.x + rect.width - width*rectEntity->scaleX*(1-originX);
                }
            }

            if (height*rectEntity->scaleY*originY > rect.height) {
                rectEntity->y = (rect.y+rect.height)/2.0 - height*rectEntity->scaleY*originY;
            }
            else {
                if (rectEntity->y - height*rectEntity->scaleY*originY < rect.y) rectEntity->y = rect.y + height*rectEntity->scaleY*originY;
                if (rectEntity->y + height*rectEntity->scaleY*(1-originY) > rect.y + rect.height) {
                    rectEntity->y = rect.y + rect.height - height*rectEntity->scaleY*(1-originY);
                }
            }
        }
        return this;
    }

    FloatVector2 MakeRect::top() {
        if (rectEntity) return {
            (float)(rectEntity->x + (0.5 - originX)*width*rectEntity->scaleX),
            (float)(rectEntity->y - height*originY*rectEntity->scaleY)
        };
        return { (float)(width/2.0), 0 };
    }
    FloatVector2 MakeRect::bottom() {
        if (rectEntity) return {
            (float)(rectEntity->x + (0.5 - originX)*width*rectEntity->scaleX),
            (float)(rectEntity->y + height*(1-originY)*rectEntity->scaleY)
        };
        return {
            (float)(width/2.0),
            (float)(height*rectEntity->scaleY)
        };
    }
    FloatVector2 MakeRect::left() {
        if (rectEntity) return {
            (float)(rectEntity->x - width*originX*rectEntity->scaleX),
            (float)(rectEntity->y + (0.5 - originY)*height*rectEntity->scaleY)
        };
        return { 0, (float)(height/2.0) };
    }
    FloatVector2 MakeRect::right() {
        if (rectEntity) return {
            (float)(rectEntity->x + width*(1-originX)*rectEntity->scaleX),
            (float)(rectEntity->y + (0.5 - originY)*height*rectEntity->scaleY)
        };
        return { (float)(width*rectEntity->scaleX), (float)(height/2.0) };
    }
    FloatVector2 MakeRect::center() {
        if (rectEntity) return {
            (float)(rectEntity->x + (0.5 - originX)*width*rectEntity->scaleX),
            (float)(rectEntity->y + (0.5 - originY)*height*rectEntity->scaleY)
        };
        return { (float)(width/2.0), (float)(height/2.0) };
    }
}