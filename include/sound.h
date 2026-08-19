#pragma once
#include <SDL3/SDL_audio.h>

constexpr int SOUND_CHANNELS = 1;
constexpr int SOUND_RATE = 48000;
constexpr float SOUND_VOLUME = 0.02f;

constexpr float SOUND_SHORT_TIME = 0.016f;
constexpr float SOUND_LONG_TIME = 0.27f;

typedef struct {
	float freq;
	float time;
} SoundType;

constexpr SoundType SOUND_START{983.0f, SOUND_SHORT_TIME};

constexpr SoundType SOUND_SCORE{246.0f, SOUND_LONG_TIME };

constexpr SoundType SOUND_HIT{492.0f, SOUND_SHORT_TIME};

constexpr SoundType SOUND_BOUNCE{983.0f, SOUND_SHORT_TIME };

class Sound {
	SDL_AudioStream* mStream{};

	void Cleanup();

public:
	Sound() = default;
	bool Init();
	void Play(SoundType st);
};