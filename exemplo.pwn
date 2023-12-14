#include <a_samp>
#include <requests>

#define FINDSAMP_SERVER_IP ("IP:PORT") // Ex: 127.0.0.1:7777
#include <findsamp>

public OnPlayerCommandText(playerid, cmdtext[])
{
    if(strcmp(cmdtext, "/votar", true) == 0) {
        if(CheckPlayerVote(playerid) == 1) {
            SendClientMessage(playerid, -1, "Verificando seu voto...");
        }
        else {
            SendClientMessage(playerid, -1, "Você deve aguardar 10 segundos para executar o comando novamente.");
        }
    }
    return 1;
}

public OnPlayerVote(playerid, status, bool:voted)
{
    static 
        string[148];

    if(status == 200)
    {
        if(voted) {
            new
                payment = random(1000) + 500;

            GivePlayerMoney(playerid, payment);

            format(string, sizeof(string), "Você votou hoje e recebeu $%d por isso! Vote novamente amanhã.", payment);
            SendClientMessage(playerid, -1, string);
        }
        else {
            format(string, sizeof(string), "Você não votou hoje! Vote em: http://servers.portalsamp.com/pt/server/%s#vote", FINDSAMP_SERVER_IP);
            SendClientMessage(playerid, -1, string);
        }
    }
    else if(status == 1) {
        SendClientMessage(playerid, -1, "Você deve aguardar um pouco! Muitas consultas estão sendo feitas nesse momento.");
    }
    else {
        format(string, sizeof(string), "Não foi possível verificar seu voto! (Erro: %d)", status);
        SendClientMessage(playerid, -1, string);
    }
    return 1;
}