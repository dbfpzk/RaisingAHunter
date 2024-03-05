// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter/Controller/UnitController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Enemy/Enemy.h"
#include "Hunter/Unit.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/Inventory/InventoryWidget.h"
#include "Hunter/PlayerState/UnitState.h"

AUnitController::AUnitController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprint/Input/IMC_Default.IMC_Default'"));
	if (IMCAsset.Succeeded())
	{
		DefaultMappingContext = IMCAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Jump.IA_Jump'"));
	if (JumpActionAsset.Succeeded())
	{
		JumpAction = JumpActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Move.IA_Move'"));
	if (MoveActionAsset.Succeeded())
	{
		MoveAction = MoveActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Look.IA_Look'"));
	if (LookActionAsset.Succeeded())
	{
		LookAction = LookActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Zoom.IA_Zoom'"));
	if (ZoomActionAsset.Succeeded())
	{
		ZoomAction = ZoomActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> DashActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Dash.IA_Dash'"));
	if (DashActionAsset.Succeeded())
	{
		DashAction = DashActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ClimbActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Climb.IA_Climb'"));
	if (ClimbActionAsset.Succeeded())
	{
		ClimbAction = ClimbActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Attack.IA_Attack'"));
	if (AttackActionAsset.Succeeded())
	{
		AttackAction = AttackActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TargetingActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Targeting.IA_Targeting'"));
	if (TargetingActionAsset.Succeeded())
	{
		TargetingAction = TargetingActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TargetLeftActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_TargetLeft.IA_TargetLeft'"));
	if (TargetLeftActionAsset.Succeeded())
	{
		TargetLeftAction = TargetLeftActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TargetRightActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_TargetRight.IA_TargetRight'"));
	if (TargetRightActionAsset.Succeeded())
	{
		TargetRightAction = TargetRightActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> UpperSlashActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_UpperSlash.IA_UpperSlash'"));
	if (TargetRightActionAsset.Succeeded())
	{
		UpperSlashAction = UpperSlashActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Inventory.IA_Inventory'"));
	if (InventoryActionAsset.Succeeded())
	{
		InventoryAction = InventoryActionAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InvincibilityAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Invincibility.IA_Invincibility'"));
	if (InvincibilityAsset.Succeeded())
	{
		InvincibilityAction = InvincibilityAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> BladeAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprint/Input/IA_Blade.IA_Blade'"));
	if (BladeAsset.Succeeded())
	{
		BladeAction = BladeAsset.Object;
	}
	// * 인벤토리
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/Widget/Inventory/WB_Inventory.WB_Inventory_C'"));
	if (InventoryAsset.Succeeded())
	{
		InventoryWidgetClass = InventoryAsset.Class;
	}
	
}

void AUnitController::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryWidgetClass != nullptr)
	{
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass));
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	UnitState = GetPlayerState<AUnitState>();
}

void AUnitController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsValid(InPawn)) return;
	Unit = Cast<AUnit>(InPawn);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AUnitController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUnitController::Jump);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnitController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUnitController::EndMove);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnitController::Look);
		//Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AUnitController::Zoom);
		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AUnitController::Dash);
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &AUnitController::Climb);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AUnitController::Attack);
		EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Started, this, &AUnitController::Targeting);
		EnhancedInputComponent->BindAction(TargetLeftAction, ETriggerEvent::Started, this, &AUnitController::TargetLeft);
		EnhancedInputComponent->BindAction(TargetRightAction, ETriggerEvent::Started, this, &AUnitController::TargetRight);
		//UpperSlash
		EnhancedInputComponent->BindAction(UpperSlashAction, ETriggerEvent::Started, this, &AUnitController::UpperSlash);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AUnitController::Inventory);
		EnhancedInputComponent->BindAction(InvincibilityAction, ETriggerEvent::Started, this, &AUnitController::Invincibility);
		EnhancedInputComponent->BindAction(BladeAction, ETriggerEvent::Started, this, &AUnitController::Blade);
	}
}

void AUnitController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	if (IsValid(LockOnEnemy))
	{
		LookAtTarget();
	}
}

