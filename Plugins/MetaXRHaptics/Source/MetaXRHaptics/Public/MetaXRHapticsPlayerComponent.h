/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * Licensed under the Oculus SDK License Agreement (the "License");
 * you may not use the Oculus SDK except in compliance with the License,
 * which is provided at the time of installation or download, or which
 * otherwise accompanies this software in either electronic or hard copy form.
 *
 * You may obtain a copy of the License at
 *
 * https://developer.oculus.com/licenses/oculussdk/
 *
 * Unless required by applicable law or agreed to in writing, the Oculus SDK
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "haptics_sdk/haptics_sdk.h"
#include "MetaXRHapticsPlayerComponent.generated.h"

class UMetaXRHapticClip;
class FMetaXRHapticsModule;

UENUM(BlueprintType)
enum class EMetaXRHapticController : uint8
{
	Left UMETA(DisplayName = "Left controller"),
	Right UMETA(DisplayName = "Right controller"),
	Both UMETA(DisplayName = "Both controllers"),
};

/**
 * Component for playing back haptic clips.
 *
 * After setting a haptic clip with the HapticClip property, playback can be started with
 * Play() or PlayWithInputs().
 *
 * The properties Amplitude, FrequencyScale, Priority and bIsLooping control playback.
 */
UCLASS(ClassGroup = (MetaXRHaptics), meta = (BlueprintSpawnableComponent, DisplayName = "MetaXR Haptics Player Component"))
class METAXRHAPTICS_API UMetaXRHapticsPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMetaXRHapticsPlayerComponent();

	/**
	 * Start playback of the haptic clip set with the HapticClip property.
	 */
	UFUNCTION(BlueprintCallable, Category = "MetaXR|Haptics")
	void Play();

	/**
	 * Start playback of the haptic clip set with the HapticClip property on the specified controller.
	 *
	 * Does not set the HapticController property.
	 *
	 * @param InController Which controller should play the haptic clip.
	 */
	UFUNCTION(BlueprintCallable, Category = "MetaXR|Haptics")
	void PlayOnController(const EMetaXRHapticController InController);

	/**
	 * Start playback of the haptic clip with custom inputs.
	 *
	 * This is a convenience method that sets the properties corresponding to the input parameters,
	 * and then starts playback.
	 *
	 * @param InController Which controller should play the haptic clip.
	 * @param InPriority Playback priority, ranging from 0 (low priority) to 1024 (high priority).
	 * @param InAmplitude Amplitude scale, ranging from 0.0 to infinite.
	 * @param InFrequencyShift Frequency shift, ranging from -1.0 to 1.0.
	 * @param bInIsLooping Whether playback should loop
	 */
	UFUNCTION(BlueprintCallable, Category = "MetaXR|Haptics")
	void PlayWithInputs(const EMetaXRHapticController InController, const int32 InPriority = 512,
		const float InAmplitude = 1.0f, const float InFrequencyShift = 0.0f, const bool bInIsLooping = false);

	/**
	 * Stops playback.
	 */
	UFUNCTION(BlueprintCallable, Category = "MetaXR|Haptics")
	void Stop();

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	UMetaXRHapticClip* GetHapticClip() const;

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetHapticClip(UMetaXRHapticClip* InHapticClip);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	EMetaXRHapticController GetController() const;

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetController(const EMetaXRHapticController& InController);

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetPriority(const int32 InPriority = 512);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	int32 GetPriority() const;

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetAmplitude(const float InAmplitude = 1.0f);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	float GetAmplitude() const;

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetFrequencyShift(const float InFrequencyShift = 0.0f);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	float GetFrequencyShift() const;

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	void SetLooping(const bool bInIsLooping = false);

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "MetaXR|Haptics")
	bool GetLooping() const;

	/**
	 * Gets the duration in seconds of the haptic clip.
	 */
	UFUNCTION(BlueprintPure, Category = "MetaXR|Haptics")
	float GetClipDuration() const;

	/**
	 * Internal method used by UMetaXRHapticsFunctionLibrary, do not call.
	 *
	 * Used to set initial values when spawning a new component from code. Needs to be called before
	 * BeginPlay().
	 */
	void SetInitialValues(UMetaXRHapticClip* InHapticClip, const EMetaXRHapticController InController, const int32 InPriority = 512,
		const float InAmplitude = 1.0f, const float InFrequencyShift = 0.0f, const bool bInIsLooping = false);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type reason) override;

