#include "OnlineSessionInterfaceEpic.h"

FOnlineSessionEpic::FOnlineSessionEpic(FOnlineSubsystemEpic& InSubsystem) :
	EpicSubsystem(InSubsystem)
{
}

FOnlineSessionEpic::~FOnlineSessionEpic()
{
}

FNamedOnlineSession* FOnlineSessionEpic::AddNamedSession(FName SessionName,
	const FOnlineSessionSettings& SessionSettings)
{
	return nullptr;
}

FNamedOnlineSession* FOnlineSessionEpic::AddNamedSession(FName SessionName, const FOnlineSession& Session)
{
	return nullptr;
}

TSharedPtr<const FUniqueNetId> FOnlineSessionEpic::CreateSessionIdFromString(const FString& SessionIdStr)
{
	return nullptr;
}

FNamedOnlineSession* FOnlineSessionEpic::GetNamedSession(FName SessionName)
{
	return nullptr;
}

void FOnlineSessionEpic::RemoveNamedSession(FName SessionName)
{
}

bool FOnlineSessionEpic::HasPresenceSession()
{
	return false;
}

EOnlineSessionState::Type FOnlineSessionEpic::GetSessionState(FName SessionName) const
{
	return EOnlineSessionState::NoSession;
}

bool FOnlineSessionEpic::CreateSession(int32 HostingPlayerNum, FName SessionName,
	const FOnlineSessionSettings& NewSessionSettings)
{
	return false;
}

bool FOnlineSessionEpic::CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName,
	const FOnlineSessionSettings& NewSessionSettings)
{
	return false;
}

bool FOnlineSessionEpic::StartSession(FName SessionName)
{
	return false;
}

bool FOnlineSessionEpic::UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings,
	bool bShouldRefreshOnlineData)
{
	return false;
}

bool FOnlineSessionEpic::EndSession(FName SessionName)
{
	return false;
}

bool FOnlineSessionEpic::DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionEpic::IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId)
{
	return false;
}

bool FOnlineSessionEpic::StartMatchmaking(const TArray<TSharedRef<const FUniqueNetId>>& LocalPlayers, FName SessionName,
	const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionEpic::CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName)
{
	return false;
}

bool FOnlineSessionEpic::CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName)
{
	return false;
}

bool FOnlineSessionEpic::FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionEpic::FindSessions(const FUniqueNetId& SearchingPlayerId,
	const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionEpic::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId,
	const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionEpic::CancelFindSessions()
{
	return false;
}

bool FOnlineSessionEpic::PingSearchResults(const FOnlineSessionSearchResult& SearchResult)
{
	return false;
}

bool FOnlineSessionEpic::JoinSession(int32 LocalUserNum, FName SessionName,
	const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
}

bool FOnlineSessionEpic::JoinSession(const FUniqueNetId& LocalUserId, FName SessionName,
	const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
}

bool FOnlineSessionEpic::FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionEpic::FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionEpic::FindFriendSession(const FUniqueNetId& LocalUserId,
	const TArray<TSharedRef<const FUniqueNetId>>& FriendList)
{
	return false;
}

bool FOnlineSessionEpic::SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionEpic::SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName,
	const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionEpic::SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName,
	const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	return false;
}

bool FOnlineSessionEpic::SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName,
	const TArray<TSharedRef<const FUniqueNetId>>& Friends)
{
	return false;
}

bool FOnlineSessionEpic::GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType)
{
	return false;
}

bool FOnlineSessionEpic::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType,
	FString& ConnectInfo)
{
	return false;
}

FOnlineSessionSettings* FOnlineSessionEpic::GetSessionSettings(FName SessionName)
{
	return nullptr;
}

bool FOnlineSessionEpic::RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited)
{
	return false;
}

bool FOnlineSessionEpic::RegisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players,
	bool bWasInvited)
{
	return false;
}

bool FOnlineSessionEpic::UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId)
{
	return false;
}

bool FOnlineSessionEpic::UnregisterPlayers(FName SessionName, const TArray<TSharedRef<const FUniqueNetId>>& Players)
{
	return false;
}

void FOnlineSessionEpic::RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	const FOnRegisterLocalPlayerCompleteDelegate& Delegate)
{
}

void FOnlineSessionEpic::UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	const FOnUnregisterLocalPlayerCompleteDelegate& Delegate)
{
}

int32 FOnlineSessionEpic::GetNumSessions()
{
	return 0;
}

void FOnlineSessionEpic::DumpSessionState()
{
}
