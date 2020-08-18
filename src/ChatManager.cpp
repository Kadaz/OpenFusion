#include "CNShardServer.hpp"
#include "CNStructs.hpp"
#include "ChatManager.hpp"
#include "PlayerManager.hpp"

void ChatManager::init() {
    REGISTER_SHARD_PACKET(P_CL2FE_REQ_SEND_FREECHAT_MESSAGE, chatHandler);
    REGISTER_SHARD_PACKET(P_CL2FE_REQ_PC_AVATAR_EMOTES_CHAT, emoteHandler);
}

void ChatManager::chatHandler(CNSocket* sock, CNPacketData* data) {
    sP_CL2FE_REQ_SEND_FREECHAT_MESSAGE* chat = (sP_CL2FE_REQ_SEND_FREECHAT_MESSAGE*)data->buf;

    // stubbed for now
}

void ChatManager::emoteHandler(CNSocket* sock, CNPacketData* data) {
    // you can dance with friends!!!!!!!!

    sP_CL2FE_REQ_PC_AVATAR_EMOTES_CHAT* emote = (sP_CL2FE_REQ_PC_AVATAR_EMOTES_CHAT*)data->buf;

    PlayerView plr = PlayerManager::players[sock];
    
    // send to client
    sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT* resp = (sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT*)xmalloc(sizeof(sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT));
    resp->iEmoteCode = emote->iEmoteCode;
    resp->iID_From = plr.plr.iID;
    sock->sendPacket(new CNPacketData((void*)resp, P_FE2CL_REP_PC_AVATAR_EMOTES_CHAT, sizeof(sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT), sock->getFEKey()));

    // send to visible players (players within render distance)
    for (CNSocket* otherSock : plr.viewable) {
        resp = (sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT*)xmalloc(sizeof(sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT));
        resp->iEmoteCode = emote->iEmoteCode;
        resp->iID_From = plr.plr.iID;
        otherSock->sendPacket(new CNPacketData((void*)resp, P_FE2CL_REP_PC_AVATAR_EMOTES_CHAT, sizeof(sP_FE2CL_REP_PC_AVATAR_EMOTES_CHAT), otherSock->getFEKey()));
    }
}