private:
	/**
	 * The haptic clip played back with this player.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetHapticClip, BlueprintSetter = SetHapticClip,
		Category = "MetaXR|Haptics")
	UMetaXRHapticClip* HapticClip = nullptr;

	/**
	 * The controller on which the haptic clip will be played back.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetController, BlueprintSetter = SetController, Category = "MetaXR|Haptics")
	EMetaXRHapticController Controller = EMetaXRHapticController::Both;

	/**
	 * The playback priority of this player.
	 *
	 * When multiple players are playing at the same time on the same controller, only the player
	 * with the highest priority will trigger vibrations.
	 *
	 * If multiple players have the same priority level, then the player that was started most
	 * recently will trigger vibrations.
	 *
	 * The priority can be any integer value between 0 (low priority) and 1024 (high priority). The
	 * default value is 512.
	 *
	 * The player's priority can be changed before and during playback.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetPriority, BlueprintSetter = SetPriority,
		Category = "MetaXR|Haptics", meta = (ClampMin = "0", ClampMax = "1024", UIMin = "0", UIMax = "1024"))
	int32 Priority = 512;

	/**
	 * The amplitude scale, controlling the vibration intensity during playback.
	 *
	 * During playback, the individual amplitudes in the clip will be multiplied by the player's
	 * amplitude. This changes how strong the vibration is. Amplitude values in a clip range from 0.0
	 * to 1.0, and the result after applying the amplitude scale will be clipped to that range.
	 *
	 * An amplitude of 0.0 means that no vibration will be triggered, and an amplitude of 0.5 will
	 * result in the clip being played back at half of its amplitude.
	 *
	 * Example: if you apply amplitude of 5.0 to a haptic clip and the following amplitudes are in the
	 * clip: [0.2, 0.5, 0.1], the initial amplitude calculation would produce these values: [1.0, 2.5, 0.5]
	 * which will then be clamped like this: [1.0, 1.0, 0.5]
	 *
	 * The property can be changed during active playback, in which case the amplitude is applied
	 * immediately, with a small delay in the tens of milliseconds.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetAmplitude, BlueprintSetter = SetAmplitude,
		Category = "MetaXR|Haptics", meta = (ClampMin = "0", UIMin = "0"))
	float Amplitude = 1.0f;

	/**
	 * The frequency shift, controlling the vibration frequency during playback.
	 *
	 * During playback, the individual frequencies in the clip will be shifted up or down. The
	 * acceptable range of values is -1.0 to 1.0 inclusive. Once the frequencies in a clip have been
	 * shifted, they will be clamped to the playable range of frequency values, i.e. zero to one.
	 * Setting this property to 0.0 means that the frequencies will not be changed.
	 *
	 * Example: if you apply a frequency shift of 0.8 to a haptic clip and the following frequencies
	 * are in the clip: [0.1, 0.5, 0.0], the initial frequency shift calculation will produce these
	 * frequencies: [0.9, 1.3, 0.8] which will then be clamped like this: [0.9, 1.0, 0.8]
	 *
	 * The property can be changed during active playback, in which case the frequency shift is
	 * applied immediately, with a small delay in the tens of milliseconds.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetFrequencyShift, BlueprintSetter = SetFrequencyShift,
		Category = "MetaXR|Haptics", meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
	float FrequencyShift = 0.0f;

	/**
	 * Whether playback is looped.
	 *
	 * When looping is enabled, clip playback will be repeated indefinitely.
	 *
	 * Looping can be enabled or disabled during playback.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter = GetLooping, BlueprintSetter = SetLooping,
		Category = "MetaXR|Haptics")
	bool bIsLooping = false;

	FMetaXRHapticsModule* HapticsModule = nullptr;

	/* The ID of the player. The player is created in BeginPlay() and released in EndPlay(). */
	int32 PlayerID = HAPTICS_SDK_INVALID_ID;

	/* The ID of the clip. The clip is created in SetClip() and released in EndPlay(). */
	int32 ClipID = HAPTICS_SDK_INVALID_ID;

	void LoadClipIntoPlayer();
	void ReleaseClip();
};
