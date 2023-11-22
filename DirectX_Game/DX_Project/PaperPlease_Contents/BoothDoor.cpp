#include "PreCompile.h"
#include "BoothDoor.h"

BoothDoor::BoothDoor()
{
}

BoothDoor::~BoothDoor()
{
}

void BoothDoor::Start()
{
	if (nullptr == GameEngineSprite::Find("BlackSquare20x20.Png"))
	{

		GameEngineSprite::CreateSingle("BlackSquare20x20.png");
		
	}
	
	sprite = CreateComponent<GameEngineSpriteRenderer>();
	sprite->SetSprite("BlackSquare20x20.png");
	Scale = GameEngineCore::MainWindow.GetScale() - float4{ 0, MASKLINEPOS_Y };
	sprite->SetImageScale({Scale});
	
	//sprite->SetRenderOrder(RenderOrder::UI);
	
	sprite->Transform.SetLocalPosition({ Scale.hX(),-GameEngineCore::MainWindow.GetScale().Y +Scale.hY()});
}

void BoothDoor::Update(float _Delta)
{
	if (0.0f >= Timer)
	{
		if (Dir == 0)
		{
			Transform.AddLocalPosition(float4::DOWN*(Scale.Y/0.4f)*_Delta);
			if (-(GameEngineCore::MainWindow.GetScale().Y + Scale.hY()) >= Transform.GetLocalPosition().Y)
			{
				Transform.SetLocalPosition({ GameEngineCore::MainWindow.GetScale().hX(),-GameEngineCore::MainWindow.GetScale().Y - Scale.hY() });
				Dir = 1;
			}
			
			
		}
	}


	Timer -= _Delta;
}
