#include <STDInclude.hpp>

#include <zlib.h>

#include "FastFiles.hpp"

namespace Components
{
	FastFiles::Key FastFiles::CurrentKey;
	symmetric_CTR FastFiles::CurrentCTR;
	std::vector<std::string> FastFiles::ZonePaths;

	Dvar::Var FastFiles::g_loadingInitialZones;

	bool FastFiles::IsIW4xZone = false;
	bool FastFiles::StreamRead = false;

	char FastFiles::LastByteRead = 0;

	unsigned int FastFiles::CurrentZone;
	unsigned int FastFiles::MaxZones;

	unsigned char FastFiles::ZoneKey[1191] =
	{
		0x30,0x82,0x04,0xA3,0x02,0x01,0x00,0x02,0x82,0x01,0x01,
		0x00,0xBD,0x7E,0xD6,0xE2,0xE9,0x1C,0xB1,0x08,0x68,0xA2,
		0xEC,0xC2,0x47,0xCB,0x60,0x00,0x0F,0xF5,0x6B,0x75,0x32,
		0xA3,0xE4,0xEB,0x85,0x2F,0x23,0x97,0x6E,0x13,0x33,0xBC,
		0xA3,0xBB,0x61,0x88,0xDE,0xB8,0xA8,0x33,0xD6,0xB8,0xA3,
		0xDB,0x48,0x09,0xE8,0x7D,0x0A,0x43,0xBF,0xDB,0x52,0x32,
		0x5C,0x8B,0x0A,0x46,0x01,0x95,0x81,0x97,0x40,0x46,0x28,
		0x73,0x96,0x88,0x7C,0x9A,0x08,0x94,0x62,0xDC,0x10,0xF1,
		0xE7,0x2B,0xF8,0xDE,0x21,0xA2,0xD1,0x39,0x45,0x1A,0xD2,
		0x41,0x45,0xC1,0xEE,0x01,0xFF,0xD6,0x33,0x7F,0xB4,0x64,
		0x41,0x87,0xE6,0x71,0xB7,0x0D,0xE8,0xCF,0xF4,0xCB,0x28,
		0x3C,0xA6,0x92,0x3A,0x69,0x95,0x93,0xBE,0x72,0xA4,0x86,
		0x76,0xF6,0x99,0x7E,0xD8,0xB0,0x63,0x1E,0xC7,0x67,0x8B,
		0xF7,0xF6,0x6B,0x46,0xF1,0xA1,0x7C,0x26,0x15,0x0E,0xF9,
		0xA6,0x3F,0x67,0x54,0x16,0x5C,0xD6,0xAE,0xDE,0x12,0xF8,
		0x30,0x9F,0x5D,0xF2,0xF7,0xAF,0x20,0xAE,0x69,0x06,0x28,
		0x80,0xA9,0xD5,0x94,0x74,0x3D,0xDB,0x5A,0x77,0x87,0xAE,
		0x1D,0x34,0x20,0xF9,0x8F,0x23,0x21,0xCE,0x86,0xB6,0xBF,
		0x8E,0xF3,0xEC,0x4A,0xDF,0xBE,0x12,0x51,0x85,0x4F,0x48,
		0x52,0x2E,0x8A,0x88,0xC6,0x6C,0xD2,0xCD,0x19,0xCF,0xDF,
		0x0A,0x9B,0xB5,0x0B,0x36,0x6D,0x28,0x36,0x57,0xC3,0x84,
		0xDD,0xC2,0xA1,0x1B,0x7A,0xE8,0xE9,0x1F,0xE4,0xC8,0x8E,
		0x8A,0x5A,0x49,0x0F,0xDB,0x8D,0x60,0x48,0xAD,0x81,0x2C,
		0xE8,0x44,0x42,0x57,0xFA,0x7C,0x18,0xAC,0xEF,0x30,0xC9,
		0xC6,0xA0,0x4D,0xFD,0x02,0x03,0x01,0x00,0x01,0x02,0x82,
		0x01,0x00,0x1C,0x18,0x93,0x59,0xDF,0x80,0x5E,0x8B,0x45,
		0xA0,0x6A,0x84,0x3F,0xCA,0xDA,0xB8,0x07,0xA5,0xB6,0xC2,
		0x10,0xB9,0x16,0x37,0x09,0x6F,0x3C,0xD2,0xB6,0x02,0x68,
		0xD8,0x5E,0x5A,0x69,0x12,0xB7,0x1B,0x1F,0xED,0x57,0xB7,
		0xD6,0xAB,0xAB,0x99,0xB4,0x7B,0xDD,0xAA,0xBF,0xE6,0x8F,
		0xE0,0x61,0xB2,0x47,0xDA,0xAB,0x5F,0x74,0x70,0x6D,0x9A,
		0x39,0x63,0x31,0xFD,0x98,0xA3,0xEA,0x03,0xBE,0x48,0xAC,
		0xC6,0x81,0x25,0x16,0xE8,0x30,0x8A,0x88,0x84,0xFA,0x47,
		0x08,0xC7,0x9E,0xC5,0x2B,0x39,0xE6,0xA9,0xE6,0xC6,0xD7,
		0x83,0x49,0xE8,0x11,0x75,0xE8,0xD3,0x4A,0x22,0x93,0x44,
		0x0F,0xFA,0x36,0x24,0x56,0x3E,0xD3,0x6B,0xAD,0x80,0x27,
		0xFE,0xBB,0xE2,0xC2,0x4D,0x79,0x69,0x65,0xB8,0xA7,0xFB,
		0x42,0x9B,0xE9,0x41,0x98,0x60,0xB9,0xEB,0xD8,0xB2,0x25,
		0xAD,0xF1,0xDC,0x0B,0x04,0x9F,0x0F,0xD0,0x96,0x9E,0x1D,
		0x7A,0x73,0xC5,0xE0,0x7A,0x5C,0xF9,0xF8,0x0F,0xCB,0xA1,
		0xFE,0xE8,0x94,0x84,0x6D,0xB4,0x2B,0xFD,0x64,0xC2,0xAF,
		0xD3,0x48,0x35,0xDF,0xCB,0x2D,0xD8,0x2E,0xE8,0x35,0x33,
		0x6E,0xBD,0xE1,0xD5,0x90,0xFC,0x67,0x9B,0xFF,0x9F,0xA0,
		0x54,0x02,0xDB,0xFB,0xCC,0xA8,0x9E,0x1C,0x35,0x88,0x10,
		0x43,0x2B,0xA5,0xCF,0x78,0x4F,0xF0,0xC6,0x74,0x4D,0x47,
		0xA4,0x9B,0x22,0x89,0x56,0xF0,0x6A,0xEA,0xFA,0x6B,0xDD,
		0xDA,0xB1,0x2F,0x55,0x6A,0x5C,0xEF,0xF6,0x31,0xB0,0x87,
		0xAC,0xBE,0xCB,0xAC,0x8D,0xA6,0xC2,0x79,0x75,0x20,0xE0,
		0x61,0x49,0x69,0xA3,0xA1,0x02,0x81,0x81,0x00,0xC9,0x70,
		0xB5,0x6E,0xA6,0x4A,0x49,0xF1,0xA9,0xC7,0xEA,0x6E,0xCF,
		0x23,0xB0,0x29,0x93,0x80,0x2A,0xA9,0x4E,0x29,0x5C,0x3B,
		0xE5,0x9C,0xA5,0x84,0x0C,0x1B,0x9D,0xD4,0xDC,0xA4,0x63,
		0xCD,0xDD,0x70,0x9B,0xBE,0x8F,0x29,0xB3,0x1E,0x06,0x46,
		0xD1,0xE6,0xEC,0xE7,0xB0,0x55,0x44,0xA5,0x24,0x56,0x5F,
		0x28,0x58,0x32,0xCF,0xDC,0x1F,0x74,0x9A,0xB6,0x78,0x83,
		0x08,0x95,0x3D,0x15,0x54,0x23,0x8B,0x15,0x0D,0x9B,0x34,
		0x2B,0x55,0xFE,0x4F,0x26,0x7E,0x59,0x62,0xE8,0x31,0xC1,
		0x8A,0x6E,0xCB,0xB8,0xFA,0xBD,0x39,0xAF,0x9A,0x9E,0xF8,
		0x7B,0x3F,0x0E,0x0A,0x22,0x40,0xD6,0x28,0xA4,0xF5,0x4E,
		0x82,0x45,0xDE,0x81,0xC5,0x7F,0x90,0x44,0x01,0x5F,0x93,
		0xCD,0x95,0x16,0x3D,0xE9,0x02,0x81,0x81,0x00,0xF0,0xD1,
		0xE9,0x8D,0xA9,0xDF,0xFA,0x1E,0xA6,0xFE,0x10,0x3B,0x68,
		0x7C,0x0E,0x52,0xD2,0x19,0x0C,0xA5,0xEB,0xFD,0xB4,0x1A,
		0xDF,0x03,0x5B,0x3E,0x82,0x5D,0x99,0x9B,0x71,0x23,0xFB,
		0xFE,0x69,0xB3,0x2D,0xEB,0xE7,0x67,0xBC,0x9E,0xF9,0x05,
		0xE6,0x4D,0x2B,0x1C,0xE9,0x12,0xFF,0xF3,0xD0,0xC8,0x42,
		0x9F,0x00,0xFE,0x82,0xC3,0x7A,0xB1,0x4F,0x94,0xA1,0xAD,
		0x90,0xE0,0xA4,0x3F,0x78,0x55,0x5A,0x54,0x59,0x4A,0xF9,
		0xEF,0x17,0x40,0xD1,0x09,0xA5,0x0D,0xAC,0xAB,0x8C,0xF0,
		0xFD,0x05,0x6B,0x9A,0xEC,0x3D,0x98,0x95,0x5A,0x33,0xA7,
		0x47,0x16,0xA8,0x9F,0xBA,0xD3,0xBE,0xCD,0x28,0xEE,0x17,
		0x64,0x40,0xBF,0xEC,0x88,0xCD,0x97,0x1F,0xBD,0x3A,0xE6,
		0x16,0xFE,0x0F,0xDE,0xF5,0x02,0x81,0x81,0x00,0xBF,0x7C,
		0xD7,0xCB,0xAE,0x61,0xF2,0x36,0xBA,0xD9,0x62,0xBE,0x21,
		0x44,0x60,0xA2,0xB5,0x27,0x61,0xE6,0x7D,0x79,0x8D,0xC7,
		0x16,0x77,0x39,0x53,0xF4,0x1A,0x90,0x87,0x97,0x92,0xE1,
		0x99,0x01,0xC6,0x99,0x16,0xA5,0x8A,0xD3,0x4D,0x58,0x54,
		0x1C,0x16,0xB3,0xDF,0x6E,0xDD,0x2F,0x9A,0xF8,0x96,0xEE,
		0x70,0x30,0x9F,0x64,0xBE,0x70,0x5C,0x6C,0xF1,0xC6,0x4F,
		0x71,0x6A,0x44,0x9D,0xB0,0xD4,0xF4,0xD2,0x77,0x93,0xB1,
		0x1C,0xFC,0xEA,0xF9,0x9C,0xB3,0x01,0x0F,0xA7,0x80,0x1C,
		0xE6,0x16,0x7A,0xAC,0x86,0x16,0x38,0xEE,0xF8,0x41,0xE4,
		0x1D,0x6C,0x8C,0x51,0x0F,0xCC,0xA8,0x88,0x0C,0x7F,0x70,
		0x39,0x20,0x67,0xEA,0xDE,0xAE,0x6B,0x9A,0x69,0xDF,0xCC,
		0x65,0xE2,0x32,0x39,0x79,0x02,0x81,0x80,0x01,0xDE,0xCF,
		0x7E,0x8F,0x2C,0x33,0x28,0x1B,0xC9,0xEB,0x5C,0x5A,0xC2,
		0x63,0xE6,0x16,0xC5,0xA5,0x08,0x80,0xDD,0xB6,0x91,0x62,
		0xDC,0x06,0xD0,0x64,0x78,0xCF,0xA1,0x9A,0x6E,0x5A,0x1D,
		0xAE,0xBA,0x7A,0x87,0xD3,0x83,0x45,0xBE,0xC2,0x56,0x5E,
		0x64,0x89,0x0A,0x2F,0x71,0x3B,0x55,0xAC,0x70,0x71,0xBC,
		0x04,0x68,0xF5,0xA1,0x09,0x09,0xE9,0x81,0x51,0x04,0x25,
		0x14,0xE9,0x91,0xA8,0xA0,0x99,0x14,0x00,0xA1,0x89,0x71,
		0x66,0xEF,0xD4,0xEF,0xCB,0x3D,0x60,0xF2,0xF0,0x24,0x4B,
		0x02,0xC8,0xC4,0x2A,0x43,0x8C,0x34,0xD4,0xBF,0x83,0xF4,
		0x14,0x63,0xF8,0xE1,0x9D,0x95,0x64,0xC8,0x85,0x98,0xDE,
		0xE9,0x75,0xD4,0x23,0x77,0xDD,0x4D,0x9C,0xCD,0xA1,0x4D,
		0xDA,0x69,0x4B,0x25,0x02,0x81,0x80,0x56,0x6F,0x21,0x4E,
		0xD2,0x95,0x50,0xDD,0x1E,0xFE,0x08,0xCD,0xE0,0x1E,0x97,
		0x98,0x3C,0xED,0xFB,0x7F,0x53,0x4A,0xFC,0x90,0x4B,0x5E,
		0x96,0x09,0x5D,0x57,0x7C,0x5C,0x39,0x21,0x10,0xA5,0xAA,
		0x49,0xB7,0xC9,0x41,0x2B,0xF4,0xAC,0x95,0x07,0xBC,0x45,
		0xCD,0xAE,0x42,0x05,0x12,0xA0,0x1C,0x8E,0x0F,0xF8,0x0A,
		0xB3,0x72,0x50,0xCD,0x0D,0x21,0x2D,0x67,0x81,0x99,0x69,
		0xFF,0xF8,0x5E,0x23,0x19,0x86,0x03,0x3D,0x42,0x6F,0x38,
		0xFB,0x39,0x1C,0xBA,0x96,0x69,0xC6,0xDC,0xE7,0x52,0x7A,
		0x7E,0xC5,0x98,0x03,0x8A,0x3A,0x88,0x7C,0x9A,0xB0,0x70,
		0x26,0x62,0xD6,0xF4,0x13,0x1D,0x37,0x7E,0x33,0x3F,0xBB,
		0xFF,0x91,0x0D,0xA6,0xBB,0x82,0x30,0x34,0xB1,0xD3,0xA4,
		0xA7,0xD6,0x91,
	};


