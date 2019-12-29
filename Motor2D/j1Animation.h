#ifndef __j1ANIMATION_H__
#define __j1ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "j1App.h"
#define MAX_FRAMES 25

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	float current_frame = 0.0f;
	int last_frame = 0;
	bool lock = false;

private:
	int loops = 0;

public:


	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		if (!App->pause)
			current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;

		}
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
		loops = 0;
	}
};

#endif
