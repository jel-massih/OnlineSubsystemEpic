#include "OnlineIdentityInterfaceEpic.h"

#include "eos_auth.h"
#include "OnlineSubsystemEpicTypes.h"

FOnlineIdentityEpic::FOnlineIdentityEpic(FOnlineSubsystemEpic& InSubsystem)
	:EpicSubsystem(InSubsystem)
{
	
}

bool FOnlineIdentityEpic::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	FString ErrorStr;
	
	if (LocalUserNum < 0 || LocalUserNum >= MAX_LOCAL_PLAYERS)
	{
		ErrorStr = FString::Printf(TEXT("Invalid LocalUserNum=%d"), LocalUserNum);
	}
	else
	{
		TSharedPtr<const FUniqueNetId>* UserId = UserIds.Find(LocalUserNum);
		if(UserId == nullptr)
		{
			if(const auto AuthHandle = EOS_Platform_GetAuthInterface(EpicSubsystem.GetPlatformHandle()))
			{
				EOS_Auth_Credentials Credentials = {};
				Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;

				EOS_Auth_LoginOptions LoginOptions;
				memset(&LoginOptions, 0, sizeof(LoginOptions));
				LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;

				//TODO: For now just support the eos dev auth tool
				UE_LOG_ONLINE_IDENTITY(Display, TEXT("%s"), *FString::Printf(TEXT("Logging Into Epic Services via Dev Auth Tool with Id %s and Token %s"), *AccountCredentials.Id, *AccountCredentials.Token));

				Credentials.Id = TCHAR_TO_ANSI(*AccountCredentials.Id);
				Credentials.Token = TCHAR_TO_ANSI(*AccountCredentials.Token);
				Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;

				LoginOptions.Credentials = &Credentials;
				EOS_Auth_Login(AuthHandle, &LoginOptions, &LocalUserNum, OnLoginCompleteCallback);
				return true;
			}
			else
			{
				ErrorStr = TEXT("Failed To Get EOS AuthHandle");
			}
		}	
		else
		{
			TriggerOnLoginCompleteDelegates(LocalUserNum, true, *UserId->Get(), *ErrorStr);
		}
	}

	if(!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Failed To login to Epic Online Services %s"), *ErrorStr);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, FUniqueNetIdEpic(), ErrorStr);
	}

	return false;
}

bool FOnlineIdentityEpic::Logout(int32 LocalUserNum)
{
	TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if(UserId.IsValid())
	{
		if(const auto AuthHandle = EOS_Platform_GetAuthInterface(EpicSubsystem.GetPlatformHandle()))
		{
			EOS_Auth_LogoutOptions LogoutOptions;
			LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
			LogoutOptions.LocalUserId = static_cast<const FUniqueNetIdEpic>(*UserId.Get()).GetId();

			EOS_Auth_Logout(AuthHandle, &LogoutOptions, &LocalUserNum, OnLogoutCompleteCallback);
			return true;
		}
		
		return true;
	}
	else
	{
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
	}
	return false;
}

bool FOnlineIdentityEpic::AutoLogin(int32 LocalUserNum)
{
	return false;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityEpic::GetUserAccount(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> Result;
	const FUniqueNetIdEpic EpicUserId(UserId);
	const TSharedRef<FUserOnlineAccount>* FoundUserAccount = UserAccounts.Find(EpicUserId);
	if(FoundUserAccount)
	{
		Result = *FoundUserAccount;
	}
	return Result;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityEpic::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount> > Result;

	for (TMap<FUniqueNetIdEpic, TSharedRef<FUserOnlineAccount>>::TConstIterator It(UserAccounts); It; ++It)
	{
		Result.Add(It.Value());
	}

	return Result;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEpic::GetUniquePlayerId(int32 LocalUserNum) const
{
	const TSharedPtr<const FUniqueNetId>* FoundId = UserIds.Find(LocalUserNum);
	if (FoundId != nullptr)
	{
		return *FoundId;
	}
	return nullptr;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEpic::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	if (Bytes && Size > 0)
	{
		const FString StrId(Size, reinterpret_cast<TCHAR*>(Bytes));
		return MakeShareable(new FUniqueNetIdEpic(StrId));
	}
	return nullptr;
}

TSharedPtr<const FUniqueNetId> FOnlineIdentityEpic::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable(new FUniqueNetIdEpic(Str));
}

ELoginStatus::Type FOnlineIdentityEpic::GetLoginStatus(int32 LocalUserNum) const
{
	const TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityEpic::GetLoginStatus(const FUniqueNetId& UserId) const
{
	const TSharedPtr<FUserOnlineAccount> UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid() &&
		UserAccount->GetUserId()->IsValid())
	{
		return ELoginStatus::LoggedIn;
	}
	return ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentityEpic::GetPlayerNickname(int32 LocalUserNum) const
{
	const TSharedPtr<const FUniqueNetId> UniqueId = GetUniquePlayerId(LocalUserNum);
	if (UniqueId.IsValid())
	{
		return GetPlayerNickname(*UniqueId);
	}

	return TEXT("EOS USER");
}

FString FOnlineIdentityEpic::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	const auto UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid())
	{
		return UserAccount->GetDisplayName();
	}
	return UserId.ToString();
}

