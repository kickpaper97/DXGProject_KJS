#include "PreCompile.h"
#include "Cursor.h"

#include "BasicActor.h"
#include<GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineCollision.h>

Cursor* Cursor::MainCursor;


Cursor::Cursor()
{
	MainCursor = this;
}

Cursor::~Cursor()
{
}

void Cursor::Start()
{
	{
	SetOrder(GameObjectType::Cursor);
	Renderer = CreateComponent<GameEngineUIRenderer>();
	Renderer->SetSprite("CursorArrow.png");
	Renderer->SetRenderOrder(RenderOrder::Cursor);
	Renderer->SetPivotType(PivotType::LeftTop);
	

	}


	/*{
		CursorCollision = CreateComponent<GameEngineCollision>();
		CursorCollision->Transform.SetLocalScale({ 5.0f,5.0f,1.0f });
		CursorCollision->SetCollisionType(ColType::AABBBOX2D);
		float4 CollisionScale = CursorCollision->Transform.GetLocalScale();
		CursorCollision->Transform.AddLocalPosition({ CollisionScale.hX(),-CollisionScale.hY() });
	}*/

}

void Cursor::Update(float _Delta)
{

	{
	float4 CusorPos = GetLevel()->GetMainCamera()->GetWorldMousePos2D();
	
	
	Transform.SetLocalPosition(CusorPos);
	
	}
	/*if (콜리전조건)
	{
	Renderer->SetSprite("CursorHand.png");

	}
	else
	{
	Renderer->SetSprite("CursorArrow.png");
	}*/
}
