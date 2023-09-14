/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CellularAutomatonHUD.generated.h"

/**
 * Shows the values of the Cellular Automaton in the scene
 */
UCLASS()
class GAMEOFLIFE_API ACellularAutomatonHUD : public AHUD
{
 GENERATED_BODY()
    
private:
    /**
     * @brief STextBlock to be displayed on the screen
     */
    TSharedPtr<STextBlock> EvolutionsTextBlock;

    /**
     * @brief STextBlock to be displayed on the screen
     */
    TSharedPtr<STextBlock> PopulationTextBlock;
 
protected:
    /**
     * @brief Binds STextBlocks with the CellularAutomaton attributes (Evolution, Population).\n
     * Called when the game starts or when it is spawned
     */
    virtual void BeginPlay() override;
};