FString FOnlineIdentityEpic::GetAuthToken(int32 LocalUserNum) const
{
	//@TODO: Implement
	return FString();
}

void FOnlineIdentityEpic::RevokeAuthToken(const FUniqueNetId& LocalUserId,
	const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	//@TODO: Implement
}

void FOnlineIdentityEpic::GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege,
	const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	//@TODO: Implement
}

FPlatformUserId FOnlineIdentityEpic::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const
{
	for (int i = 0; i < MAX_LOCAL_PLAYERS; ++i)
	{
		auto CurrentUniqueId = GetUniquePlayerId(i);
		if (CurrentUniqueId.IsValid() && (*CurrentUniqueId == UniqueNetId))
		{
			return i;
		}
	}

	return PLATFORMUSERID_NONE;
}

FString FOnlineIdentityEpic::GetAuthType() const
{
	return TEXT("Epic");
}

void FOnlineIdentityEpic::OnLoginCompleteCallback(const EOS_Auth_LoginCallbackInfo* Data)
{
	if(const auto EpicSubsystem = static_cast<FOnlineSubsystemEpic*>(IOnlineSubsystem::Get(EPIC_SUBSYSTEM)))
	{
		if(const auto OnlineIdentity = static_cast<FOnlineIdentityEpic*>(EpicSubsystem->GetIdentityInterface().Get()))
		{
			OnlineIdentity->OnLoginComplete(Data);
		}
		else
		{
			UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Epic Online Services Account Login Failed to find Online Identity"));
		}
	}
	else
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Epic Online Services Account Login Failed to find Epic Subsystem"));
	}
}

void FOnlineIdentityEpic::OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data)
{
	const FUniqueNetIdEpic EpicId(Data->LocalUserId);
	UE_LOG_ONLINE_IDENTITY(Display, TEXT("Epic Online Services Account Login Complete, UserId: %s"), *EpicId.ToString());

	FString ErrorStr;
	
	if(const auto AuthHandle = EOS_Platform_GetAuthInterface(EpicSubsystem.GetPlatformHandle()))
	{
		if(Data->ResultCode == EOS_EResult::EOS_Success)
		{
			const int32 LocalUserNum = *(static_cast<int32*>(Data->ClientData));
			TSharedPtr<const FUniqueNetId>* NewUserId = UserIds.Find(LocalUserNum);
			if(!NewUserId || !NewUserId->IsValid() || static_cast<const FUniqueNetIdEpic>(*NewUserId->Get()).GetId() != Data->LocalUserId)
			{
				UserIds.Add(LocalUserNum, MakeShareable(new FUniqueNetIdEpic(Data->LocalUserId)));
				NewUserId = UserIds.Find(LocalUserNum);
			}

			if(!NewUserId->IsValid())
			{
				ErrorStr = FString(TEXT("Unable to get a valid Epic User ID"));
			}
			else
			{
				TriggerOnLoginCompleteDelegates(LocalUserNum, true, *NewUserId->Get(), *ErrorStr);
				TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::NotLoggedIn, ELoginStatus::LoggedIn, *NewUserId->Get());
				return;
			}
		}
		else
		{
			ErrorStr = FString::Printf(TEXT("Failed to login Epic Account with status: %d"), Data->ResultCode);
		}
	}
	else
	{
		ErrorStr = FString(TEXT("Failed to Find Epic Online Services Auth Handle"));
	}

	
	UE_LOG_ONLINE_IDENTITY(Warning, TEXT("%s"), *ErrorStr);
	TriggerOnLoginCompleteDelegates(0, false, FUniqueNetIdEpic(), ErrorStr);
}

void FOnlineIdentityEpic::OnLogoutCompleteCallback(const EOS_Auth_LogoutCallbackInfo* Data)
{
	if(const auto EpicSubsystem = static_cast<FOnlineSubsystemEpic*>(IOnlineSubsystem::Get(EPIC_SUBSYSTEM)))
	{
		if(const auto OnlineIdentity = static_cast<FOnlineIdentityEpic*>(EpicSubsystem->GetIdentityInterface().Get()))
		{
			OnlineIdentity->OnLogoutComplete(Data);
		}
		else
		{
			UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Epic Online Services Account Logout Failed to find Online Identity"));
		}
	}
	else
	{
		UE_LOG_ONLINE_IDENTITY(Warning, TEXT("Epic Online Services Account Logout Failed to find Epic Subsystem"));
	}
}

void FOnlineIdentityEpic::OnLogoutComplete(const EOS_Auth_LogoutCallbackInfo* Data)
{
	UE_LOG_ONLINE_IDENTITY(Display, TEXT("Epic Online Services Account Logout Complete"));

	
	const int32 LocalUserNum = *(static_cast<int32*>(Data->ClientData));

	const TSharedPtr<const FUniqueNetId> UserId = GetUniquePlayerId(LocalUserNum);

	if(UserId.IsValid())
	{
		UserAccounts.Remove(FUniqueNetIdEpic(*UserId));
		UserIds.Remove(LocalUserNum);

		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
		TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::LoggedIn, ELoginStatus::NotLoggedIn, *UserId);
	}
	else
	{
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
	}
}