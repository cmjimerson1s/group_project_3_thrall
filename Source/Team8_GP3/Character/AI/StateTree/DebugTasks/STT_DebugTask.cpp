// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_DebugTask.h"

EStateTreeRunStatus USTT_DebugTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	if (AllowPrintTextToScreen)
		PrintTextToScreen();

	if (AllowPrintTextToLog)
		PrintTextToLog();

	if (ShouldFinish)
		FinishTask(FinishSuccess);
	
	return RunStatus;
}

void USTT_DebugTask::PrintTextToScreen()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TextToPrint);
}

void USTT_DebugTask::PrintTextToLog()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TextToPrint);
}
