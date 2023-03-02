#pragma once

#include "Common.h"
#include "Window.h"

#include <fmod.h>
#include <fmod_errors.h>

#include <string>
#include <map>
#include <vector>

namespace AudioEngine
{
	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void Create();
		void Destroy();

		void Update();

		void LoadAudio(const char* soundName, bool loop = false, bool stream = false);
		void UnloadAudio(const char* soundName, bool loop = false, bool stream = false);
		int PlayAudio(const char* soundName, float volume = 1.0f);
		int PlayMusic(const char* soundName, float volume = 1.0f);

		void StopChannel(int channelID);
		void StopAllChannels();

		bool IsPlaying(int channelID) const;

		FMOD_SYSTEM* GetRawFMOD() { return m_fmodSys; }

	protected:
		static int ErrorCheck(FMOD_RESULT result);

	private:
		FMOD_SYSTEM* m_fmodSys;

		int m_nextChannelID;

		typedef std::map<const char*, FMOD_SOUND*> SoundMap;
		typedef std::map<int, FMOD_CHANNEL*> ChannelMap;

		SoundMap m_sounds;
		ChannelMap m_channels;

	};
}
