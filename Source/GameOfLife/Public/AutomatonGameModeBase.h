/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AutomatonGameModeBase.generated.h"


/**
 * Custom GameModeBase
 */
UCLASS()
class GAMEOFLIFE_API AAutomatonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief Changes the default GameModeBase classes
	 */
	AAutomatonGameModeBase();
};
