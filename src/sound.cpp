#include <vector>
#include "sound.h"

bool Sound::Init() {
	SDL_AudioSpec spec = {SDL_AUDIO_F32, SOUND_CHANNELS, SOUND_RATE };
	mStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
	if (!mStream) {
		return false;
	}
	if (!SDL_ResumeAudioStreamDevice(mStream)) {
		Cleanup();
		return false;
	}
	return true;
}

void Sound::Cleanup() {
	if (mStream) {
		SDL_DestroyAudioStream(mStream);
		mStream = nullptr;
	}
}

void Sound::Play(SoundType st) {
	if (!mStream) return;
	const int samples_per_wave = static_cast<int>(SOUND_RATE / st.freq);
	const int total_samples = static_cast<int>(SOUND_RATE * st.time);
	if (SDL_GetAudioStreamQueued(mStream)) return;

	if (samples_per_wave < 2 || total_samples <= 0) return;

	std::vector<float> audio_buffer(total_samples);

	for (int i = 0; i < audio_buffer.size(); i++) {
		audio_buffer[i] = (i % samples_per_wave) < (samples_per_wave / 2) ? SOUND_VOLUME : -SOUND_VOLUME;
	}

	SDL_PutAudioStreamData(mStream, audio_buffer.data(), static_cast<int>(audio_buffer.size() * sizeof(float)));
}