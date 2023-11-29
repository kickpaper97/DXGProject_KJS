#include "PreCompile.h"
#include "PassPort.h"
#include "NormalTraveler.h"
#include "CustomSpriteRenderer.h"
#include "Cursor.h"
#include "InkApproved.h"

PassPort::PassPort()
{
}

PassPort::~PassPort()
{
}

void PassPort::SetOwner(std::shared_ptr<class NormalTraveler> _Owner)
{
	Owner = _Owner;
	Info = _Owner.get()->GetInfo();


	switch (Info.OriginCountry)
	{
	case Country::Arstotzka:
		SetPaperTexture("PassportArstotzka");
		break;
	case Country::Antegria:
		SetPaperTexture("PassportAntegria");

		break;
	case Country::Impor:
		SetPaperTexture("PassportAImpor");

		break;
	case Country::Kolechia:
		SetPaperTexture("PassportKolechia");

		break;
	case Country::Obristan:
		SetPaperTexture("PassportObristan");

		break;
	case Country::Republia:
		SetPaperTexture("PassportRepublia");

		break;
	case Country::United_Federation:
		SetPaperTexture("PassportUnitedFed");


		break;


	default:


		break;
	}


	

	
	
	
	



	

	
	//Picture->Transform.SetLocalPosition()


}


void PassPort::StampPassPort(PassPortChecked _Check, float4 _WorldStampPos)
{
	std::shared_ptr<CustomSpriteRenderer>StampSpriteRenderer=  std::dynamic_pointer_cast<CustomSpriteRenderer>(InnerRenderer->CreateChild<CustomSpriteRenderer>(10));
	
	//std::shared_ptr<CustomSpriteRenderer>StampSpriteRenderer = CreateComponent <CustomSpriteRenderer>();
	StampSpriteRenderer->AutoSpriteSizeOn();
	StampSpriteRenderer->SetAutoScaleRatio(2.0f);

	StampSpriteRenderer->SetMaskTexture("Desk_Mask.png");
	


	switch (_Check)
	{
	case PassPortChecked::Approved:

	{
		//std::shared_ptr<InkApproved> NewInk = CreateComponent<InkApproved>();

	}
	StampSpriteRenderer->SetSprite("InkApproved.png");


		if (EntryCheck == PassPortChecked::Yet)
		{
			EntryCheck = PassPortChecked::Approved;
		}
		
		break;
	case PassPortChecked::Denied:
		StampSpriteRenderer->SetSprite("InkDenied.png");
		if (EntryCheck == PassPortChecked::Yet)
		{
			EntryCheck = PassPortChecked::Denied;
		}

		break;


	
	default:
		break;
	}
	std::string ad = InnerRenderer->GetSprite()->GetName().data();
	StampSpriteRenderer->SetPassPortTexture(InnerRenderer->GetSprite()->GetName(), Transform.GetLocalPosition());


	float4 StampPos = this->Transform.GetWorldPosition() - _WorldStampPos;
	StampPos *= -1;
	StampSpriteRenderer->RenderBaseInfoValue.RendererScreenPos = StampPos;
	StampSpriteRenderer->Transform.SetLocalPosition(StampPos);
	
}

void PassPort::Start()
{
	
	

	if (nullptr == GameEngineSprite::Find("InkApproved.png"))
	{
		GameEngineSprite::CreateSingle("InkApproved.png");
		GameEngineSprite::CreateSingle("InkDenied.png");

	}



	if (nullptr == GameEngineSprite::Find("dSheetF0.Png"))
	{
		
		GameEngineSprite::CreateCut("dSheetF0.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetF1.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetF2.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetF3.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetF4.png", 2, 2);

		GameEngineSprite::CreateCut("dSheetM0.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetM1.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetM2.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetM3.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetM4.png", 2, 2);
		GameEngineSprite::CreateCut("dSheetM5.png", 2, 2);


	}



	PaperBase::Start();






	
	
	
	Transform.SetLocalPosition({150,-450 });
	GameEngineInput::AddInputObject(this);


}

void PassPort::Update(float _Delta)
{
	PaperBase::Update(_Delta);

	if (GameEngineInput::IsDown(VK_RBUTTON, this))
	{
		float4 pos = Cursor::MainCursor->Transform.GetWorldPosition();
		StampPassPort(PassPortChecked::Approved, pos );



	}


	//this->Childs


}

void PassPort::SetPaperTexture(std::string_view _Name)
{

	std::string Name = _Name.data();

	//std::shared_ptr< GameEngineSprite > InnerSprite= GameEngineSprite::Find(Name + "Inner.png");
	//std::shared_ptr< GameEngineSprite > OuterSprite = GameEngineSprite::Find(Name + "Outer.png");

	if (nullptr == GameEngineSprite::Find(Name + "Inner.png"))
	{
		GameEngineSprite::CreateSingle(Name + "Inner.png");
		GameEngineSprite::CreateSingle(Name + "Outer.png");

	}



	PaperBase::SetPaperTexture(Name + "Inner.png", Name + "Outer.png");
	OuterRenderer->SetAutoScaleRatio(1.0f);


	Picture = CreateComponent<GameEngineSpriteRenderer>();
	Picture->SetMaskTexture("Desk_Mask.png");
	//Picture->ChangeParent(dynamic_cast<GameEngineObject*>(InnerRenderer));
	std::string PictureName = "d" + Info.Face.SheetName;
	Picture->CreateAnimation("FacePicture", PictureName, 0.1f, Info.Face.SheetX * 2 + Info.Face.SheetY, Info.Face.SheetX * 2 + Info.Face.SheetY, false);
	Picture->ChangeAnimation("FacePicture");
	Picture->AutoSpriteSizeOn();
	Picture->SetAutoScaleRatio(0.5f);
	//Picture->Transform.SetLocalScale({})
		
	
	/*InnerRenderer->SetSprite(Name + "Inner.png");
	OuterRenderer->SetSprite(Name + "Outer.png");*/



}