void AUnitController::GetTraceTarget()
{
	FVector Location = Unit->GetActorLocation();
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreActors;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	IgnoreActors.Add(Unit);
	bool Success = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location, LockOnDistance, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true);
	if (!Success) return;
	for (const FHitResult& item : HitResults)
	{
		if (AEnemy* Enemy = Cast<AEnemy>(item.GetActor()))
		{
			TargetActors.AddUnique(Enemy); //에너미에 추가
		}
	}
}

void AUnitController::GetTargetEnemy()
{
	AEnemy* FrontTarget = nullptr;
	float Angle = -2.0f;

	for (AEnemy* Enemy : TargetActors)
	{
		float Dot = Unit->GetDotProductTo(Enemy);
		if (Angle <= Dot)
		{
			Angle = Dot;
			FrontTarget = Enemy;
		}
	}
	if (IsValid(FrontTarget))
	{
		LockOnEnemy = FrontTarget;
		LockOnEnemy->TargetEffectComp->SetVisibility(true);
	}
	else
	{
		LockOnEnemy = nullptr;
	}
}

void AUnitController::StartLockOn()
{
	GetTraceTarget();
	GetTargetEnemy();
}

void AUnitController::EndLockOn()
{
	if (IsValid(LockOnEnemy))
	{
		LockOnEnemy->TargetEffectComp->SetVisibility(false);
		LockOnEnemy = nullptr;
		TargetActors.Empty();
	}
}

void AUnitController::LookAtTarget()
{
	if (!IsValid(LockOnEnemy)) return;
	if (Unit->GetDistanceTo(LockOnEnemy) <= LockOnDistance)
	{
		FVector UnitPos = Unit->GetActorLocation();
		FVector EnemyPos = LockOnEnemy->GetActorLocation();
		SetControlRotation(UKismetMathLibrary::FindLookAtRotation(UnitPos, EnemyPos));
	}
	else
	{
		EndLockOn();
	}
}

float AUnitController::GetProjectionLength(TObjectPtr<AActor> Target)
{
	FVector ForwardVector = Unit->GetActorForwardVector();
	FVector TargetPos = Target->GetActorLocation();
	FVector UnitPos = Unit->GetActorLocation();
	FVector Cross = FVector::CrossProduct(ForwardVector, TargetPos - UnitPos);
	return FVector::DotProduct(Cross, Unit->GetActorUpVector());
}

void AUnitController::GetOtherTarget(bool isRight)
{
	if (!IsValid(LockOnEnemy)) return;
	float Distance;
	float Minimum = 1000000.f;
	AEnemy* NextTarget = nullptr;
	TMap<float, AEnemy*> DistanceToEnemy;

	for (AEnemy* Enemy : TargetActors)
	{
		if (Enemy != LockOnEnemy)
		{
			if (IsValid(Enemy))
			{
				DistanceToEnemy.Add(GetProjectionLength(Enemy), Enemy);
			}
		}
	}
	TArray<float> DistanceArr;
	DistanceToEnemy.GetKeys(DistanceArr);
	for (float DistanceIndex : DistanceArr)
	{
		Distance = DistanceIndex;
		if (isRight)
		{
			if (Distance >= 0.f)
			{
				if (FMath::Abs(Distance) <= Minimum)
				{
					Minimum = FMath::Abs(Distance);
					NextTarget = *DistanceToEnemy.Find(Distance);
				}
			}
		}
		else
		{
			if (Distance <= 0.f)
			{
				if (FMath::Abs(Distance) <= Minimum)
				{
					Minimum = FMath::Abs(Distance);
					NextTarget = *DistanceToEnemy.Find(Distance);
				}
			}
		}
	}

	for (AEnemy* Enemy : TargetActors)
	{
		if (IsValid(Enemy))
		{
			Enemy->TargetEffectComp->SetVisibility(false);
		}
	}
	if (IsValid(NextTarget))
	{
		LockOnEnemy->TargetEffectComp->SetVisibility(true);
	}
	else
	{
		LockOnEnemy = nullptr;
	}
}

void AUnitController::AddItem(FItemData InItemData)
{
	if (IsValid(UnitState) == false) return;
	int index = FindStack(InItemData);
	InventoryWidget->AddSlotToItem(InItemData);
	if (index == -1) //같은 아이템이 없다면
	{
		UnitState->GetSlotDatas().Add(InItemData);
	}
	else //같은 아이템이 있다면
	{
		UnitState->GetSlotDatas()[index].Amount += InItemData.Amount;
	}
}

