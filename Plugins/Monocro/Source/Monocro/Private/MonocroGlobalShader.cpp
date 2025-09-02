#include "MonocroGlobalShader.h"

// 実際に使うシェーダーを登録
IMPLEMENT_GLOBAL_SHADER(FMonocroPS, "/Plugin/Monocro/Private/Monocro.usf", "MainPS", SF_Pixel);