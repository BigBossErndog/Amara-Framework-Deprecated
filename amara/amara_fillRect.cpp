#pragma once
#ifndef AMARA_FILLRECT
#define AMARA_FILLRECT

#include "amara.h"

namespace Amara {
	class FillRect: public Amara::Actor {
		public:
			Amara::Color color = {0, 0, 0, 255};
			Amara::Color recColor;

			SDL_Rect viewport;
			SDL_Rect srcRect;
			SDL_Rect destRect;
			SDL_Point origin;

			SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

			float width = 0;
			float height = 0;

	        float originX = 0;
			float originY = 0;

			float renderOffsetX = 0;
			float renderOffsetY = 0;

			FillRect(float gx, float gy, float gw, int gh, Amara::Color gColor) {
				x = gx;
				y = gy;
				width = gw;
				height = gh;
				color = gColor;
			}
			FillRect(float gx, float gy, float gw, float gh): FillRect(gx, gy, gw, gh, {0, 0, 0, 255}) {}

			void configure(nlohmann::json config) {
				Amara::Actor::configure(config);
				if (config.find("width") != config.end()) {
					width = config["width"];
				}
				if (config.find("height") != config.end()) {
					height = config["height"];
				}
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
			}

			void setColor(int r, int g, int b, int a) {
				color.r = r;
				color.g = g;
				color.b = b;
				color.a = a;
			}
			void setColor(int r, int g, int b) {
				setColor(r, g, b, 255);
			}
			void setColor(Amara::Color gColor) {
				color = gColor;
			}

			void setRenderOffset(float gx, float gy) {
				renderOffsetX = gx;
				renderOffsetY = gy;
			}

			void setRenderOffset(float gi) {
				setRenderOffset(gi, gi);
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

				destRect.x = floor((x+renderOffsetX - properties->scrollX*scrollFactorX + properties->offsetX - (originX * width * scaleX)) * nzoomX);
				destRect.y = floor((y-z+renderOffsetY - properties->scrollY*scrollFactorY + properties->offsetY - (originY * height * scaleY)) * nzoomY);
				destRect.w = ceil(ceil(width * scaleX) * nzoomX);
				destRect.h = ceil(ceil(height * scaleY) * nzoomY);

				origin.x = destRect.w * originX;
				origin.y = destRect.h * originY;

				if (destRect.x + destRect.w <= 0) skipDrawing = true;
				if (destRect.y + destRect.h <= 0) skipDrawing = true;
				if (destRect.x >= vw) skipDrawing = true;
				if (destRect.y >= vh) skipDrawing = true;
				if (destRect.w <= 0) skipDrawing = true;
				if (destRect.h <= 0) skipDrawing = true;

				if (!skipDrawing) {
					int hx, hy, hw, hh = 0;
					hw = destRect.w;
					hh = destRect.h;

					if (destRect.x >= 0) {
						hx = destRect.x + vx;
					}
					else {
						hw -= -(destRect.x);
						hx = vx;
					}
					if (destRect.y >= 0) {
						hy = destRect.y + vy;
					}
					else {
						hh -= -(destRect.y);
						hy = vy;
					}
					if (hx + hw > vx + vw) hw = ((vx + vw) - hx);
					if (hy + hh > vy + vh) hh = ((vy + vh) - hy);

					checkForHover(hx, hy, hw, hh);
					
					int newAlpha = (float)color.a * alpha * properties->alpha;

					SDL_GetRenderDrawColor(properties->gRenderer, &recColor.r, &recColor.g, &recColor.b, &recColor.a);

					SDL_SetRenderDrawBlendMode(properties->gRenderer, blendMode);
	                SDL_SetRenderDrawColor(properties->gRenderer, color.r, color.g, color.b, newAlpha);

					SDL_RenderFillRect(properties->gRenderer, &destRect);

					SDL_SetRenderDrawColor(properties->gRenderer, recColor.r, recColor.g, recColor.b, recColor.a);
				}

				Amara::Actor::draw(vx, vy, vw, vh);
			}
	};
}

#endif
