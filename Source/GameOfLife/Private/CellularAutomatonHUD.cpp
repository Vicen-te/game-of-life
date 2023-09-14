/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)


#include "CellularAutomatonHUD.h"
#include "CellularAutomaton.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Layout/SConstraintCanvas.h"


void ACellularAutomatonHUD::BeginPlay()
{
    Super::BeginPlay();

    /// SVerticalBox containing each STextBlock (Evolutions, Population)
    const TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox); 

    // SConstraintCanvas Background Style
    FSlateBrush* InstancesSlateBrush = new FSlateBrush();
    InstancesSlateBrush->DrawAs = ESlateBrushDrawType::Image;
    InstancesSlateBrush->ImageSize = FVector2d(100,100);
    InstancesSlateBrush->TintColor = FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
    InstancesSlateBrush->Tiling = ESlateBrushTileType::NoTile;
    
    const TSharedPtr<SConstraintCanvas> CustomCanvas =
        SNew(SConstraintCanvas)
        +SConstraintCanvas::Slot()
        .Anchors(FAnchors(1,0))
        .Alignment(FVector2D(1, 0))
        .AutoSize(true)
        [
            SNew(SBorder)
            .BorderImage(InstancesSlateBrush)
            .Padding(FMargin(5, 5, 5, 5))
            .Content()
            [
                VerticalBox
            ]
        ];
        
    /// Add the CustomCanvas to the viewport
    GEngine->GameViewport->AddViewportWidgetContent(CustomCanvas.ToSharedRef());

    /// Gets the first Cellular Automaton from the array
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACellularAutomaton::StaticClass(), FoundActors);
    if(FoundActors.Num() == 0)
    {
        UE_LOG(LogTemp, Fatal, TEXT("There is no ACellularAutomaton in the scene"));
        return;
    }

    /// The CellularAutomaton used in the scene
    ACellularAutomaton* CellularAutomaton = Cast<ACellularAutomaton>(FoundActors[0]);
    
    /// Font path: UE_5.3/Engine/Content/Slate/Fonts.
    /// The STextBlocks will be updated with the attributes of the Cellular Automaton (Evolutions, Population)
    EvolutionsTextBlock = SNew(STextBlock)
        .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"),40))
        .ColorAndOpacity(FSlateColor(FLinearColor::White))
        .Text_UObject(CellularAutomaton, &ACellularAutomaton::GetEvolutions);

    PopulationTextBlock = SNew(STextBlock)
        .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"),40))
        .ColorAndOpacity(FSlateColor(FLinearColor::White))
        .Text_UObject(CellularAutomaton, &ACellularAutomaton::GetPopulation);

    
    VerticalBox->AddSlot()
    .Padding(50.f, 50.f, 50.f, 0.f)
    .FillHeight(0.5)
    [
        EvolutionsTextBlock.ToSharedRef()
    ];

    VerticalBox->AddSlot()
    .Padding(50.f, 25.f, 50.f, 50.f)
    .FillHeight(0.5)
    [
        PopulationTextBlock.ToSharedRef()
    ];
}
