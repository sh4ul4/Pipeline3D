#pragma once

class ScrollZone {
	const InputEvent& inputEvent;
	Mouse mouse;
	Keyboard keyboard;

	SDL_Texture* texture;
	
	Point2D<int> pos;
	int width, height;

	Point2D<int> renderPos;
	int renderW, renderH;

	Color backgroundColor;
	Color foregroundColor;
	Color contourColor;

	std::vector<TextBox const*> textBoxes{};
	ButtonManager zoneManager;
public:
	ButtonManager buttonManager;

	ScrollZone() = delete;
	ScrollZone(const InputEvent& input, const Window& window, const Point2D<int>& pos,
		int width, int height, int renderW, int renderH)
		: buttonManager(input, window), zoneManager(input, window), inputEvent(input), pos(pos), width(width), height(height),
		renderPos(0, 0), renderW(renderW), renderH(renderH)
	{
		texture = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, renderW, renderH);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		zoneManager.addRectTextButton<Point2D<int>*>("up", pos + Point2D<int>(width - 11, 1), 10, 10, "");
		zoneManager.getButton<Point2D<int>*>("up").setAction([](Point2D<int>* renderPosition) {renderPosition->y -= 10; }, &renderPos);
		zoneManager.getButton<Point2D<int>*>("up").contourCol = Color(120, 120, 120, 255);
		zoneManager.addRectTextButton<Point2D<int>*>("down", pos + Point2D<int>(width - 11, height - 11), 10, 10, "");
		zoneManager.getButton<Point2D<int>*>("down").setAction([](Point2D<int>* renderPosition) {renderPosition->y += 10; }, &renderPos);
		zoneManager.getButton<Point2D<int>*>("down").contourCol = Color(120, 120, 120, 255);
		zoneManager.getButton<Point2D<int>*>("down").setTexture(new Texture2D("../textures/32x32.gif", window.getRenderer()));
		backgroundColor = Color(60, 60, 60, 255);
		foregroundColor = Color(90, 90, 90, 255);
		contourColor = Color(120, 120, 120, 255);
	}
	ScrollZone(const InputEvent& input, const Window& window, const Point2D<int>& pos,
		int width, int height, const Point2D<int>& renderPos, int renderW, int renderH,
		const Color& bgcolor, const Color& fgcolor, const Color& contcolor)
		: buttonManager(input, window), zoneManager(input, window), inputEvent(input), pos(pos), width(width), height(height),
		renderPos(renderPos), renderW(renderW), renderH(renderH), backgroundColor(bgcolor), foregroundColor(fgcolor), contourColor(contcolor)
	{
		texture = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, renderW, renderH);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		zoneManager.addRectTextButton<Point2D<int>*>("up", Point2D<int>(pos.x + width - 26, pos.y), 25, 20, "");
		zoneManager.getButton<Point2D<int>*>("up").setAction([](Point2D<int>* renderPosition) {renderPosition->y -= 10; }, &this->renderPos);
		zoneManager.addRectTextButton<Point2D<int>*>("down", Point2D<int>(pos.x + width - 26, pos.y + height - 20), 25, 20, "");
		zoneManager.getButton<Point2D<int>*>("down").setAction([](Point2D<int>* renderPosition) {renderPosition->y += 10; }, &this->renderPos);
	}

	~ScrollZone() {
		SDL_DestroyTexture(texture);
		zoneManager.getButton<Point2D<int>*>("down").deleteTexture();
	}

	inline void startDrawInside(SDL_Renderer* renderer) { SDL_SetRenderTarget(renderer, texture); }
	inline void stopDrawInside(SDL_Renderer* renderer) { SDL_SetRenderTarget(renderer, NULL); }

	void addTextBox(const TextBox& tb) {
		textBoxes.push_back(&tb);
	}

	void setBackGroundColor(const Color& color) {
		backgroundColor = color;
	}

	void setForeGroundColor(const Color& color) {
		foregroundColor = color;
	}

	void setContourColor(const Color& color) {
		contourColor = color;
	}

	void setPosition(const Point2D<int> point) {
		pos = point;
	}

	void setRenderPosition(const Point2D<int> point) {
		renderPos = point;
		renderPos.x = Maths::clamp(renderPos.x, -renderW, width);
		renderPos.y = Maths::clamp(renderPos.y, -renderH, height);
	}

	bool mouseInside() const {
		return mouse.x < pos.x + width && mouse.x > pos.x && mouse.y < pos.y + height && mouse.y > pos.y;
	}

	void update() {
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		if (mouseInside()) {
			zoneManager.checkButtons();
			buttonManager.checkButtons(pos + renderPos);
			if (keyboard.shift.pressed) {
				renderPos.x -= mouse.wheelup * 4;
				renderPos.x += mouse.wheeldown * 4;
			}
			else {
				renderPos.y += mouse.wheelup * 4;
				renderPos.y -= mouse.wheeldown * 4;
			}
			renderPos.x = Maths::clamp(renderPos.x, -renderW, width);
			renderPos.y = Maths::clamp(renderPos.y, -renderH, height);
		}
		else {
			//zoneManager.unselectButtons();
			buttonManager.unselectButtons();
		}
	}

	void render(const Window& window) const {
		SDL_SetRenderTarget(window.getRenderer(), texture);
		buttonManager.renderButtons(window.getRenderer());
		for (auto& tb : textBoxes) tb->render(window.getRenderer());
		SDL_SetRenderTarget(window.getRenderer(), NULL);
		int srcx, srcy, srcw, srch;
		int dstx, dsty, dstw, dsth;

		if (renderPos.x > width) {
			srcx = srcw = dstx = dstw = 0;
		}
		else if (renderPos.x < 0) {
			srcx = -renderPos.x;
			srcw = Maths::clamp(renderW + renderPos.x, 0, width);
			dstx = pos.x;
			dstw = Maths::clamp(renderW + renderPos.x, 0, width);
		}
		else if (renderPos.x + renderW > width) {
			srcx = 0;
			dstx = pos.x + renderPos.x;
			srcw = width - renderPos.x;
			dstw = width - renderPos.x;
		}
		else {
			srcx = 0;
			dstx = pos.x + renderPos.x;
			srcw = renderW;
			dstw = renderW;
		}
		if (renderPos.y > height) {
			srcy = srch = dsty = dsth = 0;
		}
		else if (renderPos.y < 0) {
			srcy = -renderPos.y;
			srch = Maths::clamp(renderH + renderPos.y, 0, height);
			dsty = pos.y;
			dsth = Maths::clamp(renderH + renderPos.y, 0, height);
		}
		else if (renderPos.y + renderH > height) {
			srcy = 0;
			dsty = pos.y + renderPos.y;
			srch = height - renderPos.y;
			dsth = height - renderPos.y;
		}
		else {
			srcy = 0;
			dsty = pos.y + renderPos.y;
			srch = renderH;
			dsth = renderH;
		}
		const SDL_Rect srcrect{ srcx,srcy,srcw,srch };
		const SDL_Rect dstrect{ dstx,dsty,dstw,dsth };
		Draw::DrawFillRect(pos, width, height, backgroundColor, window.getRenderer());
		Draw::DrawFillRect(Point2D<int>(dstx,dsty), dstw, dsth, foregroundColor, window.getRenderer());
		SDL_RenderCopy(window.getRenderer(), texture, &srcrect, &dstrect);
		zoneManager.renderButtons(window.getRenderer());
		Draw::DrawRect(pos, width, height, 1, contourColor, window.getRenderer());
	}
};
