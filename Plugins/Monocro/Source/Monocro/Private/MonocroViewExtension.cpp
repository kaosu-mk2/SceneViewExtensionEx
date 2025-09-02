#include "MonocroViewExtension.h"

#include "MonocroGlobalShader.h"
#include "MonocroSubsystem.h"
#include "RenderGraphEvent.h"
#include "ScenePrivate.h"
#include "ScreenPass.h"
#include "ScreenRendering.h"
#include "PostProcess/PostProcessInputs.h"

DECLARE_GPU_STAT(Monocro);

FMonocroViewExtension::FMonocroViewExtension(const FAutoRegister& AutoRegister)
	: FSceneViewExtensionBase(AutoRegister)
{
}

void FMonocroViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
	Inputs.Validate();

	FScene* Scene = View.Family->Scene->GetRenderScene();

	const UMonocroSubsystem* Subsystem = UMonocroSubsystem::Get(Scene->World);
	if (!IsValid(Subsystem))
	{
		return;
	}

	const FMonocroSettings& Settings = Subsystem->GetMonocroSettings();
	if (!Settings.Enabled)
	{
		return;
	}

	const FIntRect PrimaryViewRect = static_cast<const FViewInfo&>(View).ViewRect;

	// PrePostProcessPass�Ȃ̂Ń|�X�g�v���Z�X�O�̕`�挋�ʂ��擾
	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

	RDG_EVENT_SCOPE(GraphBuilder, "Monocro");
	RDG_GPU_STAT_SCOPE(GraphBuilder, Monocro);

	const FScreenPassTextureViewport InputViewport(SceneColor);
	const FScreenPassTextureViewport OutputViewport(InputViewport);

	// FScreenVS=�t���X�N���[���V�F�[�_�[����肽���ꍇ�̒��_�V�F�[�_�[�̃v���Z�b�g
	TShaderMapRef<FScreenVS> VertexShader(static_cast<const FViewInfo&>(View).ShaderMap);
	TShaderMapRef<FMonocroPS> PixelShader(static_cast<const FViewInfo&>(View).ShaderMap);

	// �����_�����O���ʂ��i�[�p�̃e�N�X�`�����쐬
	FRDGTextureRef OutputTexture;
	{
		FRDGTextureDesc OutputTextureDesc = SceneColor.Texture->Desc;
		OutputTextureDesc.Reset();
		OutputTextureDesc.Flags |= TexCreate_RenderTargetable | TexCreate_ShaderResource;
		// GraphBuilder.CreateTexture�ō쐬�����e�N�X�`���͎����I�ɔj�����Ă����
		OutputTexture = GraphBuilder.CreateTexture(OutputTextureDesc, TEXT("Monocro_Output"));
	}

	FMonocroPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FMonocroPS::FParameters>();
	PassParameters->View = View.ViewUniformBuffer;
	PassParameters->SceneTextures = GetSceneTextureShaderParameters(Inputs.SceneTextures);
	PassParameters->Weight = Settings.Weight;
	// �o�͐�̃e�N�X�`����ݒ�(ERenderTargetLoadAction::EClear�ɂ��邱�ƂŃo�b�t�@���N���A���Ă��珑������ł����)
	PassParameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::EClear);

	// ���̃p�X���Ŏg��Ȃ��e�N�X�`�����_�~�[�̍��e�N�X�`���ɒu��������
	const FScreenPassTexture BlackDummy(GSystemTextures.GetBlackDummy(GraphBuilder));
	GraphBuilder.RemoveUnusedTextureWarning(BlackDummy.Texture);

	// �w�肵�������_�[�^�[�Q�b�g�ɑ΂��ĕ`�揈�����s
	AddDrawScreenPass(
		GraphBuilder,
		RDG_EVENT_NAME("Monocro"),
		View,
		OutputViewport,
		InputViewport,
		VertexShader,
		PixelShader,
		TStaticBlendState<>::GetRHI(),
		TStaticDepthStencilState<false, CF_Always>::GetRHI(),
		PassParameters);

	// �X�N���[���ɔ��f
	AddCopyTexturePass(GraphBuilder, OutputTexture, SceneColor.Texture);
}
