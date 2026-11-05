#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

struct armadura {
    string nome;
    int defesa;
};

struct jogador {
    string nomeP;
    int hp;
    int hf;
    int hi;
    int hm;
    int xp;
    int level;
};

struct feiticos {
    string name;
    int dano;
    int xp_desbloqueio;
    int chance_acerto;
    bool desbloqueado;
};

feiticos inventario_feitico[10];

jogador personagens[4];

int total_armadura = 2;

armadura armadura_jogo[5];

armadura Ainventario[2];

bool slot_ocupado[2] = {false, false};


void cadastrar_feiticos() {

    inventario_feitico[0].name = "Reducto";
    inventario_feitico[0].dano = 10;
    inventario_feitico[0].xp_desbloqueio = 0;
    inventario_feitico[0].desbloqueado = true;
    inventario_feitico[0].chance_acerto = 85;

    inventario_feitico[1].name = "Estupefaca";
    inventario_feitico[1].dano = 20;
    inventario_feitico[1].xp_desbloqueio = 0;
    inventario_feitico[1].desbloqueado = true;
    inventario_feitico[1].chance_acerto = 85;

    inventario_feitico[2].name = "Incendio";
    inventario_feitico[2].dano = 15;
    inventario_feitico[2].xp_desbloqueio = 0;
    inventario_feitico[2].desbloqueado = true;
    inventario_feitico[2].chance_acerto = 85;

    inventario_feitico[3].name = "Sectumsempra";
    inventario_feitico[3].dano = 25;
    inventario_feitico[3].xp_desbloqueio = 140;
    inventario_feitico[3].chance_acerto = 80;
    inventario_feitico[3].desbloqueado = false;

    inventario_feitico[4].name = "Petrificus Totalus";
    inventario_feitico[4].dano = 30;
    inventario_feitico[4].xp_desbloqueio = 220;
    inventario_feitico[4].chance_acerto = 70;
    inventario_feitico[4].desbloqueado = false;

    inventario_feitico[5].name = "Bombarda";
    inventario_feitico[5].dano = 40;
    inventario_feitico[5].xp_desbloqueio = 400;
    inventario_feitico[5].chance_acerto = 60;
    inventario_feitico[5].desbloqueado = false;

    inventario_feitico[6].name = "Glacius";
    inventario_feitico[6].dano = 65;
    inventario_feitico[6].xp_desbloqueio = 500;
    inventario_feitico[6].chance_acerto = 50;
    inventario_feitico[6].desbloqueado = false;

    inventario_feitico[7].name = "Crucius";
    inventario_feitico[7].dano = 80;
    inventario_feitico[7].xp_desbloqueio = 800;
    inventario_feitico[7].chance_acerto = 40;
    inventario_feitico[7].desbloqueado = false;

    inventario_feitico[8].name = "Imperium";
    inventario_feitico[8].dano = 80;
    inventario_feitico[8].xp_desbloqueio = 800;
    inventario_feitico[8].chance_acerto = 40;
    inventario_feitico[8].desbloqueado = false;

    inventario_feitico[9].name = "Avada Kedavra";
    inventario_feitico[9].dano = 10000;
    inventario_feitico[9].xp_desbloqueio = 1300;
    inventario_feitico[9].chance_acerto = 30;
    inventario_feitico[9].desbloqueado = false;
}


void cadastrar_persona() {

    personagens[0].nomeP = "Luna";
    personagens[0].hp = 100;
    personagens[0].hf = 75;
    personagens[0].hm = 50;
    personagens[0].hi = 80;
    personagens[0].xp = 0;

    personagens[1].nomeP = "Neville Longbottom";
    personagens[1].hp = 100;
    personagens[1].hf = 80;
    personagens[1].hm = 50;
    personagens[1].hi = 60;
    personagens[1].xp = 0;

    personagens[2].nomeP = "Simas Finnigan";
    personagens[2].hp = 100;	
    personagens[2].hf = 80;
    personagens[2].hm = 45;
    personagens[2].hi = 60;
    personagens[2].xp = 0;

    personagens[3].nomeP = "Cedrico Diggory";
    personagens[3].hp = 100;
    personagens[3].hf = 90;
    personagens[3].hm = 50;
    personagens[3].hi = 75;

    for (int i = 0; i < 4; i++) {
        personagens[i].xp = 0;
        personagens[i].level = 1;
    }
}
void verificar_level(jogador &p) {

    int xpPorNivel[10] = {
        0, 140, 200, 400, 580,
        700, 840, 950, 1000, 1200
    };

    while (p.level < 10 &&
           p.xp >= xpPorNivel[p.level]) {

        p.level++;

        cout << "\nLEVEL UP! Nivel "
             << p.level << " alcancado!" << endl;

        int indice = -1;

        switch (p.level) {
            case 2: indice = 3; break;
            case 3: indice = 4; break;
            case 4: indice = 5; break;
            case 6: indice = 6; break;
            case 7: indice = 7; break;
            case 8: indice = 8; break;
            case 10: indice = 9; break;
        }

        if (indice != -1) {
            inventario_feitico[indice].desbloqueado = true;

            cout << "Novo feitico desbloqueado: "
                 << inventario_feitico[indice].name
                 << endl;
        }
    }
}

