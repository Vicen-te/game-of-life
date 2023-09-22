/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)


#include "CellularAutomaton.h"
#include "Camera/CameraActor.h"
#include "EnhancedInputSubsystems.h"


ACellularAutomaton::ACellularAutomaton(): CameraActor(nullptr), Generate(nullptr), Clear(nullptr), Click(nullptr),
                                          Controller(nullptr) {}

void ACellularAutomaton::BeginPlay()
{
	Super::BeginPlay();

	/// Get player Controller 
	Controller = GEngine->GetFirstLocalPlayerController(GetWorld());

	/// Check if the controller is valid 
	if (!Controller->IsValidLowLevel()) return;

	/// Show mouse cursor
	Controller->bShowMouseCursor = true; 

	/// Change Controller Camera
	Controller->SetCinematicMode(true, true, true, true, true);
	Controller->SetViewTarget(CameraActor);

	/// Allows you to handle controller input
	EnableInput(Controller);
	
	/// Change InputComponent to EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	/// Add custom mapping context
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AutomataContext.LoadSynchronous(), 0);
	}

	/// Bind custom InputActions to our methods
	EnhancedInputComponent->BindAction(Generate, ETriggerEvent::Started, this, &ACellularAutomaton::PauseAutomaton);
	EnhancedInputComponent->BindAction(Click, ETriggerEvent::Started, this, &ACellularAutomaton::PauseAutomatonOnClick);
	EnhancedInputComponent->BindAction(Click, ETriggerEvent::Ongoing, this, &ACellularAutomaton::CreateDestroyLiveCell);
	EnhancedInputComponent->BindAction(Click, ETriggerEvent::Completed, this, &ACellularAutomaton::PreventMultipleCreations);
	EnhancedInputComponent->BindAction(Clear, ETriggerEvent::Started, this, &ACellularAutomaton::ResetAutomaton);
	
	/// Set initial size of arrays with number of cells
	const int NumCells = Width * Height;
	Automaton.AddDefaulted(NumCells);
	NextAutomaton.AddDefaulted(NumCells);
	LastAutomaton.AddDefaulted(NumCells);
	CellList.AddDefaulted(NumCells);
	
	FillAutomaton();
	SetAutomatonState();
}

FText ACellularAutomaton::GetPopulation() const
{
	return FText::FromString("Population: " + FString::FromInt(Population));
}

FText ACellularAutomaton::GetEvolutions() const
{
	return FText::FromString("Evolutions: " + FString::FromInt(Evolutions));
}

