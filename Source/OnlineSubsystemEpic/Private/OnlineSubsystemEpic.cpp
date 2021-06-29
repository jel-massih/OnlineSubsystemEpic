#include "OnlineSubsystemEpic.h"

#include "OnlineSessionInterfaceEpic.h"
#include "PlatformFilemanager.h"

namespace FNetworkProtocolTypes
{
	const FLazyName Epic(TEXT("Epic"));
}

IOnlineSessionPtr FOnlineSubsystemEpic::GetSessionInterface() const
{
	return SessionInterface;
}

IOnlineFriendsPtr FOnlineSubsystemEpic::GetFriendsInterface() const
{
	return nullptr;
}

bool FOnlineSubsystemEpic::Init()
{
	if(bEpicInit)
	{
		return true;
	}

	//Need to successfully init both SDK and platform
	bEpicInit = InitEpicSDK() && InitEpicPlatform();

	if(bEpicInit)
	{
		SessionInterface = MakeShareable(new FOnlineSessionEpic(*this));
	}
	else
	{
		FOnlineSubsystemImpl::Shutdown();
	}
	
	return bEpicInit;
}

bool FOnlineSubsystemEpic::Shutdown()
{
	UE_LOG_ONLINE(Display, TEXT("FOnlineSubsystemEpic::Shutdown()"));

	FOnlineSubsystemImpl::Shutdown();

	SessionInterface.Reset();

	bEpicInit = false;
	return true;
}

FString FOnlineSubsystemEpic::GetAppId() const
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("EpicAppId"), GEngineIni);
}

FText FOnlineSubsystemEpic::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemEpic", "OnlineServiceName", "Epic Online Services");
}

bool FOnlineSubsystemEpic::IsInitialized() const
{
	return bEpicInit;
}

FString FOnlineSubsystemEpic::GetProductName()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("ProductName"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetProductVersion()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("ProductVersion"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetProductId()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("ProductId"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetSandboxId()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("SandboxId"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetDeploymentId()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("DeploymentId"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetClientId()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("ClientId"), GEngineIni);
}

FString FOnlineSubsystemEpic::GetClientSecret()
{
	return GConfig->GetStr(TEXT("OnlineSubsystemEpic"), TEXT("ClientSecret"), GEngineIni);
}

bool FOnlineSubsystemEpic::InitEpicSDK() const
{
	UE_LOG_ONLINE(Log, TEXT("Epic Online Subsystem Initializing..."));

	const auto EpicProductName = GetProductName();
	if(EpicProductName.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductName key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}
	
	const auto EpicProductVersion = GetProductVersion();
	if(EpicProductVersion.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductVersion key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	EOS_InitializeOptions SDKOptions = {};
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = TCHAR_TO_ANSI(*EpicProductName);
	SDKOptions.ProductVersion = TCHAR_TO_ANSI(*EpicProductVersion);
	SDKOptions.Reserved = nullptr;
	SDKOptions.SystemInitializeOptions = nullptr;
	SDKOptions.OverrideThreadAffinity = nullptr;

	EOS_EResult InitResult = EOS_Initialize(&SDKOptions);
	if(InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Initialization Failed!"));
		return false;
	}

	
	UE_LOG_ONLINE(Log, TEXT("EOS SDK Initialization Success!"));
	return true;
}

bool FOnlineSubsystemEpic::InitEpicPlatform()
{
	UE_LOG_ONLINE(Log, TEXT("Epic Online Services Platform Initializing..."));

	EOS_Platform_Options PlatformOptions = {};
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.bIsServer = IsRunningDedicatedServer() ? EOS_TRUE : EOS_FALSE;
	PlatformOptions.EncryptionKey = "1111111111111111111111111111111111111111111111111111111111111111";
	PlatformOptions.Flags = 0;

#if WITH_EDITOR
	PlatformOptions.Flags |= EOS_PF_LOADING_IN_EDITOR;
#endif

	//Create a cache directory if one doesnt already exist to store player / title storage data 
	FString CacheDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + "/Temp/");
	if(!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*CacheDirectory))
	{
		if(!FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*CacheDirectory))
		{
			UE_LOG_ONLINE(Warning, TEXT("Epic Online Services Failed to create Cache Directory: %s"), *CacheDirectory);
			return false;
		}
	}
	PlatformOptions.CacheDirectory = TCHAR_TO_ANSI(*CacheDirectory);

	const auto EpicProductId = GetProductId();
	if(EpicProductId.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	const auto EpicSandboxId = GetSandboxId();
	if(EpicSandboxId.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing SandboxId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	const auto EpicDeploymentId = GetDeploymentId();
	if(EpicDeploymentId.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing DeploymentId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}
	
	const auto EpicClientId = GetClientId();
	if(EpicDeploymentId.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing DeploymentId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	const auto EpicClientSecret = GetClientSecret();
	if(EpicDeploymentId.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing DeploymentId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	PlatformOptions.ProductId = TCHAR_TO_ANSI(*EpicProductId);
	PlatformOptions.SandboxId = TCHAR_TO_ANSI(*EpicSandboxId);
	PlatformOptions.DeploymentId = TCHAR_TO_ANSI(*EpicDeploymentId);
	PlatformOptions.ClientCredentials.ClientId = TCHAR_TO_ANSI(*EpicClientId);
	PlatformOptions.ClientCredentials.ClientSecret = TCHAR_TO_ANSI(*EpicClientSecret);

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);
	if(!PlatformHandle)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Failed To Create Platform!"));
		return false;
	}
	
	UE_LOG_ONLINE(Log, TEXT("EOS SDK Create Platform Success!"));
	return true;
}
