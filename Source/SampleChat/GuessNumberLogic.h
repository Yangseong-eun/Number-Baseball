#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GuessNumberLogic.generated.h"

UCLASS()
class SAMPLECHAT_API UGuessNumberLogic : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // 🔹 랜덤한 3자리 숫자 생성 (1~9, 중복 없음)
    UFUNCTION(BlueprintCallable, Category = "GuessNumber")
    static FString GenerateRandomNumber();

    // 🔹 입력된 숫자가 유효한지 검사 (중복 없음, 1~9 숫자로만 구성)
    UFUNCTION(BlueprintCallable, Category = "GuessNumber")
    static bool IsValidGuess(const FString& Guess);

    // 🔹 정답과 플레이어 입력을 비교하여 S/B 계산
    UFUNCTION(BlueprintCallable, Category = "GuessNumber")
    static void EvaluateGuess(
        const FString& SecretNumber,
        FString PlayerGuess, 
        int32& OutStrikes,
        int32& OutBalls);

    // 게임 상태 체크 (승리, 패배, 무승부 판정)
    UFUNCTION(BlueprintCallable, Category = "GuessNumber")
    static FString CheckGameResult(
        int32& HostTries,  
        int32& GuestTries, 
        int32 HostStrikes,
        int32 GuestStrikes,
        FString& OutNewSecretNumber,
        bool& bGameReset
    );

    // 🔹 게임 리셋 (새로운 숫자 생성, 시도 횟수 초기화)
    UFUNCTION(BlueprintCallable, Category = "GuessNumber")
    static void ResetGame(FString& OutNewSecretNumber, int32& OutHostTries, int32& OutGuestTries);
};
