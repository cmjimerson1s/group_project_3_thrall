#pragma once

#define print(Message) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("%s"), *FString(Message)))

#define InvokeFunction(DelayTime, FunctionToCall) GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { FTimerHandle UnusedHandle; GetWorld()->GetTimerManager().SetTimer( UnusedHandle, [this]() { FunctionToCall; }, DelayTime, false ); });
