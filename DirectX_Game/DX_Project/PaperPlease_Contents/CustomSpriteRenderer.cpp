#include "PreCompile.h"
#include "CustomSpriteRenderer.h"


void CustomFrameAnimation::EventCall(int _Frame)
{
	if (true == FrameEventFunction.contains(Index[_Frame]))
	{
		FrameEventFunction[Index[_Frame]](Parent);
	}
}

void CustomFrameAnimation::Reset()
{
	CurTime = 0.0f;
	CurIndex = 0;
	IsEnd = false;
	EventCheck = true;
}

SpriteData CustomFrameAnimation::Update(float _DeltaTime)
{
	if (true == Parent->IsPause)
	{
		return Sprite->GetSpriteData(Index[CurIndex]);
	}

	if (true == Loop)
	{
		IsEnd = false;
	}

	if (true == EventCheck && false == IsEnd)
	{
		EventCall(CurIndex);
		EventCheck = false;
	}

	CurTime += _DeltaTime;

	if (Inter[CurIndex] <= CurTime)
	{
		CurTime -= Inter[CurIndex];
		++CurIndex;
		EventCheck = true;

		if (CurIndex > End - Start)
		{
			if (nullptr != EndEvent && false == IsEnd)
			{
				EndEvent(Parent);
			}

			IsEnd = true;

			if (true == Loop)
			{
				CurIndex = 0;
			}
			else
			{
				--CurIndex;
			}
		}
	}

	return Sprite->GetSpriteData(Index[CurIndex]);
}

CustomSpriteRenderer::CustomSpriteRenderer()
{
}

CustomSpriteRenderer::~CustomSpriteRenderer()
{
}

void CustomSpriteRenderer::Start()
{
	GameEngineRenderer::Start();

	ImageTransform.SetParent(Transform);

	GameEngineRenderer::SetMesh("Rect");
	GameEngineRenderer::SetMaterial("Custom2DTexture");

	
}

// Update Order에 영향을 받는다.
void CustomSpriteRenderer::Update(float _Delta)
{
	if (nullptr != CurFrameAnimations)
	{
		Sprite = CurFrameAnimations->Sprite;
		CurSprite = CurFrameAnimations->Update(_Delta);
	}

	if (true == IsImageSize)
	{
		float4 Scale = float4(CurSprite.GetScale());
		Scale.Z = 1.0f;
		Scale.W = 0.0f;
		SetImageScale(Scale * AutoScaleRatio);
	}

	RenderBaseInfoValue.RenderScreenScale = CurSprite.GetScale();
	// 
}

void CustomSpriteRenderer::SetImageScale(const float4& _Scale)
{
	ImageTransform.SetLocalScale(_Scale);
}

void CustomSpriteRenderer::AddImageScale(const float4& _Scale)
{
	ImageTransform.AddLocalScale(_Scale);
}


void CustomSpriteRenderer::Render(GameEngineCamera* _Camera, float _Delta)
{
	float4 ParentScale = Transform.GetLocalScale();
	float4 Scale = ImageTransform.GetLocalScale();

	float4 CalPivot = Pivot;
	CalPivot.X -= 0.5f;
	CalPivot.Y -= 0.5f;

	float4 PivotPos;
	PivotPos.X = Scale.X * CalPivot.X;
	PivotPos.Y = Scale.Y * CalPivot.Y;

	ImageTransform.SetLocalPosition(PivotPos);

	ImageTransform.TransformUpdate();
	ImageTransform.CalculationViewAndProjection(Transform.GetConstTransformDataRef());

	GetShaderResHelper().SetTexture("DiffuseTex", CurSprite.Texture);


	GameEngineRenderer::Render(_Camera, _Delta);



}

void CustomSpriteRenderer::SetSprite(std::string_view _Name, unsigned int index /*= 0*/)
{
	CurFrameAnimations = nullptr;

	Sprite = GameEngineSprite::Find(_Name);

	if (nullptr == Sprite)
	{
		MsgBoxAssert("존재하지 않는 스프라이트를 사용하려고 했습니다.");
	}

	CurSprite = Sprite->GetSpriteData(index);
	SetImageScale(CurSprite.GetScale() * AutoScaleRatio);
}

void CustomSpriteRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _SpriteName,
	float _Inter /*= 0.1f*/,
	unsigned int _Start /*= -1*/,
	unsigned int _End /*= -1*/,
	bool _Loop /*= true*/
)
{
	std::string SpriteName = GameEngineString::ToUpperReturn(_SpriteName);

	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find(SpriteName);
	if (nullptr == Sprite)
	{
		MsgBoxAssert("존재하지 않는 스프라이트로 애니메이션을 만들려고 했습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);
	if (true == FrameAnimations.contains(UpperName))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	std::shared_ptr<CustomFrameAnimation> NewAnimation = std::make_shared<CustomFrameAnimation>();
	FrameAnimations[UpperName] = NewAnimation;
	NewAnimation->AnimationName = _AnimationName;
	NewAnimation->SpriteName = _SpriteName;
	NewAnimation->Sprite = Sprite;
	NewAnimation->Loop = _Loop;

	NewAnimation->Parent = this;

	if (_Start != -1)
	{
		NewAnimation->Start = _Start;
	}
	else
	{
		NewAnimation->Start = 0;
	}

	if (_End != -1)
	{
		NewAnimation->End = _End;
	}
	else
	{
		NewAnimation->End = Sprite->GetSpriteCount() - 1;
	}


	for (unsigned int i = NewAnimation->Start; i <= NewAnimation->End; i++)
	{
		NewAnimation->Index.push_back(i);
	}

	NewAnimation->Inter.resize(NewAnimation->Index.size());
	for (size_t i = 0; i < NewAnimation->Index.size(); i++)
	{
		NewAnimation->Inter[i] = _Inter;
	}



	NewAnimation->CurIndex = 0;
}

void CustomSpriteRenderer::ChangeAnimation(std::string_view _AnimationName, bool _Force /*= false*/, unsigned int _FrameIndex /*= 0*/)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<CustomFrameAnimation>>::iterator FindIter
		= FrameAnimations.find(UpperName);

	if (FindIter == FrameAnimations.end())
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지하려고 했습니다.");
		return;
	}

	if (_Force == false && FindIter->second == CurFrameAnimations)
	{
		return;
	}

	CurFrameAnimations = FrameAnimations[UpperName];
	CurFrameAnimations->Reset();
	CurFrameAnimations->CurIndex = _FrameIndex;
	Sprite = CurFrameAnimations->Sprite;
	CurSprite = CurFrameAnimations->Sprite->GetSpriteData(CurFrameAnimations->CurIndex);
}

void CustomSpriteRenderer::AutoSpriteSizeOn()
{
	IsImageSize = true;
}

void CustomSpriteRenderer::AutoSpriteSizeOff()
{
	IsImageSize = false;
}