void adicionar_xp(jogador &p, int quantidade) {

    if (quantidade <= 0) {
        return;
    }

    p.xp += quantidade;

    cout << "\nXP recebido: "
         << quantidade << endl;

    cout << "XP total: " << p.xp << endl;

    verificar_level(p);
}

void mostrar_feitico() {

    cout << "====================================" << endl;
    cout << "        FEITICOS DISPONIVEIS        " << endl;
    cout << "====================================" << endl;

    for (int i = 0; i < 10; i++) {

        if (inventario_feitico[i].desbloqueado == true) {
            cout << inventario_feitico[i].name << endl;
        }
    }

    cout << "====================================" << endl;
}


int escolher_persona() {

    int escolha;

    cout << "Escolha seu personagem: 1-Luna, 2-Neville, 3-Simas, 4-Cedrico" << endl;

    cin >> escolha;

    if (cin.fail() || escolha < 1 || escolha > 4) {
        cout << "Personagem invalido" << endl;
        return -1;
    }

    return escolha - 1;
}


void cadastrar_armadura() {

    armadura_jogo[0].nome = "Manto inicial";
    armadura_jogo[0].defesa = 5;

    armadura_jogo[1].nome = "Tenis voador";
    armadura_jogo[1].defesa = 25;

    armadura_jogo[2].nome = "Elmo viking";
    armadura_jogo[2].defesa = 15;

    armadura_jogo[3].nome = "Capa da invisibilidade";
    armadura_jogo[3].defesa = 70;

    armadura_jogo[4].nome = "Calcinha do Dumbledore";
    armadura_jogo[4].defesa = 70;
}


void mapa_dos_marotos() {

    char mapa[30][60];

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 60; j++) {
            mapa[i][j] = '_';
        }
    }

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 60; j++) {
            cout << mapa[i][j] << " ";
        }

        cout << endl;
    }
}


void escolher_casa() {

    char q1, q2, q3, q4;

    cout << "Diante de um monstro antigo guardando um tesouro, qual e seu impulso?" << endl;
    cout << "a) Combate frontal" << endl;
    cout << "b) Estrategia e conhecimento" << endl;
    cout << "c) Atacar pelas sombras" << endl;
    cout << "d) Proteger o grupo" << endl;

    cin >> q1;

    cout << "Qual relicario escolheria?" << endl;
    cout << "a) Uma lamina lendaria" << endl;
    cout << "b) Um tomo antigo" << endl;
    cout << "c) Adagas silenciosas" << endl;
    cout << "d) Protecao aos aliados" << endl;

    cin >> q2;

    cout << "Qual virtude guia suas decisoes?" << endl;
    cout << "a) Destemor" << endl;
    cout << "b) Conhecimento" << endl;
    cout << "c) Adaptabilidade" << endl;
    cout << "d) Lealdade" << endl;

    cin >> q3;

    cout << "Qual caminho escolheria na masmorra?" << endl;
    cout << "a) Caminho com tochas" << endl;
    cout << "b) Caminho com runas" << endl;
    cout << "c) Tunel silencioso" << endl;
    cout << "d) Ponte de pedra" << endl;

    cin >> q4;

    int grif = 0, corv = 0, sons = 0, lufa = 0;

    string casa;

    char respostas[4] = {q1, q2, q3, q4};

    for (int i = 0; i < 4; i++) {

        if (respostas[i] >= 'A' && respostas[i] <= 'D') {
            respostas[i] = respostas[i] + ('a' - 'A');
        }

        if (respostas[i] != 'a' &&
            respostas[i] != 'b' &&
            respostas[i] != 'c' &&
            respostas[i] != 'd') {

            cout << "Opcao invalida!" << endl;
            exit(1);
        }

        switch (respostas[i]) {

            case 'a':
                grif++;
                break;

            case 'b':
                corv++;
                break;

            case 'c':
                sons++;
                break;

            case 'd':
                lufa++;
                break;
        }
    }

    if (grif >= sons && grif >= lufa && grif >= corv) {
        casa = "Grifinoria";
    }
    else if (corv >= grif && corv >= sons && corv >= lufa) {
        casa = "Corvinal";
    }
    else if (sons >= grif && sons >= corv && sons >= lufa) {
        casa = "Sonserina";
    }
    else {
        casa = "Lufa-Lufa";
    }

    cout << "\n====================================" << endl;
    cout << "Sua casa e: " << casa << endl;
    cout << "====================================" << endl;
}


