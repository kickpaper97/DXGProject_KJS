
#pragma once
#include <GameEngineCore/GameEngineActor.h>

class BasicActor : public GameEngineActor
{
public:
	BasicActor();
	~BasicActor();

	BasicActor(const BasicActor& _Other) = delete;
	BasicActor(BasicActor&& _Other) noexcept = delete;
	BasicActor& operator=(const BasicActor& _Other) = delete;
	BasicActor& operator=(const BasicActor&& _Other) noexcept = delete;

	
	std::shared_ptr<class GameEngineSpriteRenderer> GetSpriteRenderer() const
	{
		return SpriteRenderer;
	}

protected:

	void Start() override;

	std::shared_ptr<class GameEngineSpriteRenderer> SpriteRenderer = nullptr;
	

private:

};

