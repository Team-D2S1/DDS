// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Socket/SteamHelper.h"
#include "steam/steam_api.h"

class IOnlineSubsystem;

UTexture2D* SteamHelper::GetSteamProfileImage()
{
	if(!SteamAPI_Init()) return nullptr;

	uint32 Width, Height;
	
	ISteamFriends* Friends = SteamFriends();
	ISteamUser* User = SteamUser();
	if(Friends && User)
	{
		const int Avatar = Friends->GetLargeFriendAvatar(User->GetSteamID());
		if(Avatar == -1) return nullptr;
		if(ISteamUtils* Utils = SteamUtils())
		{
			Utils->GetImageSize(Avatar, &Width, &Height);
			TArray<uint8> AvatarRGBA;
			AvatarRGBA.SetNumUninitialized(Width * Height * 4);

			if(Utils->GetImageRGBA(Avatar, AvatarRGBA.GetData(), AvatarRGBA.Num() * sizeof(uint8)))
			{
				UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
				if(AvatarTexture)
				{
					FTexturePlatformData* PlatformData = AvatarTexture->GetPlatformData();
					if(PlatformData)
					{
						void* TextureData = PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TextureData, AvatarRGBA.GetData(), AvatarRGBA.Num());
						PlatformData->Mips[0].BulkData.Unlock();

						AvatarTexture->UpdateResource();

						return AvatarTexture;
					}
				}
			}
		}
	}
	
	return nullptr;
}