void inicio() {

    cout << "=====================================================" << endl;
    cout << "              MYSTERY OF HOGWARTS                   " << endl;
    cout << "=====================================================" << endl;
}

void barra_info(int vida, int hm, int hi, int hf, int xp) {

    cout << "====================================" << endl;
    cout << "         STATUS DO PERSONAGEM       " << endl;
    cout << "====================================" << endl;

    cout << "Vida: " << vida << "/100" << endl;
    cout << "Habilidade fisica: " << hf << endl;
    cout << "Intelecto: " << hi << endl;
    cout << "Habilidade magica: " << hm << endl;
    cout << "Experiencia: " << xp << " XP" << endl;

    cout << "====================================" << endl;
}


int mostrar_historias() {

    int escolha_historia = 0;

    cout << "==========================================" << endl;
    cout << "||                                      ||" << endl;
    cout << "||         HISTORIAS DISPONIVEIS        ||" << endl;
    cout << "||                                      ||" << endl;
    cout << "||  1. Os instrumentos mortais          ||" << endl;
    cout << "||  2. Os artificios das trevas         ||" << endl;
    cout << "||                                      ||" << endl;
    cout << "==========================================" << endl;

    cout << "Informe a escolha da sua historia: ";

    if (!(cin >> escolha_historia)) {
        cout << "Opcao invalida!!" << endl;
        return -1;
    }

    if (escolha_historia == 1) {
        cout << "Historia escolhida: Os Instrumentos Mortais" << endl;
    }
    else if (escolha_historia == 2) {
        cout << "Historia escolhida: Os Artificios das Trevas" << endl;
    }
    else {
        cout << "Opcao invalida!!" << endl;
        return -1;
    }

    return escolha_historia;
}

void missaoUm(jogador &p) {

    int vidaInimigo = 30;
    int escolha;
    int dano;

    cout << "====================================" << endl;
    cout << "         INICIO DA AVENTURA         " << endl;
    cout << "====================================" << endl;

    cout << p.nomeP << " caminhava pelos corredores de Hogwarts." << endl;

    cout << "O ambiente havia mudado drasticamente desde a chegada de voces sabem quem." << endl;

    cout << "Eu sempre andava imersa... no meu mundinho." << endl;
    cout << "Os duendes felizes haviam sumido." << endl;
    cout << "Culpa dos dementadores. Agora eu era apenas triste." << endl;

    cout << "\nEra estranho." << endl;
    cout << "Hogwarts continuava sendo Hogwarts." << endl;
    cout << "As paredes, as escadas, os retratos..." << endl;
    cout << "Tudo parecia estar no mesmo lugar." << endl;
    cout << "Mas nada estava realmente ali." << endl;

    cout << "\nEu sentia falta dos meus amigos." << endl;
    cout << "Harry, Hermione, Roni..." << endl;
    cout << "Talvez estivessem procurando alguma coisa." << endl;
    cout << "Espero que tenham encontrado." << endl;

    cout << "\nDe repente, ouvi passos." << endl;
    cout << "Passos pesados. Nada amigáveis." << endl;

    cout << "\nUm comensal da morte apareceu subitamente!" << endl;

    cout << "\nBruxo: O que faz perambulando pelos corredores?" << endl;

    cout << "\nEu nao sabia muito bem o que responder." << endl;
    cout << "Talvez estivesse procurando meus pensamentos." << endl;
    cout << "Eles costumavam fugir de mim." << endl;

    cout << "\nBruxo: RESPONDA!" << endl;

    cout << "\nAcho que ele não gostava de esperar." << endl;
    cout << "Que sujeito desagradavel." << endl;

    cout << "\n====================================" << endl;
    cout << "           PRIMEIRO COMBATE         " << endl;
    cout << "====================================" << endl;

    while (p.hp > 0 && vidaInimigo > 0) {

        cout << "\nSua vida: " << p.hp << endl;
        cout << "Vida do inimigo: " << vidaInimigo << endl;

        cout << "\nO que deseja fazer?" << endl;
        cout << "1 - Lancar feitico" << endl;
        cout << "2 - Defender" << endl;
        cout << "3 - Tentar fugir" << endl;
        cout << "Escolha: ";
        cin >> escolha;
  if (escolha == 1) {

            dano = 20;
            vidaInimigo -= dano;

            cout << "\nEu ergui minha varinha." << endl;
            cout << "Nunca gostei muito de machucar as pessoas." << endl;
            cout << "Mas algumas pessoas nao deixam muitas escolhas." << endl;

            cout << "\nEstupefaca!" << endl;
            cout << "O inimigo recebeu " << dano << " de dano!" << endl;

  }
}
    }