void ACellularAutomaton::PauseAutomaton()
{
	/// If the timer has already been created, pause or unpause it
	if(MemberTimerHandle.IsValid())
	{
		if(GetWorldTimerManager().IsTimerPaused(MemberTimerHandle))
			GetWorldTimerManager().UnPauseTimer(MemberTimerHandle);
		else
			GetWorldTimerManager().PauseTimer(MemberTimerHandle);
	}
	/// if not, create a new one that will create a generation every time range (InRate)
	else
	{
		GetWorldTimerManager().SetTimer
		(MemberTimerHandle, this, &ACellularAutomaton::CreateGenerations, InRate, true, InRate);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
/// It can't be const if you use it in a BindAction
void ACellularAutomaton::PauseAutomatonOnClick()
{
	/// Stop creating new generations
	if(MemberTimerHandle.IsValid())
	{
		if(!GetWorldTimerManager().IsTimerPaused(MemberTimerHandle))
		{
			GetWorldTimerManager().PauseTimer(MemberTimerHandle);
		}
	}
}

void ACellularAutomaton::PreventMultipleCreations()
{
	/// Set LastAutomaton so that you can only change the cell once when you click on it
	SetAutomaton(LastAutomaton, Automaton);
}

void ACellularAutomaton::CreateDestroyLiveCell()
{
	FHitResult HitRes = FHitResult();
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	
	if(Controller->GetHitResultUnderCursorForObjects(ObjectType, false, HitRes))
	{
		/// Check if HitResult is equal to ACellClass 
		if(HitRes.GetActor()->GetClass() == ACell::StaticClass())
		{
			/// Cast the HitResult to ACell
			ACell* Cell = Cast<ACell>(HitRes.GetActor());

			/// Get cell position by finding in the cellList.
			/// The SpriteComponent of CellList and Cell State of Automaton are in the same position of both arrays
			const int Position = CellList.Find(Cell);

			/// Prevents cells from being changed multiple times while clicking
			if(LastAutomaton[Position] != Automaton[Position]) return;
			
			/// Check Automaton value and change its values
			if(Automaton[Position] == 1)
			{
				Cell->ChangeColor(DeadCell);
				Automaton[Position] = 0;
				--Population;
			}
			else
			{
				Cell->ChangeColor(LiveCell);
				Automaton[Position] = 1;
				++Population;
			}

			SetAutomatonState();
		}
	}
}

void ACellularAutomaton::SetAutomaton(TArray<int>& FirstAutomaton, TArray<int>& SecondAutomaton)
{
	for (int i = 0; i < FirstAutomaton.Num(); i++)
	{
		FirstAutomaton[i] = SecondAutomaton[i];
	}
}

void ACellularAutomaton::ResetArray(TArray<int>& Array)
{
	for (int& Cell : Array)
	{
		Cell = 0;
	}
}

void ACellularAutomaton::ResetAutomaton()
{
	Evolutions = 0;
	Population = 0;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
	ResetArray(Automaton);
	SetAutomatonState();
}

void ACellularAutomaton::CreateGenerations()
{
	AutomatonRules();

	/// Stop Generations
	if(Automaton == NextAutomaton)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		return;
	}

	SetAutomaton(Automaton, NextAutomaton);

	SetAutomatonState();
	++Evolutions;
}

void ACellularAutomaton::SetAutomatonState()
{
	Population = 0;
	
	/// The cell state(Automaton) and PaperComponent(CellList) of each cell are in the same position.
	for (int i = 0; i < Automaton.Num(); ++i)
	{
		/// Change the color of the Sprite depending on its value
		if (Automaton[i] == 1)
		{
			++Population;
			CellList[i]->ChangeColor(LiveCell);
		}
		else
			CellList[i]->ChangeColor(DeadCell);
	}
}

void ACellularAutomaton::AutomatonRules()
{
	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			/// Gets the population of neighboring cells.
			const int NeighbourCellTiles = GetNeighbour(x, y);

			/// Create a Live Cell
			if (NeighbourCellTiles == 3)
				NextAutomaton[(y * Width) + x] = 1;

			/// Still alive
			else if(NeighbourCellTiles == 2 && Automaton[(y * Width) + x] == 1)
				NextAutomaton[(y * Width) + x] = 1;

			/// Cell Empty
			else if (NeighbourCellTiles < 2 || NeighbourCellTiles > 3)
				NextAutomaton[(y * Width) + x] = 0;
		}
	}
}

int ACellularAutomaton::GetNeighbour(const int Cell_X, const int Cell_Y) const
{
	int Neighbours = 0;
	
	/// Adds +-1 one cell in horizontal direction
	for (int NeighbourX = Cell_X - 1; NeighbourX <= Cell_X + 1; NeighbourX++)
	{
		/// Adds +-1 one cell in vertically direction
		for (int NeighbourY = Cell_Y - 1; NeighbourY <= Cell_Y + 1; NeighbourY++)
		{
			/// Check that it does not go outside the limits.
			if (NeighbourX >= 0 && NeighbourX < Width && NeighbourY >= 0 && NeighbourY < Height)
			{
				/// Verify that it is not the actual cell
				if (NeighbourX != Cell_X || NeighbourY != Cell_Y)
				{
					/// Add its value if there is someone alive it is 1 otherwise 0.
					Neighbours += Automaton[(NeighbourY * Width) + NeighbourX];
				}
			}
		}
	}
	
	return Neighbours;
}

void ACellularAutomaton::FillAutomaton()
{
	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			/// Cell parameters
			const FVector* Location =
				new FVector(
				(-Width * 100) / 2 + (x * 100) + 50,
				0,
				(-Height * 100) / 2 + (y * 100) + 50
				);
			const FRotator* Rotation = new FRotator(0, 0, 0);
			const FActorSpawnParameters Params;
			const int Position = (y * Width) + x;

			/// Spawn ACellClass and add the created cell to the CellList
			ACell* Cell = GetWorld()->SpawnActor<ACell>(ACell::StaticClass(), *Location, *Rotation, Params);
			CellList[Position] = Cell;
		}
	}
}

