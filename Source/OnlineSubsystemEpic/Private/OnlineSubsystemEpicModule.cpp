// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemEpicModule.h"
#include "Core.h"
#include "IPluginManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEpic.h"

IMPLEMENT_MODULE(FOnlineSubsystemEpicModule, OnlineSubsystemEpic)

class FOnlineFactoryEpic : public IOnlineFactory
{
private:
	FOnlineSubsystemEpicPtr EpicSub;

public:

	FOnlineFactoryEpic() {}
	virtual ~FOnlineFactoryEpic() override {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override
	{
		UE_LOG(LogEpicOS, Log, TEXT("TEST UE_LOG"));
		EPIC_OS_LOG(Log, TEXT("TEST EPIC_OS_LOG"));
		UE_LOG_ONLINE(Log, TEXT("TEST UE_LOG_ONLINE"));
		if(!EpicSub.IsValid())
		{
			EpicSub = MakeShared<FOnlineSubsystemEpic, ESPMode::ThreadSafe>(InstanceName);
			if(EpicSub->IsEnabled())
			{
				if(!EpicSub->Init())
				{
					EPIC_OS_LOG(Warning, TEXT("Epic API failed to initialize"));
					EpicSub = nullptr;
				}
			}
			else
			{
				UE_CLOG_ONLINE(IsRunningDedicatedServer() || IsRunningGame(), Warning, TEXT("Epic API disabled!"));
				EpicSub->Shutdown();
				EpicSub = nullptr;
			}
			return EpicSub;
		}
		
		EPIC_OS_LOG(Warning, TEXT("Cannot Create more than one instance of Epic Online Subsystem!"));
		return nullptr;
	}
};

void FOnlineSubsystemEpicModule::StartupModule()
{
	EPIC_OS_LOG(Log, TEXT("Epic Startup!"));

	FString BaseDir = IPluginManager::Get().FindPlugin("OnlineSubsystemEpic")->GetBaseDir();
	
	//Need to load the EOS Dll
	FString LibraryPath;
#if PLATFORM_32BITS
	LibraryPath = FPaths::Combine(BaseDir, TEXT("Source/ThirdParty/EpicSdk/Bin/EOSSDK-Win32-Shipping.dll"));
#elif PLATFORM_64BITS
	LibraryPath = FPaths::Combine(BaseDir, TEXT("Source/ThirdParty/EpicSdk/Bin/EOSSDK-Win64-Shipping.dll"));
#endif

	EOSSdkLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if(!EOSSdkLibraryHandle)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to load EOS SDK library")));
	}
	
	EpicFactory = new FOnlineFactoryEpic();

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(EPIC_SUBSYSTEM, EpicFactory);
}

void FOnlineSubsystemEpicModule::ShutdownModule()
{
	EPIC_OS_LOG(Log, TEXT("Epic Shutdown!"));

	
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService(EPIC_SUBSYSTEM);

	delete EpicFactory;
	EpicFactory = nullptr;
}


DEFINE_LOG_CATEGORY( LogEpicOS );