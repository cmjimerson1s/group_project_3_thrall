#include "SCR_RoomManagerSubsystem.h"
#include "SCR_RoomManager.h"

void USCR_RoomManagerSubsystem::RegisterRoomManager(ASCR_RoomManager* RoomManager)
{
	if (RoomManager)
	{
		RoomManagers.Add(RoomManager);
	}
}

ASCR_RoomManager* USCR_RoomManagerSubsystem::GetRoomManager(int n)
{
	if (n < RoomManagers.Num())
		return RoomManagers[n];

	return nullptr;
}
