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
