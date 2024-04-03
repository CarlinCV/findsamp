# FindSAMP

Esse include verifica e retorna o status de voto de um servidor e nome específico. Você pode utilizar isso para dar recompensa aos seus jogadores em troca de votos em [FindSAMP](https://servers.portalsamp.com/).

Esse include foi feito para ser fácil e prático, você precisa definir apenas o endereço do seu servidor, manipular a callback de retorno. Será anexado um arquivo de exemplo no repositório, você pode ver como funciona.


## Dependências
- [Pawn Requests](https://github.com/Southclaws/pawn-requests) - consulta a API do [FindSAMP](https://servers.portalsamp.com/).
- [Memory access](https://github.com/BigETI/pawn-memory)
- [MAP (Hash-map)](https://github.com/BigETI/pawn-map)

## Funções

Envia uma verificação a API sobre o voto do jogador, retorna as informações em `OnPlayerVote`.

```pawn
CheckPlayerVote(playerid); // 1 = Verificando, 0 = Aguarde 10 segundos
```

## Retornos

Retornar informações sobre o voto de um jogador.

```pawn
public OnPlayerVote(playerid, status, bool:voted) {
    return 1;
}
```

## Exemplo

```pawn
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
```
---
> [!CAUTION]
> Você precisa salvar se o jogador votou no dia, caso contrário ele poderá receber diversas recompensas caso não haja essa verificação básica.

> [!TIP]
> Você pode dar recompensas aos jogadores que votaram em seu servidor, pode ser itens comuns, dinheiro ou VIP's temporários.

## Créditos
- [pushline](https://github.com/pushline) - melhorias no código `findsamp.inc`.
- [CarlinCV](https://github.com/CarlinCV) - correção e reforma do `findsamp.inc`.
- [domingues93](https://github.com/domingues93) - pela base do `findsamp.inc`.
