/// Copyright (c) 2023 Vicente Brisa Saez (Github: Vicen-te)


#include "AutomatonGameModeBase.h"
#include "CellularAutomatonHUD.h"


AAutomatonGameModeBase::AAutomatonGameModeBase()
{
	HUDClass = ACellularAutomatonHUD::StaticClass();
}
