#pragma once

namespace Components
{
	class Voice : public Component
	{
	public:
		Voice();

		static bool SV_VoiceEnabled();

	private:
		static constexpr auto MAX_VOICE_PACKET_DATA = 256;
		static constexpr auto MAX_SERVER_QUEUED_VOICE_PACKETS = 40;

		static Game::VoicePacket_t voicePackets[Game::MAX_CLIENTS][MAX_SERVER_QUEUED_VOICE_PACKETS];
		static int voicePacketCount[Game::MAX_CLIENTS];

		static bool s_playerMute[Game::MAX_CLIENTS];

		static const Game::dvar_t* sv_voice;

		static void SV_WriteVoiceDataToClient(int clientNum, Game::msg_t* msg);
		static void SV_SendClientVoiceData(Game::client_t* client);
		static void SV_SendClientMessages_Stub(Game::client_t* client, Game::msg_t* msg, unsigned char* snapshotMsgBuf);

		static bool OnSameTeam(Game::gentity_s* ent1, Game::gentity_s* ent2);
		static void SV_QueueVoicePacket(int talkerNum, int clientNum, Game::VoicePacket_t* voicePacket);
		static void G_BroadcastVoice(Game::gentity_s* talker, Game::VoicePacket_t* voicePacket);
		static void SV_UserVoice(Game::client_t* cl, Game::msg_t* msg);
		static void SV_PreGameUserVoice(Game::client_t* cl, Game::msg_t* msg);
		static void SV_VoicePacket(Game::netadr_t from, Game::msg_t* msg);

		static void CL_ClearMutedList();
		static bool CL_IsPlayerMuted_Hk(Game::SessionData* session, int localClientNum, int muteClientIndex);
		static void CL_MutePlayer_Hk(Game::SessionData* session, int muteClientIndex);
		static void Voice_UnmuteMember_Hk(Game::SessionData* session, int clientNum);
		static void CL_TogglePlayerMute(int localClientNum, int muteClientIndex);

		static void CL_WriteVoicePacket_Hk(int localClientNum);
		static void CL_VoicePacket_Hk(int localClientNum, Game::msg_t* msg);

		static void UI_Mute_player(int clientNum, int localClientNum);
		static void UI_Mute_Player_Stub();
	};
}