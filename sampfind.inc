/*
*   System: FindSAMP
*   Author: domingues93 & Carlos Victor
*   Version: 1.0.0
*
*   Funções:
*   - CheckPlayerVote | Verifica se um jogador já votou ou não no dia.
*
*   Chamados:
*   - OnPlayerVote | Retorna o PlayerID, Status e Voto (true = votou, false = não votou).
*
*
*   Créditos:
*   - Equipe do Portal SAMP. (https://servers.portalsamp.com)
*   - João (pé de feijão) Pedro Alves
*/

#if defined _inc_findsamp
	#undef _inc_findsamp
#endif

#if defined _findsamp_included
	#endinput
#endif
#define _findsamp_included

#tryinclude <requests>

#if !defined _requests_included
    #error Required dependency: https://github.com/Southclaws/pawn-requests
#endif

#if !defined FINDSAMP_SERVER_IP
    #error You need to set your IP in FINDSAMP_SERVER_IP before findsamp.inc
#endif

#if MAX_PLAYERS > 1000
    #define FS_MAX_REQUESTS (1000)
#else
    #define FS_MAX_REQUESTS (MAX_PLAYERS)
#endif

static 
    PLAYERID[FS_MAX_REQUESTS],
    REQUEST_TIME[MAX_PLAYERS];

forward OnPlayerVote(playerid, status, bool:voted);

stock CheckPlayerVote(playerid)
{
    if(!IsPlayerConnected(playerid) || REQUEST_TIME[playerid] > gettime())
    	return 0;

    REQUEST_TIME[playerid] = gettime() + 10;

    new RequestsClient:client = RequestsClient("https://servers.portalsamp.com/");
    PLAYERID[_:client] = playerid;

    new path[128], nickname[MAX_PLAYER_NAME];

    GetPlayerName(playerid, nickname, sizeof(nickname));

    format(path, sizeof(path), "api/voted/%s?nickname=%s", FINDSAMP_SERVER_IP, nickname);
    Request(client, path, HTTP_METHOD_GET, "OnFindsampResponse", .headers = RequestHeaders());
    return 1;
}

forward OnFindsampResponse(Request:id, E_HTTP_STATUS:status, data[], dataLen);
public OnFindsampResponse(Request:id, E_HTTP_STATUS:status, data[], dataLen)
{
    new 
        playerid = PLAYERID[_:id],
        response;

    PLAYERID[_:id] = INVALID_PLAYER_ID;

    if(!IsPlayerConnected(playerid)) {
        printf("[FindSAMP]: The player %d has disconnected.", playerid);
        return 0;
    }
    else if(funcidx("OnPlayerVote") == -1) {
        print("[FindSAMP]: The 'OnPlayerVote' callback is not defined.");
        return 0;
    }
    else if(_:id >= FS_MAX_REQUESTS) {
        print("[FindSAMP]: The maximum number of requests has been reached.");
        response = 1;
    }

    if(!response)
        response = _:status;

    REQUEST_TIME[playerid] = gettime() + 10;
    return CallLocalFunction("OnPlayerVote", "iii", playerid, response, strval(data[0]));
}

public OnRequestFailure(Request:id, errorCode, errorMessage[], len)
{
    printf("[FindSAMP]: %s", errorMessage);
    return 1;
}

public OnPlayerConnect(playerid)
{
    REQUEST_TIME[playerid] = 0;    

    #if defined SAMP_FIND_OnPlayerConnect
        return SAMP_FIND_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect SAMP_FIND_OnPlayerConnect

#if defined SAMP_FIND_OnPlayerConnect
    forward SAMP_FIND_OnPlayerConnect(playerid);
#endif