int AUnitController::FindStack(FItemData& InItemData)
{
	for (int i = 0; i < UnitState->GetSlotDatas().Num(); i++)
	{
		if (UnitState->GetSlotDatas()[i] == InItemData)
		{
			return i;
		}
	}
	return -1;
}

void AUnitController::Move(const FInputActionValue& Value)
{
	if (!IsValid(Unit)) return;
	if (Unit->GetUnitStateType() != EUnitStateType::Idle) return;
	if (Unit->isClimb) return;
	if (!Unit->GetIsMove()) return;
	Movement = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	switch (Unit->GetCharacterMovement()->MovementMode)
	{
	case EMovementMode::MOVE_Walking:
	case EMovementMode::MOVE_Falling:
		Unit->AddMovementInput(ForwardDirection, Movement.Y);
		Unit->AddMovementInput(RightDirection, Movement.X);
		break;
	case EMovementMode::MOVE_Flying:
		if (Movement.Size() > 1.f)
		{
			Movement.Normalize();
		}
		Unit->ClimbMovement(Unit->GetActorRightVector(), Movement.X);
		Unit->ClimbMovement(Unit->GetActorUpVector(), Movement.Y);
		Unit->LeftAndRight = Movement.X;
		Unit->UpAndDown = Movement.Y;
		break;
	default:
		break;
	}
}

void AUnitController::EndMove()
{
	Unit->LeftAndRight = 0.f;
	Unit->UpAndDown = 0.f;
}

void AUnitController::Look(const FInputActionValue& Value)
{
	if (!IsValid(Unit)) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AUnitController::Zoom(const FInputActionValue& Value)
{
	if (!IsValid(Unit)) return;
	Unit->Zoom(Value.Get<float>());
}

void AUnitController::Dash()
{
	if (!IsValid(Unit)) return;
	if (Unit->GetUnitStateType() != EUnitStateType::Idle) return;
	Unit->SetUnitStateType(EUnitStateType::Dash);
	Unit->Dash(Movement);
}

void AUnitController::Jump()
{
	if (!IsValid(Unit)) return;
	if (Unit->GetUnitStateType() != EUnitStateType::Idle) return;
	Unit->SetUnitStateType(EUnitStateType::Jump);
	Unit->Jump();
}

void AUnitController::Climb()
{
	if (Unit->GetVelocity().Y != 0.f) return;
	if (Unit->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying)
	{
		FHitResult HitResult = Unit->AttachToSurfaceCalculation();
		if (HitResult.bBlockingHit)
		{
			Unit->bUseControllerRotationYaw = false;
			Unit->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			FVector Location = HitResult.Location + (HitResult.Normal * Unit->GetCapsuleComponent()->GetScaledCapsuleRadius());
			//MakeRotFromX : X축만을 기준으로 회전행렬을 만든다.
			FRotator Rotator = UKismetMathLibrary::MakeRotFromX(HitResult.Normal * -1.f);
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Unit->GetCapsuleComponent(), Location, Rotator, false, false, 0.2f, false, EMoveComponentAction::Type::Move, Info);
		}
	}
	else
	{
		Unit->StopClimbing();
	}
}

void AUnitController::Attack()
{
	if (!IsValid(Unit)) return;
	//if (Unit->GetUnitStateType() != EUnitStateType::Idle) return;
	Unit->Attack();
}

void AUnitController::Targeting()
{
	IsValid(LockOnEnemy) ? EndLockOn() : StartLockOn();
}

void AUnitController::TargetLeft()
{
	GetOtherTarget(false);
}

void AUnitController::TargetRight()
{
	GetOtherTarget(true);
}

void AUnitController::UpperSlash()
{
	Unit->UpperSlash();
}

void AUnitController::Inventory()
{
	if (!IsValid(InventoryWidget)) return;
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		PlayerController->SetInputMode(FInputModeGameAndUI(InputMode));
	}
	else
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(false); //커서를 숨겨줌
		PlayerController->SetInputMode(FInputModeGameOnly()); //게임에게만 입력을 전달
		//InventoryWidget->RemoveFromParent(); //위젯 제거
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden); //위젯 숨겨줌
	}
}

void AUnitController::Invincibility()
{
	if (IsValid(Unit))
	{
		Unit->Invincibility();
	}
}

void AUnitController::Blade()
{
	if (IsValid(Unit))
	{
		Unit->Blade();
	}
}
