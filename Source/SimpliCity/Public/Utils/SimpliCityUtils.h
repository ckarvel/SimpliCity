// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Current Class Name + Function Name where this is called!
#define TRACE_STR_CUR_CLASS_FUNC (FString(__FUNCTION__))
// Current Class where this is called!
#define TRACE_STR_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))
// Current Function Name where this is called!
#define TRACE_STR_CUR_FUNC                                                                                             \
  (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2))
// Current Line Number in the code where this is called!
#define TRACE_STR_CUR_LINE (FString::FromInt(__LINE__))
// Current Class and Line Number where this is called!
#define TRACE_STR_CUR_CLASS_LINE (TRACE_STR_CUR_CLASS + "(" + TRACE_STR_CUR_LINE + ")")
// Current Class Name + Function Name and Line Number where this is called!
#define TRACE_STR_CUR_CLASS_FUNC_LINE (TRACE_STR_CUR_CLASS_FUNC + "(" + TRACE_STR_CUR_LINE + ")")
// Current Function Signature where this is called!
#define TRACE_STR_CUR_FUNCSIG (FString(__FUNCSIG__))
// Screen Message
#define TRACE_SCREENMSG(OutputMessage)                                                                                 \
  (GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, *(TRACE_STR_CUR_CLASS_FUNC_LINE + ": " + OutputMessage)))
#define TRACE_SCREENMSG_PRINTF(FormatString, ...)                                                                      \
  (GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,                                                         \
    *(TRACE_STR_CUR_CLASS_FUNC_LINE + ": " + (FString::Printf(TEXT(FormatString), ##__VA_ARGS__)))))
// UE LOG!
#define TRACE_LOG(LogCategory, OutputMessage)                                                                          \
  UE_LOG(LogCategory, Log, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE, *FString(OutputMessage))
#define TRACE_LOG_PRINTF(LogCat, FormatString, ...)                                                                    \
  UE_LOG(                                                                                                              \
    LogCat, Log, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__))
#define TRACE_WARNING(LogCategory, OutputMessage)                                                                      \
  UE_LOG(LogCategory, Warning, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE, *FString(OutputMessage))
#define TRACE_WARNING_PRINTF(LogCategory, FormatString, ...)                                                           \
  UE_LOG(LogCategory, Warning, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE,                                         \
    *FString::Printf(TEXT(FormatString), ##__VA_ARGS__))
#define TRACE_ERROR(LogCategory, OutputMessage)                                                                        \
  UE_LOG(LogCategory, Error, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE, *FString(OutputMessage))
#define TRACE_ERROR_PRINTF(LogCategory, FormatString, ...)                                                             \
  UE_LOG(LogCategory, Error, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE,                                           \
    *FString::Printf(TEXT(FormatString), ##__VA_ARGS__))
#define TRACE_FATAL(LogCategory, OutputMessage)                                                                        \
  UE_LOG(LogCategory, Fatal, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE, *FString(OutputMessage))
#define TRACE_FATAL_PRINTF(LogCategory, FormatString, ...)                                                             \
  UE_LOG(LogCategory, Fatal, TEXT("%s: %s"), *TRACE_STR_CUR_CLASS_FUNC_LINE,                                           \
    *FString::Printf(TEXT(FormatString), ##__VA_ARGS__))

// #pragma once
// #include "Kismet/BlueprintFunctionLibrary.h"
// #include "TraceHelper.h"
// #include "DateTime.h"
// #include "TraceTester.generated.h"
// UCLASS()
// class YOURGAMES_API UTraceTester : public UBlueprintFunctionLibrary {
//   GENERATED_BODY()
//
// public:
//   UFUNCTION(BlueprintCallable,Category = "Traceing")
//     static void TestTraceHelper() {
//     FDateTime startTime = FDateTime::Now();
//     TRACE_SCREENMSG(TEXT("Current Unix Timestamp: ") + FString::SanitizeFloat((startTime -
//     FDateTime::Now()).GetTotalMicroseconds())); TRACE_SCREENMSG_PRINTF("Current Unix Timestamp: %f",(startTime -
//     FDateTime::Now()).GetTotalMicroseconds()); TRACE_LOG(GeneralLog,FString::SanitizeFloat((startTime -
//     FDateTime::Now()).GetTotalMicroseconds())); TRACE_LOG_PRINTF(GeneralLog,"Time since Function start:
//     %f",(startTime - FDateTime::Now()).GetTotalMicroseconds());
//     TRACE_WARNING(GeneralLog,FString::SanitizeFloat((startTime - FDateTime::Now()).GetTotalMicroseconds()));
//     TRACE_WARNING_PRINTF(GeneralLog,"Time since Function start: %f",(startTime -
//     FDateTime::Now()).GetTotalMicroseconds()); TRACE_ERROR(GeneralLog,FString::SanitizeFloat((startTime -
//     FDateTime::Now()).GetTotalMicroseconds())); TRACE_ERROR_PRINTF(GeneralLog,"Time since Function start:
//     %f",(startTime - FDateTime::Now()).GetTotalMicroseconds());
//     // This crashes your game
//     // TRACE_FATAL(GeneralLog, FString::SanitizeFloat((startTime - FDateTime::Now()).GetTotalMicroseconds()));
//     // TRACE_FATAL_PRINTF(GeneralLog, "Time since Function start: %d", (startTime -
//     FDateTime::Now()).GetTotalMicroseconds());
//   }
//
// };