	// This has to be called only once, when the game starts
	void FastFiles::LoadInitialZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		g_loadingInitialZones.set(true);

		std::vector<Game::XZoneInfo> data;
		Utils::Merge(&data, zoneInfo, zoneCount);

		if (FastFiles::Exists("iw4x_patch_mp"))
		{
			data.push_back({ "iw4x_patch_mp", 1, 0 });
		}

		if (Utils::IO::FileExists(Dvar::Var("fs_game").get<std::string>() + "/mod.ff"))
		{
			data.push_back({ "mod", 1, 0 });
		}

		return FastFiles::LoadDLCUIZones(data.data(), data.size(), sync);
	}

	// This has to be called every time the cgame is reinitialized
	void FastFiles::LoadDLCUIZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		std::vector<Game::XZoneInfo> data;
		Utils::Merge(&data, zoneInfo, zoneCount);

		Game::XZoneInfo info = { nullptr, 2, 0 };

		// Custom ui stuff
		if (FastFiles::Exists("iw4x_ui_mp"))
		{
			info.name = "iw4x_ui_mp";
			data.push_back(info);
		}
		else // Fallback
		{
			info.name = "dlc1_ui_mp";
			data.push_back(info);

			info.name = "dlc2_ui_mp";
			data.push_back(info);
		}

		return FastFiles::LoadLocalizeZones(data.data(), data.size(), sync);
	}

	void FastFiles::LoadGfxZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		std::vector<Game::XZoneInfo> data;
		Utils::Merge(&data, zoneInfo, zoneCount);

		if (FastFiles::Exists("iw4x_code_post_gfx_mp"))
		{
			data.push_back({ "iw4x_code_post_gfx_mp", zoneInfo->allocFlags, zoneInfo->freeFlags });
		}

		Game::DB_LoadXAssets(data.data(), data.size(), sync);
	}

	// This has to be called every time fastfiles are loaded :D
	void FastFiles::LoadLocalizeZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		std::vector<Game::XZoneInfo> data;
		Utils::Merge(&data, zoneInfo, zoneCount);

		Game::XZoneInfo info = { nullptr, 4, 0 };

		// Not sure how they should be loaded :S
		std::string langZone = Utils::String::VA("iw4x_localized_%s", Game::Win_GetLanguage());

		if (FastFiles::Exists(langZone))
		{
			info.name = langZone.data();
		}
		else if (FastFiles::Exists("iw4x_localized_english")) // Fallback
		{
			info.name = "iw4x_localized_english";
		}

		data.push_back(info);

		Game::DB_LoadXAssets(data.data(), data.size(), sync);

		Scheduler::OnGameInitialized([]
		{
			g_loadingInitialZones.set(false);
		}, Scheduler::Pipeline::MAIN);
	}

	// Name is a bit weird, due to FasFileS and ExistS :P
	bool FastFiles::Exists(const std::string& file)
	{
		std::string path = FastFiles::GetZoneLocation(file.data());
		path.append(file);

		if (!Utils::String::EndsWith(path, ".ff"))
		{
			path.append(".ff");
		}

		return Utils::IO::FileExists(path);
	}

	bool FastFiles::Ready()
	{
		return (Game::Sys_IsDatabaseReady() && Game::Sys_IsDatabaseReady2());
	}

	const char* FastFiles::GetZoneLocation(const char* file)
	{
		const auto* dir = (*Game::fs_basepath)->current.string;

		std::vector<std::string> paths;
		const std::string fsGame = (*Game::fs_gameDirVar)->current.string;

		if ((file == "mod"s || file == "mod.ff"s) && !fsGame.empty())
		{
			paths.push_back(std::format("{}\\", fsGame));
		}

		if (Utils::String::StartsWith(file, "mp_"))
		{
			std::string zone = file;
			if (Utils::String::EndsWith(zone, ".ff"))
			{
				Utils::String::Replace(zone, ".ff", "");
			}

			std::string filename = zone;

			if (Utils::String::EndsWith(zone, "_load"))
			{
				Utils::String::Replace(zone, "_load", "");
			}

			if (Utils::IO::FileExists(std::format("usermaps\\{}\\{}.ff", zone, filename)))
			{
				return Utils::String::Format("usermaps\\{}\\", zone);
			}
		}

		Utils::Merge(&paths, FastFiles::ZonePaths);

		for (auto& path : paths)
		{
			auto absoluteFile = std::format("{}\\{}{}", dir, path, file);

			// No ".ff" appended, append it manually
			if (!Utils::String::EndsWith(absoluteFile, ".ff"))
			{
				absoluteFile.append(".ff");
			}

			// Check if FastFile exists
			if (Utils::IO::FileExists(absoluteFile))
			{
				return Utils::String::Format("{}", path);
			}
		}

		return Utils::String::Format("zone\\{}\\", Game::Win_GetLanguage());
	}

	void FastFiles::AddZonePath(const std::string& path)
	{
		FastFiles::ZonePaths.push_back(path);
	}

	std::string FastFiles::Current()
	{
		const char* file = (Utils::Hook::Get<char*>(0x112A680) + 4);

		if (file == reinterpret_cast<char*>(4))
		{
			return "";
		}

		return file;
	}

	void FastFiles::ReadXFileHeader(void* buffer, int size)
	{
		if (FastFiles::IsIW4xZone)
		{
			char pad;
			Game::DB_ReadXFile(&pad, 1);
		}

		Game::DB_ReadXFile(buffer, size);
	}

	void FastFiles::ReadVersionStub(unsigned int* version, int size)
	{
		FastFiles::CurrentZone++;
		Game::DB_ReadXFileUncompressed(version, size);

		Zones::SetVersion(*version);

		// Allow loading of codo versions
		if ((*version >= VERSION_ALPHA2 && *version <= 360) || (*version >= 423 && *version <= 461))
		{
			*version = XFILE_VERSION;
		}

		if (*version != XFILE_VERSION)
		{
			Logger::Error(Game::ERR_FATAL, "Zone version {} is not supported!", Zones::Version());
		}
	}

	void FastFiles::ReadHeaderStub(unsigned int* header, int size)
	{
		FastFiles::IsIW4xZone = false;
		FastFiles::LastByteRead = 0;
		Game::DB_ReadXFileUncompressed(header, size);

		if (header[0] == XFILE_HEADER_IW4X)
		{
			FastFiles::IsIW4xZone = true;

			if (header[1] < XFILE_VERSION_IW4X)
			{
				Logger::Error(Game::ERR_FATAL, "The fastfile you are trying to load is outdated ({}, expected {})", header[1], XFILE_VERSION_IW4X);
			}
			else if (header[1] > XFILE_VERSION_IW4X)
			{
				Logger::Error(Game::ERR_FATAL, "You are loading a fastfile that is too new ({}, expected {}), update your game or rebuild the fastfile", header[1], XFILE_VERSION_IW4X);
			}

			*reinterpret_cast<unsigned __int64*>(header) = XFILE_MAGIC_UNSIGNED;
		}
	}

	void FastFiles::AuthLoadInitCrypto()
	{
		if (Zones::Version() >= 319)
		{
			register_hash(&sha256_desc);
			register_cipher(&aes_desc);

			rsa_key key;
			unsigned char encKey[256];
			int hash = find_hash("sha256"), aes = find_cipher("aes"), stat;

			Game::DB_ReadXFileUncompressed(encKey, 256);

			unsigned long outLen = sizeof(FastFiles::CurrentKey);
			rsa_import(FastFiles::ZoneKey, sizeof(FastFiles::ZoneKey), &key);
			rsa_decrypt_key_ex(encKey, 256, FastFiles::CurrentKey.data, &outLen, nullptr, NULL, hash, Zones::Version() >= 359 ? 1 : 2, &stat, &key);
			rsa_free(&key);

			ctr_start(aes, FastFiles::CurrentKey.iv, FastFiles::CurrentKey.key, sizeof(FastFiles::CurrentKey.key), 0, 0, &FastFiles::CurrentCTR);
		}

		Utils::Hook::Call<void()>(0x46FAE0)();
	}

	int FastFiles::AuthLoadInflateCompare(unsigned char* buffer, int length, unsigned char* ivValue)
	{
		if (Zones::Version() >= 319)
		{
			ctr_setiv(ivValue, 16, &FastFiles::CurrentCTR);
			ctr_decrypt(buffer, buffer, length, &FastFiles::CurrentCTR);
		}

		return Utils::Hook::Call<int(unsigned char*, int, unsigned char*)>(0x5BA240)(buffer, length, ivValue);
	}

	static int InflateInitDecrypt(z_streamp strm, const char* version, int stream_size)
	{
		if (Zones::Version() >= 319)
		{
			ctr_decrypt(strm->next_in, const_cast<unsigned char*>(strm->next_in), strm->avail_in, &FastFiles::CurrentCTR);
		}

		return Utils::Hook::Call<int(z_streamp, const char*, int)>(0x4D8090)(strm, version, stream_size);
	}

	void FastFiles::AuthLoadInflateDecryptBaseFunc(unsigned char* buffer)
	{
		if (Zones::Version() >= 319)
		{
			ctr_setiv(FastFiles::CurrentKey.iv, sizeof(FastFiles::CurrentKey.iv), &FastFiles::CurrentCTR);
			ctr_decrypt(buffer, buffer, 8192, &FastFiles::CurrentCTR);
		}
	}

	__declspec(naked) void FastFiles::AuthLoadInflateDecryptBase()
	{
		__asm
		{
			pushad
			push ebx
			call FastFiles::AuthLoadInflateDecryptBaseFunc
			pop ebx
			popad

			push 5B96F0h
			retn
		}
	}

	float FastFiles::GetFullLoadedFraction()
	{
		float singleProgress = 1.0f / FastFiles::MaxZones;
		float partialProgress = singleProgress * (FastFiles::CurrentZone - 1);
		float currentProgress = std::max(std::min(Game::DB_GetLoadedFraction(), 1.0f), 0.0f);
		return std::min(partialProgress + (currentProgress * singleProgress), 1.0f);
	}

	void FastFiles::LoadZonesStub(Game::XZoneInfo* zoneInfo, unsigned int zoneCount)
	{
		FastFiles::CurrentZone = 0;
		FastFiles::MaxZones = zoneCount;

		Utils::Hook::Call<void(Game::XZoneInfo*, unsigned int)>(0x5BBAC0)(zoneInfo, zoneCount);
	}

	__declspec(naked) void FastFiles::ReadXFile(void* /*buffer*/, int /*size*/)
	{
		__asm
		{
			mov eax, [esp + 4]
			mov ecx, [esp + 8]

			push 445468h
			retn
		}
	}

	void FastFiles::ReadXFileStub(char* buffer, int size)
	{
		FastFiles::ReadXFile(buffer, size);

		if (FastFiles::IsIW4xZone)
		{
			for (int i = 0; i < size; ++i)
			{
				buffer[i] ^= FastFiles::LastByteRead;
				Utils::RotLeft(buffer[i], 4);
				buffer[i] ^= -1;
				Utils::RotRight(buffer[i], 6);

				FastFiles::LastByteRead = buffer[i];
			}
		}
	}

