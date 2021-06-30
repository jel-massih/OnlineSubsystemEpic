#pragma once
#include "CoreOnline.h"
#include "eos_common.h"
#include "OnlineSubsystemEpic.h"

class FUniqueNetIdEpic : public FUniqueNetId
{
private:
	EOS_EpicAccountId Id;

public:
	FUniqueNetIdEpic() { Id = nullptr; }
	FUniqueNetIdEpic(const FUniqueNetIdEpic& InId) { Id = InId.Id; }

	explicit FUniqueNetIdEpic(const FUniqueNetId& InId)
	{
		if(InId.GetSize() == sizeof(EOS_EpicAccountId))
		{
			Id = static_cast<const FUniqueNetIdEpic&>(InId).Id;
		}
	}
	explicit FUniqueNetIdEpic(const EOS_EpicAccountId& InId) { Id = InId; }
	explicit FUniqueNetIdEpic(const FString& IdStr)
	{
		Id = EOS_EpicAccountId_FromString(TCHAR_TO_ANSI(*IdStr));
	}
	
	virtual FName GetType() const override
	{
		return EPIC_SUBSYSTEM;
	}

	// IOnlinePlatformData

	virtual const uint8* GetBytes() const override
	{
		return reinterpret_cast<const uint8*>(&Id);
	}

	virtual int32 GetSize() const override
	{
		return sizeof(EOS_EpicAccountId);
	}

	virtual bool IsValid() const override
	{
		// Not completely accurate, but safe to assume numbers below this is invalid
		return  EOS_EpicAccountId_IsValid(Id) == EOS_TRUE;
	}

	virtual FString ToString() const override
	{
		static char TempBuffer[EOS_EPICACCOUNTID_MAX_LENGTH + 1];
		int32_t TempBufferSize = sizeof(TempBuffer);
		EOS_EpicAccountId_ToString(Id, TempBuffer, &TempBufferSize);
		return FString(TempBuffer);
	}

	virtual FString ToDebugString() const override
	{
		if(IsValid())
		{
			return ToString();
		}
		return TEXT("INVALID");
	}

	EOS_EpicAccountId GetId() const {return Id;}
};
