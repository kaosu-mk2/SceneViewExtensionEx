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

	//�V�F�[�_�[���ɓn���p�����[�^���`
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
		SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
		SHADER_PARAMETER(float, Weight)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		// �Ή�����v���b�g�t�H�[�����ǂ����������ɏ���(����͑S�Ẵv���b�g�t�H�[���ɑΉ�)
		return true;
	}
};