#ifdef DEBUG
	void FastFiles::LogStreamRead(int len)
	{
		*Game::g_streamPos += len;

		if (FastFiles::StreamRead)
		{
			std::string data = Utils::String::VA("%d\n", len);
			if (*Game::g_streamPosIndex == 2) data = Utils::String::VA("(%d)\n", len);
			Utils::IO::WriteFile("userraw/logs/iw4_reads.log", data, true);
		}
	}
#endif

	void FastFiles::Load_XSurfaceArray(int atStreamStart, [[maybe_unused]] int count)
	{
		// read the actual count from the varXModelSurfs ptr
		auto surface = *reinterpret_cast<Game::XModelSurfs**>(0x0112A95C);

		// call original read function with the correct count
		return Utils::Hook::Call<void(int, int)>(0x004925B0)(atStreamStart, surface->numsurfs);
	}

	FastFiles::FastFiles()
	{
		Dvar::Register<bool>("ui_zoneDebug", false, Game::DVAR_ARCHIVE, "Display current loaded zone.");
		g_loadingInitialZones = Dvar::Register<bool>("g_loadingInitialZones", true, Game::DVAR_NONE, "Is loading initial zones");

		// Fix XSurface assets
		Utils::Hook(0x0048E8A5, FastFiles::Load_XSurfaceArray, HOOK_CALL).install()->quick();

		// Redirect zone paths
		Utils::Hook(0x44DA90, FastFiles::GetZoneLocation, HOOK_JUMP).install()->quick();

		// Allow loading 'newer' zones
		Utils::Hook(0x4158E7, FastFiles::ReadVersionStub, HOOK_CALL).install()->quick();

		// Allow loading IW4x zones
		Utils::Hook(0x4157B8, FastFiles::ReadHeaderStub, HOOK_CALL).install()->quick();

		// Obfuscate zone data
		Utils::Hook(Game::DB_ReadXFile, FastFiles::ReadXFileStub, HOOK_JUMP).install()->quick();

		// Allow custom zone loading
		if (!ZoneBuilder::IsEnabled())
		{
			Utils::Hook(0x506BC7, FastFiles::LoadInitialZones, HOOK_CALL).install()->quick();
			Utils::Hook(0x60B4AC, FastFiles::LoadDLCUIZones, HOOK_CALL).install()->quick();
			Utils::Hook(0x506B25, FastFiles::LoadGfxZones, HOOK_CALL).install()->quick();
		}

		// basic checks (hash jumps, both normal and playlist)
		Utils::Hook::Nop(0x5B97A3, 2);
		Utils::Hook::Nop(0x5BA493, 2);

		Utils::Hook::Nop(0x5B991C, 2);
		Utils::Hook::Nop(0x5BA60C, 2);

		Utils::Hook::Nop(0x5B97B4, 2);
		Utils::Hook::Nop(0x5BA4A4, 2);

		// allow loading of IWffu (unsigned) files
		Utils::Hook::Set<BYTE>(0x4158D9, 0xEB); // main function
		Utils::Hook::Nop(0x4A1D97, 2); // DB_AuthLoad_InflateInit

		// some other, unknown, check
		// this is replaced by hooks below
		Utils::Hook::Set<BYTE>(0x5B9912, 0xB8);
		Utils::Hook::Set<DWORD>(0x5B9913, 1);

		Utils::Hook::Set<BYTE>(0x5BA602, 0xB8);
		Utils::Hook::Set<DWORD>(0x5BA603, 1);

		// Initialize crypto
		Utils::Hook(0x4D02F0, FastFiles::AuthLoadInitCrypto, HOOK_CALL).install()->quick();

		// Initial stage decryption
		Utils::Hook(0x4D0306, InflateInitDecrypt, HOOK_CALL).install()->quick();

		// Hash bit decryption
		Utils::Hook(0x5B9958, FastFiles::AuthLoadInflateCompare, HOOK_CALL).install()->quick();
		Utils::Hook(0x5B9912, FastFiles::AuthLoadInflateCompare, HOOK_CALL).install()->quick();

		// General read
		Utils::Hook(0x5B98E4, FastFiles::AuthLoadInflateDecryptBase, HOOK_CALL).install()->quick();

		// Fix fastfile progress
		Utils::Hook(0x4E5DE3, FastFiles::LoadZonesStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x407761, FastFiles::GetFullLoadedFraction, HOOK_CALL).install()->quick();
		Utils::Hook(0x49FA1E, FastFiles::GetFullLoadedFraction, HOOK_CALL).install()->quick();
		Utils::Hook(0x589090, FastFiles::GetFullLoadedFraction, HOOK_CALL).install()->quick();
		Utils::Hook(0x629FC0, FastFiles::GetFullLoadedFraction, HOOK_JUMP).install()->quick();

		// XFile header loading
		Utils::Hook(0x4159E2, FastFiles::ReadXFileHeader, HOOK_CALL).install()->quick();

		// Add custom zone paths
		FastFiles::AddZonePath("zone\\patch\\");
		FastFiles::AddZonePath("zone\\dlc\\");

		if (!Dedicated::IsEnabled() && !ZoneBuilder::IsEnabled())
		{
			Scheduler::Loop([]
			{
				if (FastFiles::Current().empty() || !Dvar::Var("ui_zoneDebug").get<bool>()) return;

				auto* const font = Game::R_RegisterFont("fonts/consoleFont", 0);
				float color[4] = { 1.0f, 1.0f, 1.0f, (Game::CL_IsCgameInitialized() ? 0.3f : 1.0f) };

				auto FFTotalSize = *reinterpret_cast<std::uint32_t*>(0x10AA5D8);
				auto FFCurrentOffset = *reinterpret_cast<std::uint32_t*>(0x10AA608);

				float fastfileLoadProgress = (float(FFCurrentOffset) / float(FFTotalSize)) * 100.0f;
				if (std::isinf(fastfileLoadProgress))
				{
					fastfileLoadProgress = 100.0f;
				}
				else if (std::isnan(fastfileLoadProgress))
				{
					fastfileLoadProgress = 0.0f;
				}

				Game::R_AddCmdDrawText(Utils::String::VA("Loading FastFile: %s [%0.1f%%]", FastFiles::Current().data(), fastfileLoadProgress), std::numeric_limits<int>::max(), font, 5.0f, static_cast<float>(Renderer::Height() - 5), 1.0f, 1.0f, 0.0f, color, Game::ITEM_TEXTSTYLE_NORMAL);
			}, Scheduler::Pipeline::RENDERER);
		}

		Command::Add("loadzone", [](const Command::Params* params)
		{
			if (params->size() < 2) return;

			Game::XZoneInfo info;
			info.name = params->get(1);
			info.allocFlags = 1;//0x01000000;
			info.freeFlags = 0;

			Game::DB_LoadXAssets(&info, 1, true);
		});

		Command::Add("awaitDatabase", []()
		{
			Logger::Print("Waiting for database...\n");
			while (!Game::Sys_IsDatabaseReady()) std::this_thread::sleep_for(100ms);
		});

#ifdef _DEBUG
		// ZoneBuilder debugging
		Utils::IO::WriteFile("userraw/logs/iw4_reads.log", "", false);
		Utils::Hook(0x4A8FA0, FastFiles::LogStreamRead, HOOK_JUMP).install()->quick();
		Utils::Hook(0x4BCB62, []
		{
			FastFiles::StreamRead = true;
			Utils::Hook::Call<void(bool)>(0x4B8DB0)(true); // currently set to Load_GfxWorld
			FastFiles::StreamRead = false;
		}, HOOK_CALL).install()/*->quick()*/;
#endif
	}
}
