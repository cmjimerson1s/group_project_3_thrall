// Fill out your copyright notice in the Description page of Project Settings.


#include "STT_ToggleFocusTarget.h"

#include "Camera/CameraComponent.h"
#include "Team8_GP3/Character/AI/SCR_AIBrainSubsystem.h"

EStateTreeRunStatus USTT_ToggleFocusTarget::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (!ShouldFocus)
	{
		Controller->ClearFocus(EAIFocusPriority::Gameplay);
		return RunStatus;
	}

	auto target = GetWorld()->GetGameInstance()->GetSubsystem<USCR_AIBrainSubsystem>()->GetAITarget();
	if (!target) return EStateTreeRunStatus::Failed;

	auto camera = target->GetComponentByClass<UCameraComponent>();
	if (!camera) return EStateTreeRunStatus::Failed;

	Controller->SetFocalPoint(camera->GetComponentLocation());
	
	return RunStatus;
}