void CustomSpriteRenderer::SetFrameEvent(std::string_view _AnimationName, int _Frame, std::function<void(CustomSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<CustomFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<CustomFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->FrameEventFunction[_Frame] = _Function;
}

void CustomSpriteRenderer::SetStartEvent(std::string_view _AnimationName, std::function<void(CustomSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<CustomFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<CustomFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->FrameEventFunction[Animation->Index[0]] = _Function;
}

void CustomSpriteRenderer::SetEndEvent(std::string_view _AnimationName, std::function<void(CustomSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<CustomFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<CustomFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->EndEvent = _Function;
}

void CustomSpriteRenderer::AnimationPauseSwitch()
{
	IsPause = !IsPause;
}

void CustomSpriteRenderer::AnimationPauseOn()
{
	IsPause = true;
}

void CustomSpriteRenderer::AnimationPauseOff()
{
	IsPause = false;
}

void CustomSpriteRenderer::SetPivotType(PivotType _Type)
{
	switch (_Type)
	{
	case PivotType::Center:
		Pivot = { 0.5f, 0.5f };
		break;
	case PivotType::Top:
		Pivot = { 0.5f, 0.0f };
		break;
	case PivotType::RightUp:
		Pivot = { 0.0f, 0.0f };
		break;
	case PivotType::Right:
		Pivot = { 0.0f, 0.5f };
		break;
	case PivotType::RightBottom:
		Pivot = { 0.0f, 1.0f };
		break;
	case PivotType::Bottom:
		Pivot = { 0.5f, 1.0f };
		break;
	case PivotType::LeftBottom:
		Pivot = { 1.0f, 1.0f };
		break;
	case PivotType::Left:
		Pivot = { 1.0f, 0.5f };
		break;
	case PivotType::LeftTop:
		Pivot = { 1.0f, 0.0f };
		break;
	default:
		break;
	}
}

void CustomSpriteRenderer::SetMaterialEvent(std::string_view _Name, int _Index)
{
	const TransformData& Data = ImageTransform.GetConstTransformDataRef();
	GetShaderResHelper().SetConstantBufferLink("TransformData", Data);
	GetShaderResHelper().SetConstantBufferLink("SpriteData", CurSprite.SpritePivot);
	GetShaderResHelper().SetConstantBufferLink("SpriteRendererInfo", SpriteRendererInfoValue);
	GetShaderResHelper().SetConstantBufferLink("ColorData", ColorDataValue);
	GetShaderResHelper().SetConstantBufferLink("TexcoordData", TexCoordDataValue);
	GetShaderResHelper().SetConstantBufferLink("PassPortMaskData", PassPortMaskDataValue);
	GetShaderResHelper().SetConstantBufferLink("FaceFadeData", FaceFadeDataValue);




	SetSprite("NSet.png");
}


void CustomSpriteRenderer::SetMaskTexture(std::string_view _Texture, MaskMode _Mask)
{
	//std::shared_ptr<CustomFrameAnimation> TempCurFrameAnimation = CurFrameAnimations;
	//std::shared_ptr<GameEngineSprite> TempSprite = Sprite;
	//SpriteData TempCurSprite = CurSprite;

	//GameEngineRenderer::SetMaterial("2DTextureMask");

	//if (CurFrameAnimations != TempCurFrameAnimation)
	//{
	//	CurFrameAnimations = TempCurFrameAnimation;
	//}

	//if (Sprite != TempSprite)
	//{
	//	Sprite = TempSprite;
	//	CurSprite = TempCurSprite;
	//}

	RenderBaseInfoValue.IsMask = 1;
	RenderBaseInfoValue.MaskMode = static_cast<int>(_Mask);
	GetShaderResHelper().SetTexture("MaskTex", _Texture);

	std::shared_ptr<GameEngineTexture> Ptr = GameEngineTexture::Find(_Texture);
	RenderBaseInfoValue.MaskScreeneScale = Ptr->GetScale();
}

void CustomSpriteRenderer::SetPassPortTexture(std::string_view _Texture,const float4& _TexturePos)
{
	
	RenderBaseInfoValue.IsMask = 1;
	RenderBaseInfoValue.MaskMode = static_cast<int>(MaskMode::DynamicMask);
	PassPortMaskDataValue.IsStamp = 1;
	RenderBaseInfoValue.MaskPivot = { _TexturePos };

	GetShaderResHelper().SetTexture("PassPortTex", _Texture);
	std::shared_ptr<GameEngineTexture> Ptr = GameEngineTexture::Find(_Texture);
	
	PassPortMaskDataValue.PassportPos = _TexturePos;
	PassPortMaskDataValue.PassportScale = Ptr->GetScale();
}



void CustomSpriteRenderer::SetIntroSprite()
{
	TexCoordDataValue.IsUseTexCoord = 1;
	TexCoordDataValue.CurTexCoord = 1.0f;
	GetShaderResHelper().SetConstantBufferLink("TexcoordData", TexCoordDataValue);
}


void CustomSpriteRenderer::SetPaletteTexture(std::string_view _Texture)
{
	// 이녀석한테 있는
	GameEngineRenderer::SetMaterial("2DTextureMask");

	GetShaderResHelper().SetTexture("FacePalette", _Texture);
}

void CustomSpriteRenderer::SetFaceFadeTexture(int _FadeNum)
{
	FaceFadeDataValue.IsFaceFade = 1;
	FaceFadeDataValue.FadeNum = _FadeNum;
	GetShaderResHelper().SetConstantBufferLink("FaceFadeData", FaceFadeDataValue);

}