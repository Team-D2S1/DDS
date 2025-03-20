// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "LobbyGameMode.h"

#include "Session/LobbySession.h"


ALobbyGameMode::ALobbyGameMode()
{
	GameSessionClass = ALobbySession::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(IsRunningDedicatedServer())
	{
		// TODO
		// Dedicated Server 실행과 동시에 BeginPlay가 실행될거임
		// 그럼 여기서 Session Setting 하고 온라인 서브시스템에 등록해야겠지
		// SessionSubsystem에 구현 로직 다 있으니까 그거 사용하면 될듯
	}
}
