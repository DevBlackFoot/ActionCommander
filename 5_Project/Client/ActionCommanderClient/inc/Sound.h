#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

#include <fmod.hpp>
#include <fmod_errors.h>
#include <unordered_map>
#include <string>


namespace SanyoniEngine
{

	class Sound final
	{
		DECLARE_SINGLETON_CLASS(Sound)

	public:
		void PlaySound(std::string _soundName);
		void PlayLoop(std::string _soundName);
		void PlayEffect(std::string _soundEffectName);

		void StopMusic();
		void PauseMusic();
		void RePlayMusic();
		void ReleaseSound();

		void InitSound();

		void UpdateSystem();

	private:
		// ���带 ����� �ý���
		FMOD::System* m_System;

		std::unordered_map<std::string, FMOD::Sound*> m_BGMSound;
		FMOD::Channel* m_BGMChannel;

		std::unordered_map<std::string, FMOD::Sound*> m_EffectSound;
		// ����Ʈ�� ��� ���� �� �鸱 �� �ֱ� ������ ä���� ��������.
		FMOD::Channel* m_EffectChannel[20];

		FMOD_RESULT m_Result;
	};
}