void minigame_bandido(jogador &p) {

    int vidaBandido = 85;
    int escolhaFeitico;
    int dano;

    cout << "\n====================================" << endl;
    cout << "       MINIGAME 1 - BANDIDO         " << endl;
    cout << "====================================" << endl;

    while (p.hp > 0 && vidaBandido > 0) {

        cout << "\nSua vida: " << p.hp << endl;
        cout << "Vida do Bandido: " << vidaBandido << endl;

        cout << "\nEscolha seu feitico:" << endl;

        for (int i = 0; i < 10; i++) {
            if (inventario_feitico[i].desbloqueado) {
                cout << i + 1 << " - "
                     << inventario_feitico[i].name << endl;
            }
        }

        cin >> escolhaFeitico;

        if (escolhaFeitico < 1 || escolhaFeitico > 10 ||
            !inventario_feitico[escolhaFeitico - 1].desbloqueado) {
            cout << "Feitico invalido!" << endl;
            continue;
        }

        feiticos escolhido = inventario_feitico[escolhaFeitico - 1];

        if (rand() % 100 < escolhido.chance_acerto) {
            dano = escolhido.dano;
            vidaBandido -= dano;
            cout << "Voce acertou! Dano: " << dano << endl;
        } else {
            cout << "O feitico errou!" << endl;
        }

        if (vidaBandido <= 0) {
    break;
}

// O Bandido tem 70% de chance de acertar -também da pra mudar depois se precisar
int chanceAtaque = rand() % 100;

if (chanceAtaque < 70) {

    p.hp -= 10;

    cout << "O Bandido acertou o ataque!" << endl;
    cout << "Voce perdeu 10 HP." << endl;

} else {

    cout << "O Bandido errou o ataque!" << endl;
    cout << "Voce nao recebeu dano!" << endl;

}
    }

    if (p.hp > 0) {
        cout << "\nVoce derrotou o Bandido!" << endl;
        adicionar_xp(p, 30);
    } else {
        cout << "\nVoce foi derrotado! Fim da campanha." << endl;
    }
}

void minigame_trasgo(jogador &p) {

    int vidaTrasgo = 130;
    int escolhaFeitico;
    int dano;

    cout << "\n====================================" << endl;
    cout << "        MINIGAME 2 - TRASGO         " << endl;
    cout << "====================================" << endl;

    while (p.hp > 0 && vidaTrasgo > 0) {

        cout << "\nSua vida: " << p.hp << endl;
        cout << "Vida do Trasgo: " << vidaTrasgo << endl;

        cout << "\nEscolha seu feitico:" << endl;

        for (int i = 0; i < 10; i++) {
            if (inventario_feitico[i].desbloqueado) {
                cout << i + 1 << " - "
                     << inventario_feitico[i].name << endl;
            }
        }

        cin >> escolhaFeitico;

        if (escolhaFeitico < 1 || escolhaFeitico > 10 ||
            !inventario_feitico[escolhaFeitico - 1].desbloqueado) {
            cout << "Feitico invalido!" << endl;
            continue;
        }

        feiticos escolhido = inventario_feitico[escolhaFeitico - 1];

        if (rand() % 100 < escolhido.chance_acerto) {
            dano = escolhido.dano;
            vidaTrasgo -= dano;
            cout << "Voce acertou! Dano: " << dano << endl;
        } else {
            cout << "O feitico errou!" << endl;
        }

        if (vidaTrasgo <= 0) {
    break;
}

// O Trasgo tem 70% de chance de acertar -da pra mudar depois
int chanceAtaque = rand() % 100;

if (chanceAtaque < 70) {

    p.hp -= 10;

    cout << "O Trasgo acertou o ataque!" << endl;
    cout << "Voce perdeu 10 HP." << endl;

} else {

    cout << "O Trasgo errou o ataque!" << endl;
    cout << "Voce nao recebeu dano!" << endl;

}
    }

    if (p.hp > 0) {

        cout << "\nVoce derrotou o Trasgo!" << endl;

        adicionar_xp(p, 50);

        if (p.level > 3) {

            bool recebeuElmo = false;

            for (int i = 0; i < 2; i++) {

                if (!slot_ocupado[i]) {

                    inventario[i] = armadura_jogo[2];
                    slot_ocupado[i] = true;

                    recebeuElmo = true;
                    break;
                }
            }

            if (recebeuElmo) {
                cout << "\nVoce recebeu o Elmo Viking!" << endl;
            } else {
                cout << "\nInventario cheio! Elmo nao recebido." << endl;
            }

        } else {
            cout << "\nVoce nao possui nivel suficiente para receber o Elmo." << endl;
        }

    } else {
        cout << "\nVoce foi derrotado! Fim da campanha." << endl;
    }
}

