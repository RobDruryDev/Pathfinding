#pragma once

#include <unordered_map>

//#include <boost/container/flat_set.hpp>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>;

#include "Vector2.h"

class Button
{
private:
	const SDL_Color BORDER_COLOR{ 255, 0, 0, 255 };
	const SDL_Color HOVER_COLOR{ 255, 255, 255, 128 };
	const SDL_Color CLICK_COLOR{ 255, 255, 255, 255 };

	const SDL_Color TEXT_COLOR{ 255, 255, 255, 255 };
	const SDL_Color TEXT_HOVER_COLOR{ 0, 0, 0, 255 };
	static _STD unordered_map<size_t, _STD pair<int, TTF_Font*>> _font_map;
	//static boost::container::flat_set<Button> _buttons;

	bool _dirty, _hovered;
	size_t _fontHash;
	SDL_Event _clickEv;
	_STD string _text;

#if _DEBUG
	_STD string _font;
#endif

	SDL_FRect _rect, _padding;
	SDL_Texture* _tex = nullptr;
	

public:
	Button(const SDL_FRect& rect, _STD string&& font, _STD string&& text = "");
	~Button();

	void SetText(const _STD string& text)
	{
		_text = text;
		_dirty = true;
	}

	void SetPosition(const Vector2F& pos)
	{
		_rect.x = pos.x;
		_rect.y = pos.y;
		_dirty = true;
	}

	void SetSize(const Vector2F& size)
	{
		_rect.w = size.x;
		_rect.h = size.y;
		_dirty = true;
	}

	void SetPadding(float left, float right, float top, float bottom)
	{
		_padding.x = left;
		_padding.y = right;
		_padding.w = top;
		_padding.h = bottom;
		_dirty = true;
	}

	void SetClicEvent(Uint32 ev)
	{
		_clickEv.type = ev;
	}

	void SetHovered(bool hovered)
	{
		_dirty = _dirty || _hovered != hovered;
		_hovered = hovered;
	}

	bool Overlaps(const Vector2F& pos)
	{
		return pos.x > _rect.x && pos.x < _rect.x + _rect.w &&
			   pos.y > _rect.y && pos.y < _rect.y + _rect.h;
	}

	void OnClick()
	{
		SDL_PushEvent(&_clickEv);
	}

	void Render(SDL_Renderer* r);
};

