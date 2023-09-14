/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Cell.generated.h"


/**
 * Cellular Automaton Cell
 */
UCLASS()
class GAMEOFLIFE_API ACell : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Visual representation
	 */
	UPROPERTY(EditAnywhere, Category = "Sprite Component")
		UPaperSpriteComponent* PaperSpriteComponent;

	/**
	 * @brief Changes the color of UPaperSpriteComponent
	 * @param NewColor Color to be set on the PaperSpriteComponent
	 */
	void ChangeColor(FLinearColor NewColor) const;
	
	/**
	 * @brief Initializes the UPaperSpriteComponent
	 */
	ACell();
};
