/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)


#include "Cell.h"
#include "PaperSprite.h"


ACell::ACell()
{
	/// Creates UPaperSpriteComponent and assigns it to the RootComponent
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("PaperSpriteComponent");
	RootComponent = PaperSpriteComponent;

	/// Loads the sprite asset as PaperSprite and assigns it to the PaperSpriteComponent
    const ConstructorHelpers::FObjectFinder<UPaperSprite> Paper_Sprite
	(TEXT("PaperSprite'/Game/Sprites/Cell_Sprite.Cell_Sprite'"));
	PaperSpriteComponent->SetSprite(Paper_Sprite.Object);
}

void ACell::ChangeColor(const FLinearColor NewColor) const
{
	PaperSpriteComponent->SetSpriteColor(NewColor);
}

