#include "net.h"
#include "config.h"
#include <algorithm>

void RenderNet(SDL_Renderer* renderer, float xCenter, float length) {
	SDL_FRect rect = {
		.x = xCenter - NET_WIDTH / 2,
		.y = 0,
		.w = NET_WIDTH,
		.h = 0
	};
	float remaining;
	bool isDash = true;
	while (rect.y < length) {
		remaining = length - rect.y;
		if (isDash) {
			rect.h = std::min((float)NET_STRIP_SIZE, remaining);
			SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
			SDL_RenderFillRect(renderer, &rect);
			rect.y += rect.h;
		}
		else {
			rect.y += std::min((float)NET_GAP, remaining);
		}
		isDash = !isDash;
	}
}