/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)

#pragma once

#include "CoreMinimal.h"
#include "Cell.h"
#include "GameFramework/Actor.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "CellularAutomaton.generated.h"


/**
 * Board that manages the states of the cells
 */
UCLASS()
class GAMEOFLIFE_API ACellularAutomaton : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Initializes all the members to nullptr
	 */
	ACellularAutomaton();
	
	/**
	 * @brief View of our controller
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		ACameraActor* CameraActor;
	
	/**
	 * @brief Automaton board width
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		int Width = 50;

	/**
	 * @brief Automaton board height
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		int Height = 50;

	/**
	 * @brief Time between iterations
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		float InRate = 0.1f;
	
	/**
	 * @brief Color of a dead cell
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		FLinearColor DeadCell = FLinearColor::White;

	/**
	 * @brief Color of a live cell
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
		FLinearColor LiveCell = FLinearColor::Black;


	/**
	 * @brief Custom MappingContext
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
		TSoftObjectPtr<UInputMappingContext> AutomataContext;

	/**
	 * @brief InputAction that generates and pauses evolutions
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
		UInputAction* Generate;

	/**
	 * @brief InputAction that sets to 0 all the values (Evolutions, Population, CellList)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
		UInputAction* Clear;

	/**
	 * @brief Creates new live or dead cells
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
		UInputAction* Click;
	
private:
	/**
	 * @brief Number of generations since the initial population
	 */
	int Evolutions = 0;

	/**
	 * @brief Number of cells of the current generation
	 */
	int Population = 0;

	/**
	 * @brief Represents the states of the cells.\n
	 * Automaton only stores 1 or 0, which represents the state of the cell:\n
	 * 1: Live cell, 0: Dead cell
	 */
	TArray<int> Automaton;

	/**
	 * @brief It represents the next generation of the automaton.\n
	 * Prevent the program from running when there are no more generations to create
	 */
	TArray<int> NextAutomaton;

	/**
	 * @brief Represents the last Automaton before click.\n
	 * Prevents the creation and destruction of a cell multiple times
	 */
	TArray<int> LastAutomaton;

	/**
	 * @brief List of cells that contains all the cells on the board
	 */
	UPROPERTY()
	TArray<ACell*> CellList;

	/**
	 * @brief Reference to PlayerController
	 */  
	UPROPERTY()
	APlayerController* Controller;

public:
	/**
	 * @brief Get the updated text of the Population 
	 */  
	FText GetPopulation() const;

	/**
	 * @brief Get the updated text of the Evolutions 
	 */  
	FText GetEvolutions() const;

protected:
	/**
	 * @brief Changes controller values, configures inputs and initializes arrays.\n
	 * Called when the game starts or when it is spawned
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Timer that controls the generation frequency
	 */
	FTimerHandle MemberTimerHandle;

	/**
	 * @brief Reset all values in an array to zero.
	 * @param Array Array to be emptied
	 */
	static void ResetArray(TArray<int>& Array);

	/**
	 * @brief Sets the values of the SecondAutomaton to FirstAutomaton
	 * @param FirstAutomaton Where values are going to be changed
	 * @param SecondAutomaton Values to be copied
	 */
	static void SetAutomaton(TArray<int>& FirstAutomaton, TArray<int>& SecondAutomaton);

	/**
	 * @brief Reset values
	 */
	void ResetAutomaton();

	/**
	 * @brief Creates new generations and stops creating new ones if there is no change
	 */
	void CreateGenerations();

	/**
	 * @brief Changes the color of the cell depending on its value in Automaton (live/empty).\n
	 * It also updates the population value
	 */
	void SetAutomatonState();

	/**
	 * @brief Rules to decide if a cell lives or not
	 */
	void AutomatonRules();

	/**
	 * @brief Gets the number of live neighboring cells from the position of a cell
	 * @param Cell_X X-coordinate of the cell
	 * @param Cell_Y Y-coordinate of the cell
	 * @return Number of live neighboring cells around the position (Cell_X, Cell_Y)
	 */
	int GetNeighbour(int Cell_X, int Cell_Y) const;

	/**
	 * @brief Creates cells according to the width and height and stores them in the CellList
	 */
	void FillAutomaton();

	/**
	 * @brief Pauses and resumes MemberTimerHandle if it has been previously initiated
	 */
	void PauseAutomaton();

	/**
	 * @brief Checks HitCollision and Changes Cell Color
	 */
	void CreateDestroyLiveCell();

	/**
	 * @brief Pause cell evolutions
	 */
	void PauseAutomatonOnClick();

	/**
	 * @brief Sets Automaton values to LastAutomaton
	 */
	void PreventMultipleCreations();
};
