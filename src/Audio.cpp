#include "Audio.h"

#include <assert.h>

namespace AudioEngine
{
	AudioEngine::AudioEngine()
		: m_fmodSys(nullptr)
	{ }
	AudioEngine::~AudioEngine()
	{ }

	void AudioEngine::Create()
	{
		ErrorCheck(FMOD_System_Create(&m_fmodSys, FMOD_VERSION));
		FMOD_INITFLAGS initFlags = FMOD_INIT_NORMAL;
		ErrorCheck(FMOD_System_Init(m_fmodSys, 12, initFlags, nullptr));
#ifdef DEBUG_BUILD
		FMOD_DEBUG_FLAGS dFlags = FMOD_DEBUG_LEVEL_LOG;
		ErrorCheck(FMOD_Debug_Initialize(dFlags, FMOD_DEBUG_MODE::FMOD_DEBUG_MODE_TTY, nullptr, nullptr));
#endif
	}
	void AudioEngine::Destroy()
	{
		FMOD_System_Release(m_fmodSys);
	}

	void AudioEngine::Update()
	{
		std::vector<ChannelMap::iterator> stoppedChannels;
		for (auto it = m_channels.begin(), itEnd = m_channels.end(); it != itEnd; ++it)
		{
			FMOD_BOOL isPlaying = false;
			FMOD_Channel_IsPlaying(it->second, &isPlaying);
			if (!isPlaying)
			{
				stoppedChannels.push_back(it);
			}
		}
		for (auto& it : stoppedChannels)
		{
			m_channels.erase(it);
		}
		ErrorCheck(FMOD_System_Update(m_fmodSys));
	}

	void AudioEngine::LoadAudio(const char* soundName, bool loop, bool stream)
	{
		auto foundIt = m_sounds.find(soundName);
		if (foundIt != m_sounds.end())
			return;
		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		FMOD_SOUND* sound = nullptr;
		ErrorCheck(FMOD_System_CreateSound(m_fmodSys, soundName, mode, nullptr, &sound));
		if (sound)
			m_sounds[soundName] = sound;
	}
	void AudioEngine::UnloadAudio(const char* soundName, bool loop, bool stream)
	{
		auto foundIt = m_sounds.find(soundName);
		if (foundIt == m_sounds.end())
			return;
		ErrorCheck(FMOD_Sound_Release(foundIt->second));
		m_sounds.erase(foundIt);
	}
	int AudioEngine::PlayAudio(const char* soundName, float volume)
	{
		int channelID = m_nextChannelID;
		auto foundIt = m_sounds.find(soundName);
		if (foundIt == m_sounds.end())
		{
			LoadAudio(soundName);
			foundIt = m_sounds.find(soundName);
			if (foundIt == m_sounds.end())
				return channelID;
		}
		FMOD_CHANNEL* channel = nullptr;
		ErrorCheck(FMOD_System_PlaySound(m_fmodSys, foundIt->second, nullptr, true, &channel));
		if (channel)
		{
			FMOD_MODE mode;
			FMOD_Sound_GetMode(foundIt->second, &mode);
			ErrorCheck(FMOD_Channel_SetVolume(channel, volume));
			ErrorCheck(FMOD_Channel_SetPaused(channel, false));
			ErrorCheck(FMOD_Channel_SetMode(channel, mode));
			m_channels[channelID] = channel;
		}
		return channelID;
	}
	int AudioEngine::PlayMusic(const char* soundName, float volume)
	{
		int channelID = m_nextChannelID;
		auto foundIt = m_sounds.find(soundName);
		if (foundIt == m_sounds.end())
		{
			LoadAudio(soundName, true, true);
			foundIt = m_sounds.find(soundName);
			if (foundIt == m_sounds.end())
				return channelID;
		}
		FMOD_CHANNEL* channel = nullptr;
		ErrorCheck(FMOD_System_PlaySound(m_fmodSys, foundIt->second, nullptr, true, &channel));
		if (channel)
		{
			FMOD_MODE mode;
			FMOD_Sound_GetMode(foundIt->second, &mode);
			ErrorCheck(FMOD_Channel_SetVolume(channel, volume));
			ErrorCheck(FMOD_Channel_SetPaused(channel, false));
			ErrorCheck(FMOD_Channel_SetMode(channel, mode));
			m_channels[channelID] = channel;
		}
		return channelID;
	}

	void AudioEngine::StopChannel(int channelID)
	{
	}
	void AudioEngine::StopAllChannels()
	{
	}

	bool AudioEngine::IsPlaying(int channelID) const
	{
		return false;
	}

	int AudioEngine::ErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			OutputDebugString("FMOD Error.\n");
			return 1;
		}

		return 0;
	}
}
