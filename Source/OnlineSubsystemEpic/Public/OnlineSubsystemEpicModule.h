// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


class FOnlineSubsystemEpicModule : public IModuleInterface
{
private:

	//Class responsible for creating instances of subsystem
	class FOnlineFactoryEpic* EpicFactory;
	
public:

	FOnlineSubsystemEpicModule()
		: EpicFactory(nullptr)
	{}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override { return false; }
	virtual bool SupportsAutomaticShutdown() override { return false; }

private:
	void* EOSSdkLibraryHandle;
};

DECLARE_LOG_CATEGORY_EXTERN( LogEpicOS, Display, All );

#define EPIC_OS_LOG(Verbosity, Format, ...) \
{ \
	UE_LOG(LogEpicOS, Verbosity, TEXT("%s%s"), TEXT("Epic OSS: "), *FString::Printf(Format, ##__VA_ARGS__)); \
}