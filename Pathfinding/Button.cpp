#include <assert.h>
#include <iostream>
#include "Button.h"

_STD unordered_map<size_t, _STD pair<int, TTF_Font*>> Button::_font_map;

Button::Button(const SDL_FRect& rect, _STD string&& font, _STD string&& text) : _text(text), _rect(rect), _dirty(true)
{
#if _DEBUG
	_font = font;
#endif

	_fontHash = _STD hash<_STD string>()(font);
	if (_font_map.find(_fontHash) == _font_map.end())
	{
		TTF_Font* f = TTF_OpenFont(font.c_str(), 12);
		if (f == NULL)
		{
			_STD cout << TTF_GetError() << _STD endl;
		}

		_font_map[_fontHash] = _STD make_pair(1, f);
	}
	else
		++_font_map[_fontHash].first;
	
}

Button::~Button()
{
	if (_tex)
	{
		SDL_DestroyTexture(_tex);
		_tex = nullptr;
	}

	if (_font_map.find(_fontHash) == _font_map.end())
		return;

	--_font_map[_fontHash].first;
	if (_font_map[_fontHash].first <= 0)
	{
		TTF_CloseFont(_font_map[_fontHash].second);
		_font_map.erase(_fontHash);
	}
}

void Button::Render(SDL_Renderer* r)
{
	static SDL_FRect text_rect =
	{
		_rect.x + _padding.x,
		_rect.y, 0, 0
	};

	if (_dirty)
	{
#if _DEBUG
		assert(_font_map.find(_fontHash) != _font_map.end());
#endif
		int w, h;
		if (TTF_SizeText(_font_map[_fontHash].second, _text.c_str(), &w, &h) != 0)
		{
			_STD cout << TTF_GetError() << _STD endl;
		}

		text_rect.w = w;
		text_rect.h = h;

		// this is incorrect... the position will shift
		_rect.y = text_rect.y - _padding.w;
		_rect.w = w + _padding.x + _padding.y; 
		_rect.h = h + _padding.w + _padding.h;

		SDL_Surface* button_surf = TTF_RenderText_Blended(_font_map[_fontHash].second, _text.c_str(), _hovered ? TEXT_HOVER_COLOR : TEXT_COLOR);
		_tex = SDL_CreateTextureFromSurface(r, button_surf);
		_dirty = false;
	}

	SDL_Color initial;
	SDL_GetRenderDrawColor(r, &initial.r, &initial.g, &initial.b, &initial.a);
	
	if (_hovered)
	{
		SDL_SetRenderDrawColor(r, HOVER_COLOR.r, HOVER_COLOR.g, HOVER_COLOR.b, HOVER_COLOR.a);
		SDL_RenderFillRect(r, &_rect);
	}

	SDL_SetRenderDrawColor(r, BORDER_COLOR.r, BORDER_COLOR.g, BORDER_COLOR.b, BORDER_COLOR.a);

	SDL_RenderLine(r, _rect.x, _rect.y, _rect.x + _rect.w, _rect.y);
	SDL_RenderLine(r, _rect.x, _rect.y, _rect.x, _rect.y + _rect.h);
	SDL_RenderLine(r, _rect.x + _rect.w, _rect.y, _rect.x + _rect.w, _rect.y + _rect.h);
	SDL_RenderLine(r, _rect.x, _rect.y + _rect.h, _rect.x + _rect.w, _rect.y + _rect.h);

	SDL_SetRenderDrawColor(r, initial.r, initial.g, initial.b, initial.a);

	if (SDL_RenderTexture(r, _tex, NULL, &text_rect) != 0)
	{
		_STD cout << SDL_GetError() << _STD endl;
	}
}