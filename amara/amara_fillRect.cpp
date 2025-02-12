namespace Amara {
	class FillRect: public Amara::Actor, public Amara::MakeRect {
	public:
		Amara::Color color = {0, 0, 0, 255};
		Amara::Color recColor;

		SDL_Rect viewport;
		SDL_Rect srcRect;
		SDL_FRect destRect;
		SDL_Point origin;

		SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

		float renderOffsetX = 0;
		float renderOffsetY = 0;

		using Amara::Actor::Actor;
		FillRect(float gx, float gy, float gw, float gh, Amara::Color gColor) {
			x = gx;
			y = gy;
			width = gw;
			height = gh;
			color = gColor;
		}
		FillRect(float gx, float gy, float gw, float gh): FillRect(gx, gy, gw, gh, {0, 0, 0, 255}) {}

		using Amara::Actor::init;
		void init() {
			rectInit(this);
			Amara::Actor::init();
			entityType = "fillRect";
		}

		void configure(nlohmann::json config) {
			Amara::Actor::configure(config);
			rectConfigure(config);
		}

		Amara::FillRect* setColor(int r, int g, int b, int a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
			return this;
		}
		Amara::FillRect* setColor(int r, int g, int b) {
			return setColor(r, g, b, 255);
		}
		Amara::FillRect* setColor(Amara::Color gColor) {
			color = gColor;
			return this;
		}

		Amara::FillRect* setRenderOffset(float gx, float gy) {
			renderOffsetX = gx;
			renderOffsetY = gy;
			return this;
		}

		Amara::FillRect* setRenderOffset(float gi) {
			return setRenderOffset(gi, gi);
		}

		Amara::FillRect* setOrigin(float gx, float gy) {
			originX = gx;
			originY = gy;
			return this;
		}
		Amara::FillRect* setOrigin(float g) {
			return setOrigin(g, g);
		}
		Amara::FillRect* setOriginPosition(float gx, float gy) {
			originX = gx/width;
			originY = gy/height;
			return this;
		}
		Amara::FillRect* setOriginPosition(float g) {
			return setOriginPosition(g, g);
		}

		Amara::FillRect* setSize(float gw, float gh) {
			width = gw;
			height = gh;
			return this;
		}
		Amara::FillRect* setSize(float gs) {
			return setSize(gs, gs);
		}

		Amara::FloatRect calculateRect() {
			// Doesn't take into account scrollFactor or zoomFactor.
			return { 
				(x+renderOffsetX + properties->offsetX - (originX * width * scaleX)),
				(y-z+renderOffsetY + properties->offsetY - (originY * height * scaleY)),
				(width * scaleX),
				(height * scaleY)
			};
		}

		void draw(int vx, int vy, int vw, int vh) {
			if (!isVisible) return;
			bool skipDrawing = false;

			if (alpha < 0) alpha = 0;
			if (alpha > 1) alpha = 1;

			viewport.x = vx;
			viewport.y = vy;
			viewport.w = vw;
			viewport.h = vh;
			SDL_RenderSetViewport(properties->gRenderer, &viewport);

			float nzoomX = 1 + (properties->zoomX-1)*zoomFactorX*properties->zoomFactorX;
			float nzoomY = 1 + (properties->zoomY-1)*zoomFactorY*properties->zoomFactorY;

			bool scaleFlipHorizontal = false;
			bool scaleFlipVertical = false;
			float recScaleX = scaleX;
			float recScaleY = scaleY;

			if (scaleX < 0) {
				scaleFlipHorizontal = true;
				scaleX = abs(scaleX);
			}
			if (scaleY < 0) {
				scaleFlipVertical = true;
				scaleY = abs(scaleY);
			}
			scaleX = scaleX * (1 + (nzoomX - 1)*(zoomScaleX - 1));
			scaleY = scaleY * (1 + (nzoomY - 1)*(zoomScaleY - 1));

			destRect.x = ((x+renderOffsetX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
			destRect.y = ((y-z+renderOffsetY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
			destRect.w = ((width * scaleX) * nzoomX);
			destRect.h = ((height * scaleY) * nzoomY);

			scaleX = recScaleX;
			scaleY = recScaleY;

			origin.x = destRect.w * originX;
			origin.y = destRect.h * originY;

			if (destRect.x + destRect.w <= 0) skipDrawing = true;
			if (destRect.y + destRect.h <= 0) skipDrawing = true;
			if (destRect.x >= vw) skipDrawing = true;
			if (destRect.y >= vh) skipDrawing = true;
			if (destRect.w <= 0) skipDrawing = true;
			if (destRect.h <= 0) skipDrawing = true;

			if (!skipDrawing) {
				int newAlpha = (float)color.a * alpha * properties->alpha;

				checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);

				SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);

				SDL_SetRenderDrawBlendMode(properties->gRenderer, blendMode);
				SDL_SetRenderDrawColor(properties->gRenderer, color.r, color.g, color.b, newAlpha);

				SDL_RenderFillRectF(properties->gRenderer, &destRect);

				SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);

				checkHover(vx, vy, vw, vh, destRect.x, destRect.y, destRect.w, destRect.h);
			}

			Amara::Actor::draw(vx, vy, vw, vh);
		}
	};
}
