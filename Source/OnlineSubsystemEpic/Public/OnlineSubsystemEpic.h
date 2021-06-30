#pragma once
#include "eos_sdk.h"
#include "OnlineSubsystemImpl.h"

#define EPIC_SUBSYSTEM FName(TEXT("EPIC"))

typedef TSharedPtr<class FOnlineSessionEpic, ESPMode::ThreadSafe> FOnlineSessionEpicPtr;
typedef TSharedPtr<class FOnlineIdentityEpic, ESPMode::ThreadSafe> FOnlineIdentityEpicPtr;


class FOnlineSubsystemEpic : public FOnlineSubsystemImpl
{
public:
	
	virtual ~FOnlineSubsystemEpic() override = default;

	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual bool Tick(float DeltaTime) override;
	
	virtual FString GetAppId() const override;
	virtual FText GetOnlineServiceName() const override;
	
PACKAGE_SCOPE:
	FOnlineSubsystemEpic() = delete;
	explicit FOnlineSubsystemEpic(FName InInstanceName) :
		FOnlineSubsystemImpl(EPIC_SUBSYSTEM, InInstanceName), bEpicInit(false), PlatformHandle(nullptr)
	{
	}

	bool IsInitialized() const;

	static FString GetProductName();
	static FString GetProductVersion();

	static FString GetProductId();
	static FString GetSandboxId();
	static FString GetDeploymentId();
	static FString GetClientId();
	static FString GetClientSecret();

	EOS_HPlatform GetPlatformHandle() const { return PlatformHandle; }

private:

	bool bEpicInit;

	EOS_HPlatform PlatformHandle;

	bool InitEpicSDK() const;
	bool InitEpicPlatform();

	FOnlineIdentityEpicPtr IdentityInterface;
	FOnlineSessionEpicPtr SessionInterface;
};

namespace FNetworkProtocolTypes
{
	extern const FLazyName Epic;
}

typedef TSharedPtr<FOnlineSubsystemEpic, ESPMode::ThreadSafe> FOnlineSubsystemEpicPtr;
