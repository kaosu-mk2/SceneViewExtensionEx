#include "MonocroSettings.h"

FMonocroSettings::FMonocroSettings()
{
	//�\���̂̃f�t�H���g�l���J�X�^���������̂Ń��������[���N���A����
	FMemory::Memzero(this, sizeof(FMonocroSettings));

	Enabled = false;
	Weight = 1.0f;
}