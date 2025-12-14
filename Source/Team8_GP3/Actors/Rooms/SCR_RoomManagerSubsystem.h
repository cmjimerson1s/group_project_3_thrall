#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SCR_RoomManagerSubsystem.generated.h"

class ASCR_RoomManager;

UCLASS()
class TEAM8_GP3_API USCR_RoomManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterRoomManager(ASCR_RoomManager* RoomManager);

	ASCR_RoomManager* GetRoomManager(int n = 0);
 
private:
	UPROPERTY()
	TArray<ASCR_RoomManager*> RoomManagers;
};
