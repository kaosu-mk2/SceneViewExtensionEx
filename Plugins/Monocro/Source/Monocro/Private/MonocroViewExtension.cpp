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

	// PrePostProcessPassなのでポストプロセス前の描画結果を取得
	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

	RDG_EVENT_SCOPE(GraphBuilder, "Monocro");
	RDG_GPU_STAT_SCOPE(GraphBuilder, Monocro);

	const FScreenPassTextureViewport InputViewport(SceneColor);
	const FScreenPassTextureViewport OutputViewport(InputViewport);

	// FScreenVS=フルスクリーンシェーダーを作りたい場合の頂点シェーダーのプリセット
	TShaderMapRef<FScreenVS> VertexShader(static_cast<const FViewInfo&>(View).ShaderMap);
	TShaderMapRef<FMonocroPS> PixelShader(static_cast<const FViewInfo&>(View).ShaderMap);

	// レンダリング結果を格納用のテクスチャを作成
	FRDGTextureRef OutputTexture;
	{
		FRDGTextureDesc OutputTextureDesc = SceneColor.Texture->Desc;
		OutputTextureDesc.Reset();
		OutputTextureDesc.Flags |= TexCreate_RenderTargetable | TexCreate_ShaderResource;
		// GraphBuilder.CreateTextureで作成したテクスチャは自動的に破棄してくれる
		OutputTexture = GraphBuilder.CreateTexture(OutputTextureDesc, TEXT("Monocro_Output"));
	}

	FMonocroPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FMonocroPS::FParameters>();
	PassParameters->View = View.ViewUniformBuffer;
	PassParameters->SceneTextures = GetSceneTextureShaderParameters(Inputs.SceneTextures);
	PassParameters->Weight = Settings.Weight;
	// 出力先のテクスチャを設定(ERenderTargetLoadAction::EClearにすることでバッファをクリアしてから書き込んでくれる)
	PassParameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::EClear);

	// このパス内で使わないテクスチャをダミーの黒テクスチャに置き換える
	const FScreenPassTexture BlackDummy(GSystemTextures.GetBlackDummy(GraphBuilder));
	GraphBuilder.RemoveUnusedTextureWarning(BlackDummy.Texture);

	// 指定したレンダーターゲットに対して描画処理実行
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

	// スクリーンに反映
	AddCopyTexturePass(GraphBuilder, OutputTexture, SceneColor.Texture);
}