void minigame_pedro(jogador &p) {

    int vidaPedro = 140;
    int escolhaFeitico;
    int dano;

    cout << "\n====================================" << endl;
    cout << "   MINIGAME 3 - PEDRO PETTIGREW    " << endl;
    cout << "====================================" << endl;

    cout << "\nPedro Pettigrew está tentando fugir!" << endl;
    cout << "Derrote-o para salvar Bicuço!" << endl;

    while (p.hp > 0 && vidaPedro > 0) {

        cout << "\nSua vida: " << p.hp << endl;
        cout << "Vida de Pedro: " << vidaPedro << endl;

        cout << "\nEscolha seu feitico:" << endl;

        for (int i = 0; i < 10; i++) {
            if (inventario_feitico[i].desbloqueado) {
                cout << i + 1 << " - "
                     << inventario_feitico[i].name << endl;
            }
        }

        cin >> escolhaFeitico;

        if (escolhaFeitico < 1 || escolhaFeitico > 10 ||
            !inventario_feitico[escolhaFeitico - 1].desbloqueado) {
            cout << "Feitico invalido!" << endl;
            continue;
        }

        feiticos escolhido = inventario_feitico[escolhaFeitico - 1];

        if (rand() % 100 < escolhido.chance_acerto) {
            dano = escolhido.dano;
            vidaPedro -= dano;

            cout << "Voce acertou! Dano: " << dano << endl;
        } else {
            cout << "O feitico errou!" << endl;
        }

        if (vidaPedro <= 0) {
            break;
        }

        // Pedro tem 70% de chance de acertar - tambem da pra mudar depois 
        int chanceAtaque = rand() % 100;

        if (chanceAtaque < 70) {
            p.hp -= 10;

            cout << "Pedro acertou o ataque!" << endl;
            cout << "Voce perdeu 10 HP." << endl;
        } else {
            cout << "Pedro errou o ataque!" << endl;
            cout << "Voce nao recebeu dano!" << endl;
        }
    }

    if (p.hp > 0) {
        cout << "\nVoce derrotou Pedro Pettigrew!" << endl;
        cout << "Bicuço foi salvo!" << endl;

        adicionar_xp(p, 70);

    } else {
        cout << "\nVoce foi derrotado! Fim da campanha." << endl;
    }
}

int main() {

    srand(time(0));

    cadastrar_feiticos();
    cadastrar_persona();
    cadastrar_armadura();

    inicio();

    int escolha = escolher_persona();

    if (escolha != -1) {

        cout << "Voce escolheu: " << personagens[escolha].nomeP << endl;

        barra_info(personagens[escolha].hp,
                   personagens[escolha].hm,
                   personagens[escolha].hi,
                   personagens[escolha].hf,
                   personagens[escolha].xp);
                   cout << "Level: "
     << personagens[escolha].level << endl;


        mostrar_feitico();

    }
    else {
        cout << "Opcao invalida!!" << endl;
        return 1;
    }

    escolher_casa();

    int historia = mostrar_historias();

    if (historia == -1) {
        return 1;
    }
    minigame_bandido(personagens[escolha]);

    if (personagens[escolha].hp > 0) {
        minigame_trasgo(personagens[escolha]);
    }
    if (personagens[escolha].hp > 0) {
        minigame_pedro(personagens[escolha]);
    }

    return 0;
}

