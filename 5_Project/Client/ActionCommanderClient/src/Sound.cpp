#include "pch.h"
#include "Sound.h"

#include <fmod.hpp>

IMPLEMENT_SINGLETON_CLASS(Sound);

void Sound::InitSound()
{
	m_Result = FMOD::System_Create(&m_System, FMOD_VERSION);
	m_Result = m_System->init(8, FMOD_INIT_NORMAL, 0);

	// 하나씩 만들어서 넣기
	//BGM
	FMOD::Sound* _mainBGM;
	m_System->createSound("Assets/Sound/Main_BGM.wav", FMOD_LOOP_OFF, 0, &_mainBGM);
	m_BGMSound.emplace("Main_BGM.wav", _mainBGM);

	FMOD::Sound* _clearBGM;
	m_System->createSound("Assets/Sound/Clear_BGM.wav", FMOD_LOOP_OFF, 0, &_clearBGM);
	m_BGMSound.emplace("Clear_BGM.wav", _clearBGM);

	FMOD::Sound* _gameOverBGM;
	m_System->createSound("Assets/Sound/GameOver_BGM.wav", FMOD_LOOP_OFF, 0, &_gameOverBGM);
	m_BGMSound.emplace("GameOver_BGM.wav", _gameOverBGM);

	FMOD::Sound* _titleBGM;
	m_System->createSound("Assets/Sound/Title_BGM.wav", FMOD_LOOP_OFF, 0, &_titleBGM);
	m_BGMSound.emplace("Title_BGM.wav", _titleBGM);

	// Effect
	FMOD::Sound* _dash;
	m_System->createSound("Assets/Sound/Character/Dash.wav", FMOD_LOOP_OFF, 0, &_dash);
	m_EffectSound.emplace("Dash.wav", _dash);

	FMOD::Sound* _jump;
	m_System->createSound("Assets/Sound/Character/Jump.wav", FMOD_LOOP_OFF, 0, &_jump);
	m_EffectSound.emplace("Jump.wav", _jump);

	FMOD::Sound* _skillGroundHit;
	m_System->createSound("Assets/Sound/Character/Skill_Ground_Hit.wav", FMOD_LOOP_OFF, 0, &_skillGroundHit);
	m_EffectSound.emplace("Skill_Ground_Hit.wav", _skillGroundHit);

	FMOD::Sound* _swordHit;
	m_System->createSound("Assets/Sound/Character/Sword_Hit.wav", FMOD_LOOP_OFF, 0, &_swordHit);
	m_EffectSound.emplace("Sword_Hit.wav", _swordHit);

	FMOD::Sound* _swordSwing;
	m_System->createSound("Assets/Sound/Character/Sword_Swing.wav", FMOD_LOOP_OFF, 0, &_swordSwing);
	m_EffectSound.emplace("Sword_Swing.wav", _swordSwing);

	FMOD::Sound* _skillGroundHit2;
	m_System->createSound("Assets/Sound/Character/Skill_Ground_Hit2.wav", FMOD_LOOP_OFF, 0, &_skillGroundHit2);
	m_EffectSound.emplace("Skill_Ground_Hit.wav", _skillGroundHit2);

	FMOD::Sound* _skillVoice1;
	m_System->createSound("Assets/Sound/Character/Skill_Voice1.wav", FMOD_LOOP_OFF, 0, &_skillVoice1);
	m_EffectSound.emplace("Skill_Voice1.wav", _skillVoice1);

	FMOD::Sound* _skillVoice2;
	m_System->createSound("Assets/Sound/Character/Skill_Voice2.wav", FMOD_LOOP_OFF, 0, &_skillVoice2);
	m_EffectSound.emplace("Skill_Voice2.wav", _skillVoice2);
}

void Sound::UpdateSystem()
{
	m_System->update();
}

void Sound::PlaySound(std::string _soundName)
{
	StopMusic();
	m_System->playSound(m_BGMSound.at(_soundName), 0, false, &m_BGMChannel);
}

void Sound::PlayLoop(std::string _soundName)
{
	bool _playing = false;
	m_BGMChannel->isPlaying(&_playing);
	if (!_playing)
	{
		m_System->playSound(m_BGMSound.at(_soundName), 0, false, &m_BGMChannel);
	}
}

void Sound::PlayEffect(std::string _soundEffectName)
{
	bool _effectPlaying = false;

	// 빈 채널을 찾아서 재생 시킨다.
	for (int i = 0; i < 20; i++)
	{
		m_EffectChannel[i]->isPlaying(&_effectPlaying);
		if (!_effectPlaying)
		{
			m_System->playSound(m_EffectSound.at(_soundEffectName), 0, false, &m_EffectChannel[i]);
		}
	}
}

void Sound::StopMusic()
{
	m_BGMChannel->stop();
}

void Sound::PauseMusic()
{
	m_BGMChannel->setPaused(true);
}

void Sound::RePlayMusic()
{
	m_BGMChannel->setPaused(false);
}

void Sound::ReleaseSound()
{
	m_System->release();
}
