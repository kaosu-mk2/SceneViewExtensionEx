#include "MonocroSettings.h"

FMonocroSettings::FMonocroSettings()
{
	//構造体のデフォルト値をカスタムしたいのでメモリをゼロクリアする
	FMemory::Memzero(this, sizeof(FMonocroSettings));

	Enabled = false;
	Weight = 1.0f;
}