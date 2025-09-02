#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "SceneTexturesConfig.h"
#include "SceneView.h"

class FMonocroPS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FMonocroPS);
	SHADER_USE_PARAMETER_STRUCT(FMonocroPS, FGlobalShader);

	//シェーダー側に渡すパラメータを定義
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
		SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
		SHADER_PARAMETER(float, Weight)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		// 対応するプラットフォームかどうかをここに書く(今回は全てのプラットフォームに対応)
		return true;
	}
};