#include "GuessNumberLogic.h"

FString UGuessNumberLogic::GenerateRandomNumber()
{
    FString GeneratedNumber;
    TArray<int32> Numbers = { 1,2,3,4,5,6,7,8,9 };

    // 배열을 무작위로 섞기
    for (int32 i = Numbers.Num() - 1; i > 0; i--)
    {
        int32 SwapIndex = FMath::RandRange(0, i);
        Numbers.Swap(i, SwapIndex);
    }

    // 처음 3개 숫자를 조합하여 문자열로 변환
    for (int32 i = 0; i < 3; i++)
    {
        GeneratedNumber.Append(FString::FromInt(Numbers[i]));
    }

    return GeneratedNumber;
}

bool UGuessNumberLogic::IsValidGuess(const FString& Guess)
{
    if (Guess.Len() != 3) return false;

    TSet<TCHAR> UniqueDigits;
    for (int32 i = 0; i < 3; i++)
    {
        if (!FChar::IsDigit(Guess[i]) || Guess[i] == '0') return false;
        UniqueDigits.Add(Guess[i]);
    }

    return UniqueDigits.Num() == 3;
}

void UGuessNumberLogic::EvaluateGuess(
    const FString& SecretNumber,
    FString PlayerGuess, // 복사본을 사용하여 수정 가능하도록 함
    int32& OutStrikes,
    int32& OutBalls)
{
    OutStrikes = 0;
    OutBalls = 0;

    UE_LOG(LogTemp, Display, TEXT("▶▶ Raw PlayerGuess: [%s] (Length: %d)"), *PlayerGuess, PlayerGuess.Len());

    //  '/'가 있는지 확인하고 제거
    if (PlayerGuess.StartsWith(TEXT("/")))
    {
        PlayerGuess = PlayerGuess.RightChop(1);  // '/' 제거
        UE_LOG(LogTemp, Display, TEXT("▶▶ After Removing '/': [%s] (Length: %d)"), *PlayerGuess, PlayerGuess.Len());
    }

    // 길이 확인 후 오류 메시지 출력
    if (SecretNumber.Len() != 3 || PlayerGuess.Len() != 3)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Invalid input length! SecretNumber Len: %d, PlayerGuess Len: %d"),
            SecretNumber.Len(), PlayerGuess.Len());
        return;
    }

    // 정상적인 비교 실행
    for (int32 i = 0; i < 3; i++)
    {
        if (SecretNumber[i] == PlayerGuess[i])
        {
            OutStrikes++;
        }
        else
        {
            for (int32 j = 0; j < 3; j++)
            {
                if (i != j && SecretNumber[j] == PlayerGuess[i])
                {
                    OutBalls++;
                    break;
                }
            }
        }
    }

    // OUT 로직 추가
    if (OutStrikes == 0 && OutBalls == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("🆘 OUT 발생! SecretNumber: %s, PlayerGuess: %s"), *SecretNumber, *PlayerGuess);
    }

    // 최종 결과 출력
    UE_LOG(LogTemp, Warning, TEXT("🎯 Result: %dS %dB"), OutStrikes, OutBalls);
}

// 게임 리셋 함수 (FString을 올바르게 업데이트하도록 수정)
void UGuessNumberLogic::ResetGame(FString& OutNewSecretNumber, int32& OutHostTries, int32& OutGuestTries)
{
    // 새로운 난수 생성
    OutNewSecretNumber = GenerateRandomNumber();

    // 시도 횟수 초기화
    OutHostTries = 0;
    OutGuestTries = 0;

    UE_LOG(LogTemp, Warning, TEXT("🎮 게임이 리셋되었습니다! 새로운 숫자: %s"), *OutNewSecretNumber);
}

FString UGuessNumberLogic::CheckGameResult(
    int32& HostTries, 
    int32& GuestTries, 
    int32 HostStrikes,
    int32 GuestStrikes,
    FString& OutNewSecretNumber,
    bool& bGameReset)
{
    if (HostStrikes == 3)
    {
        bGameReset = true;
        ResetGame(OutNewSecretNumber, HostTries, GuestTries);
        return TEXT("🎉 Host Won!! 다시 게임이 시작됐다.");
    }
    else if (GuestStrikes == 3)
    {
        bGameReset = true;
        ResetGame(OutNewSecretNumber, HostTries, GuestTries);
        return TEXT("🎉 Guest Won!! 다시 게임이 시작됐다.");
    }
    else if (HostTries >= 3 && GuestTries >= 3)
    {
        bGameReset = true;
        ResetGame(OutNewSecretNumber, HostTries, GuestTries);
        return TEXT("😅 무승부군. 다시 게임을 시작하지");
    }

    bGameReset = false;
    return TEXT(""); 
}
