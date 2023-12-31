#include "PreCompile.h"
#include "PlayMap.h"

PlayMap* PlayMap::MainMap;

PlayMap::PlayMap() 
{
	MainMap = this;
}

PlayMap::~PlayMap() 
{
}

void PlayMap::Start()
{

	if (nullptr == GameEngineSprite::Find("CheckpointBack.png"))
	{
		GameEngineSprite::CreateSingle("CheckpointBack.png");
		//GameEngineSprite::CreateCut("")
		GameEngineSprite::CreateSingle("Desk.png");
		//GameEngineSprite::CreateSingle("Console.png");
		GameEngineSprite::CreateSingle("BoothWall.png");

	}


	{
		{
			std::shared_ptr<GameEngineSpriteRenderer> OutsideRenderer = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::BackGround);
			OutsideRenderer->SetSprite("CheckpointBack.png");
			OutsideRenderer->SetRenderOrder(RenderOrder::BackGround);
			//OutsideRenderer->Transform.SetLocalPosition(GameEngineCore::MainWindow.GetScale().Half());
			OutsideRenderer->AutoSpriteSizeOn();
			OutsideRenderer->SetAutoScaleRatio(2.0f);
			std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find("CheckpointBack.png");
		
			float4 HScale = Tex->GetScale();
			HScale.Y *= -1.0f;

			OutsideRenderer->Transform.SetLocalPosition(HScale);
		}

		{
			BoothWall = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::BackGround);
			BoothWall->SetSprite("BoothWall.png");
			BoothWall->SetRenderOrder(RenderOrder::BackGround);
			//OutsideRenderer->Transform.SetLocalPosition(GameEngineCore::MainWindow.GetScale().Half());
			BoothWall->AutoSpriteSizeOn();
			BoothWall->SetAutoScaleRatio(2.0f);
			std::shared_ptr<GameEngineTexture> OutsideRendererTex = GameEngineTexture::Find("CheckpointBack.png");
			std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find("BoothWall.png");
			float4 OSRScale = OutsideRendererTex->GetScale();
			OSRScale.Y *= -1.0f;
			float4 HScale = Tex->GetScale();
			HScale.Y *= -1.0f;
			BoothWall->Transform.SetLocalPosition({HScale.X,HScale.Y+ OSRScale.Y*2.0f});

			
		}

		{
			Desk = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::BackGround);
			Desk->SetSprite("Desk.png");
			Desk->SetRenderOrder(RenderOrder::BackGround);
			//OutsideRenderer->Transform.SetLocalPosition(GameEngineCore::MainWindow.GetScale().Half());
			Desk->AutoSpriteSizeOn();
			Desk->SetAutoScaleRatio(2.0f);
			std::shared_ptr<GameEngineTexture> OutsideRendererTex = GameEngineTexture::Find("CheckpointBack.png");
			std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find("Desk.png");
			float4 OSRScale = OutsideRendererTex->GetScale();
			OSRScale.Y *= -1.0f;
			float4 HScale = Tex->GetScale();
			HScale.Y *= -1.0f;
			Desk->Transform.SetLocalPosition({ HScale.X,HScale.Y + OSRScale.Y * 2.0f });
			
		}

		{

		}

	}


}

GameEngineColor PlayMap::GetColor(float4 _Pos, GameEngineColor _DefaultColor)
{
	// 플레이어의 위치를 이미지의 좌표계로 변경한다.
	// 이미지는 위에서부터 아래로 내려갈수록 +가 되기 때문이다.
	_Pos.Y *= -1.0f;

	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find("TestMap.png");


	return Tex->GetColor(_Pos, _DefaultColor);
}