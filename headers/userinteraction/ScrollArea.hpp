#pragma once

class DragBar {
public:
	Point2D<int> pos{ 0,0 };
	int width{ 0 };
	int height{ 0 };
	bool dragging = false;
	bool vertical = true;
	int padPos{ 0 };
	int padSize{ 0 };

	double normalizedPadPos{ 0 };

	Mouse mouse;
	bool selected = false;

	DragBar(const Point2D<int>& pos, int width, int height, int padSize, bool vertical = true)
		:pos(pos), width(width), height(height), padSize(padSize), vertical(vertical) {}

	void render(SDL_Renderer* renderer) const {
		Draw::DrawFillContouredRect(pos, width, height, 1, gray, dark_gray, renderer);
		if (vertical) Draw::DrawFillRoundedRectContoured(Point2D<int>(pos.x + 1, pos.y + padPos), width-2, padSize, 3, light_gray, black, renderer);
		else Draw::DrawFillRoundedRectContoured(Point2D<int>(pos.x + padPos, pos.y + 1), padSize, height-2, 3, light_gray, black, renderer);
	}

	bool mouseInside(const InputEvent& ie, const Point2D<int>& p = { 0,0 }) {
		ie.updateMouse(mouse);
		const Point2D<int> m(mouse.x, mouse.y);
		if (vertical) {
			selected = m.x < pos.x + width + p.x && m.x > pos.x + p.x && m.y < pos.y + padPos + padSize + p.y && m.y > pos.y + padPos + p.y;
		}
		if (!vertical) {
			selected = m.x < pos.x + padPos + padSize + p.x && m.x > pos.x + padPos + p.x && m.y < pos.y + height + p.y && m.y > pos.y + p.y;
		}
		return selected;
	}

	void check(const InputEvent& ie, const Point2D<int>& p = { 0,0 }) {
		ie.updateMouse(mouse);
		if (dragging) dragging = mouse.leftClick;
		else dragging = mouseInside(ie, p) && mouse.leftClick;
		if (dragging) {
			if (vertical)padPos = mouse.y - pos.y - padSize / 2;
			else padPos = mouse.x - pos.x - padSize / 2;
		}
		if (vertical)padPos = Maths::clamp(padPos, 1, height - padSize - 2);
		else padPos = Maths::clamp(padPos, 1, width - padSize - 2);
		if (vertical)normalizedPadPos = 1 - (double)padPos / (height - padSize);
		else normalizedPadPos = 1 - (double)padPos / (width - padSize);
	}
};

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

	DragBar verticalDragBar;
	DragBar horizontalDragBar;
public:
	ButtonManager buttonManager;

	ScrollZone() = delete;
	ScrollZone(const InputEvent& input, const Window& window, const Point2D<int>& pos,
		int width, int height, int renderW, int renderH)
		: inputEvent(input), pos(pos), buttonManager(input, window), width(width), height(height),
		renderPos(0, 0), renderW(renderW), renderH(renderH),
		verticalDragBar(Point2D<int>(pos.x + width - 13, pos.y + 1), 12, height - 13, 20),
		horizontalDragBar(Point2D<int>(pos.x + 1, pos.y + height - 13), width - 13, 12, 20, false)
	{
		texture = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, renderW, renderH);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		backgroundColor = Color(60, 60, 60, 255);
		foregroundColor = Color(90, 90, 90, 255);
		contourColor = Color(120, 120, 120, 255);
	}
	ScrollZone(const InputEvent& input, const Window& window, const Point2D<int>& pos,
		int width, int height, const Point2D<int>& renderPos, int renderW, int renderH,
		const Color& bgcolor, const Color& fgcolor, const Color& contcolor)
		: inputEvent(input), pos(pos), buttonManager(input, window), width(width), height(height),
		renderPos(renderPos), renderW(renderW), renderH(renderH), backgroundColor(bgcolor), foregroundColor(fgcolor), contourColor(contcolor),
		verticalDragBar(Point2D<int>(pos.x + width - 13, pos.y + 1), 12, height - 13, 20),
		horizontalDragBar(Point2D<int>(pos.x + 1, pos.y + height - 13), width - 13, 12, 20, false)
	{
		texture = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, renderW, renderH);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	}

	~ScrollZone() {
		SDL_DestroyTexture(texture);
	}

	inline void startDrawInside(SDL_Renderer* renderer) { SDL_SetRenderTarget(renderer, texture); }
	inline void stopDrawInside(SDL_Renderer* renderer) { SDL_SetRenderTarget(renderer, NULL); }

	void linkTextBox(const TextBox& tb) {
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
		verticalDragBar.check(inputEvent);
		horizontalDragBar.check(inputEvent);
		if (verticalDragBar.dragging)renderPos.y = std::min(0, height - renderH) + (0 - std::min(0, height - renderH)) * verticalDragBar.normalizedPadPos;
		if (horizontalDragBar.dragging)renderPos.x = std::min(0, width - renderW) + (0 - std::min(0, width - renderW)) * horizontalDragBar.normalizedPadPos;
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		if (mouseInside()) {
			buttonManager.checkButtons(pos + renderPos);
			if (keyboard.shift.pressed) {
				renderPos.x -= mouse.wheelup * 4;
				renderPos.x += mouse.wheeldown * 4;
			}
			else {
				renderPos.y += mouse.wheelup * 4;
				renderPos.y -= mouse.wheeldown * 4;
			}
		}
		else {
			buttonManager.unselectButtons();
		}
		renderPos.x = Maths::clamp(renderPos.x, std::min(0, width - renderW), 0);
		renderPos.y = Maths::clamp(renderPos.y, std::min(0, height - renderH), 0);
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
		verticalDragBar.render(window.getRenderer());
		horizontalDragBar.render(window.getRenderer());
		Draw::DrawRect(pos, width, height, 1, contourColor, window.getRenderer());
	}
};
