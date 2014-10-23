#ifndef _SNODEFINITIONS_H_
#define _SNODEFINITIONS_H_
#include "Types.h"

typedef void *HANDLE;

namespace SNOTypes
{
	enum SNOGroup
	{
		Code = -2,
		None = -1,
		Actor = 1,
		Adventure = 2,
		AiBehavior = 3,
		AiState = 4,
		AmbientSound = 5,
		Anim = 6,
		Anim2D = 7,
		AnimSet = 8,
		Appearance = 9,
		Hero = 10,
		Cloth = 11,
		Conversation = 12,
		ConversationList = 13,
		EffectGroup = 14,
		Encounter = 15,
		Explosion = 17,
		FlagSet = 18,
		Font = 19,
		GameBalance = 20,
		Globals = 21,
		LevelArea = 22,
		Light = 23,
		MarkerSet = 24,
		Monster = 25,
		Observer = 26,
		Particle = 27,
		Physics = 28,
		Power = 29,
		Quest = 31,
		Rope = 32,
		Scene = 33,
		SceneGroup = 34,
		Script = 35,
		ShaderMap = 36,
		Shaders = 37,
		Shakes = 38,
		SkillKit = 39,
		Sound = 40,
		SoundBank = 41,
		StringList = 42,
		Surface = 43,
		Textures = 44,
		Trail = 45,
		UI = 46,
		Weather = 47,
		Worlds = 48,
		Recipe = 49,
		Condition = 51,
		TreasureClass = 52,
		Account = 53,
		Conductor = 54,
		TimedEvent = 55,
		Act = 56,
		Material = 57,
		QuestRange = 58,
		Lore = 59,
		Reverb = 60,
		PhysMesh = 61,
		Music = 62,
		Tutorial = 63,
		BossEncounter = 64
	};
}

namespace SNOFiles
{
	struct SNOFile //Runtime only class, used by the SNO manager and other classes.
	{
		uint32 uSNOIndex;
		uint32 uSNOType;
		QString SNOName;
		HANDLE hStormFile;
	};

	struct SNOHeader
	{
		uint32 uDeadBeef;
		uint32 uSNOType;
		uint32 Unknown[2];
	};

	struct MipMapLevel
	{
		uint32 uOffset;
		uint32 uSize;
	};

	struct TextureNameEntry //Number of file name entries matches the Atlas Count
	{
		char szName[256];
		char szInternalPathName[256];
		uint32 uIndex;
	};

	//GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	//GL_COMPRESSED_RGBA_S3TC_DXT3_EXT

	struct TextureHeader
	{
		uint32 uSNOIndex;
		uint32 Unknown00[2];
		uint32 uFlags;
		uint32 uPixelFormat;
		uint32 uWidth;
		uint32 uHeight;
		uint32 uMipMapCount; //I doubt this is the mipmap count
		uint32 Unknown01[2];
		MipMapLevel levels[31];
		uint32 uFrameType;
		uint32 Unknown02;
		uint32 uPixelDataOffset; //Runtime?
		uint32 Unknown03[55];
		uint32 uAtlasCount;
		uint32 uAtlasOffset;
		uint32 Unknown04[16];
		char szInternalFilePathName[256];
		uint32 Unknown05[6];
	};
}

#endif //#ifndef _SNODEFINITIONS_H_
