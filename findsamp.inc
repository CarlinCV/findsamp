/*
*   System: FindSAMP
*   Author: domingues93 & Carlos Victor
*   Version: 1.0.0
*
*   Contribuidores: pushline
*
*   Funções:
*   - CheckPlayerVote | Envia uma verificação do jogador.
*
*   Chamados:
*   - OnPlayerVote | Retorna informações do voto.
*
*
*   Créditos:
*   - Equipe do Portal SAMP. (https://servers.portalsamp.com)
*
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

#tryinclude <memory>

#if !defined __MEMORY_INCLUDED__
    #error Required dependency: https://github.com/BigETI/pawn-memory
#endif

#tryinclude <map>

#if !defined __MAP_INCLUDED__
    #error Required dependency: https://github.com/BigETI/pawn-map
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
    REQUEST_TIME[MAX_PLAYERS];

new
    Map:LoadRequestToPlayerID;

new 
    RequestsClient:client;

forward OnPlayerVote(playerid, status, bool:voted);

public OnGameModeInit()
{
    client = RequestsClient("https://servers.portalsamp.com/");
}

stock CheckPlayerVote(playerid)
{
    if(!IsPlayerConnected(playerid) || REQUEST_TIME[playerid] > gettime())
    	return 0;

    REQUEST_TIME[playerid] = gettime() + 10;

    new path[128], nickname[MAX_PLAYER_NAME];

    GetPlayerName(playerid, nickname, sizeof(nickname));

    format(path, sizeof(path), "api/voted/%s?nickname=%s", FINDSAMP_SERVER_IP, nickname);
    new Request:id = Request(client, path, HTTP_METHOD_GET, "OnFindsampResponse", .headers = RequestHeaders());
    
    // mapeia o id do request pro playerid
    MAP_insert_val_val(LoadRequestToPlayerID, _:id, playerid);
    return 1;
}

forward OnFindsampResponse(Request:id, E_HTTP_STATUS:status, data[], dataLen);
public OnFindsampResponse(Request:id, E_HTTP_STATUS:status, data[], dataLen)
{
    new 
        // pega o id do player que foi armazenado no checkplayervote
        playerid = MAP_get_val_val(LoadRequestToPlayerID, _:id),
        response;

    MAP_remove_val(LoadRequestToPlayerID, _:id);

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

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit SAMP_FIND_OnGameModeInit

#if defined SAMP_FIND_OnGameModeInit
    forward SAMP_FIND_OnGameModeInit();
#endif
