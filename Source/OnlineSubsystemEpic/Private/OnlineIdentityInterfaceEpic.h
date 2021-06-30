#pragma once
#include "OnlineIdentityInterface.h"
#include "OnlineSubsystemEpic.h"
#include "OnlineSubsystemEpicTypes.h"


class FOnlineIdentityEpic : public IOnlineIdentity
{
public:
	explicit FOnlineIdentityEpic(FOnlineSubsystemEpic& InSubsystem);
	virtual ~FOnlineIdentityEpic() override = default;

	
	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual TArray<TSharedPtr<FUserOnlineAccount>> GetAllUserAccounts() const override;
	virtual TSharedPtr<const FUniqueNetId> GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void RevokeAuthToken(const FUniqueNetId& LocalUserId,
		const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	virtual void GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege,
		const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
	virtual FString GetAuthType() const override;


	PACKAGE_SCOPE:

	static void OnLoginCompleteCallback(const EOS_Auth_LoginCallbackInfo* Data);
	void OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data);
	
	static void OnLogoutCompleteCallback(const EOS_Auth_LogoutCallbackInfo* Data);
	void OnLogoutComplete(const EOS_Auth_LogoutCallbackInfo* Data);

private:
	FOnlineSubsystemEpic& EpicSubsystem;

	TMap<int32, TSharedPtr<const FUniqueNetId>> UserIds;

	TMap<FUniqueNetIdEpic, TSharedRef<FUserOnlineAccount>> UserAccounts;
};

typedef TSharedPtr<FOnlineIdentityEpic, ESPMode::ThreadSafe> FOnlineIdentityEpicPtr;
