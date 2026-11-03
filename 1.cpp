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

void inventario_feitico() {

    inventario_feitico[0].name = "Reducto"

    inventario_feitico[0].dano = 0;
    inventario_feitico[0].xp_desbloqueio = 0;
    inventario_feitico[0].desbloqueado = true;
    inventario_feitico[0].chance_acerto = 85;

    inventario_feitico[1].name = "Estupefaça"

    inventario_feitico[1].dano = 20;
    inventario_feitico[1].xp_desbloqueio = 0;
    inventario_feitico[1].desbloqueado = true;
    inventario_feitico[1].chance_acerto = 85;

    inventario_feitico[2].name = "Incendio"

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
    inventario_feitico[5].desbloqueio = false;

    inventario_feitico[6].name = "Glacius";
    inventario_feitico[6].dano = 65;
    inventario_feitico[6].xp_desbloqueio = 500;
    inventario_feitico[6].chance_acerto = 50;
    inventario_feitico[6].desbloqueio = false;

    inventario_feitico[7].name = "Crucius";
    inventario_feitico[7].dano = 80;
    inventario_feitico[7].xp_desbloqueio = 800;
    inventario_feitico[7].chance_acerto = 40;
    inventario_feitico[7].desbloqueio = false;

    inventario_feitico[8].name = "Imperium";
    inventario_feitico[8].dano = 80;
    inventario_feitico[8].xp_desbloqueio = 800;
    inventario_feitico[8].chance_acerto = 40;
    inventario_feitico[8].desbloqueio = false;

    inventario_feitico[9].name = "Avada Kedravra";
    inventario_feitico[9].dano = 10000;
    inventario_feitico[9].xp_desbloqueio = 1300
    inventario_feitico[9].chance_acerto = 30;
    inventario_feitico[9].desbloqueio = false;
}

void cadastrar_persona() {

    personagens[0].nomeP = " Luna ";
    personagens[0].hp = 100;
    personagens[0].hf = 75;
    personagens[0].hm = 50;
    personagens[0].hi = 80;

    personagens[1].nomeP = "Niville Longbotton";
    personagens[1].hp = 100;
    personagens[1].hf = 80;
    personagens[1].hm = 50;
    personagens[1].hi = 60;

    personagens[2].nomeP = "Simas Finnigan";
    personagens[2].hp = 100;
    personagens[2].hf = 80;
    personagens[2].hm = 45;
    personagens[2].hi = 60;

    personagens[3].nomeP = "Cedrico Diggory";
    personagens[3].hp = 100;
    personagens[3].hf = 90;
    personagens[3].hm = 50;
    personagens[3].hi = 75;
}

void cadastrar_armadura() {

    armadura_jogo[0].nome = "manto inicial";
    armadura_jogo[0].defesa = 5;

    armadura_jogo[1].nome = " tenis voador";
    armadura_jogo[1].defesa = 25;

    armadura_jogo[2].nome = "elmo viking";
    armadura_jogo[2].defesa = 15;

    armadura_jogo[3].nome = "capa da invisibilidade";
    armadura_jogo[3].defesa = 70;

    armadura_jogo[4].nome = "calcinha do dumbledore";
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
            cout << mapa[i][j];
            cout << " ";
        }
        cout << endl;
    }
}


void escolher_casa() {
    char q1, q2, q3, q4;

    cout << "Diante de um monstro antigo guardando um tesouro, qual é o seu primeiro impulso?" << endl;
    cout << "a) Avançar imediatamente para um combate frontal." << endl;
    cout << "b) Analisar os padrões da criatura e conjurar um feitiço estratégico." << endl;
    cout << "c) Desaparecer nas sombras e buscar um ponto cego para atacar." << endl;
    cout << "d) Posicionar-se à frente do grupo para absorver o primeiro impacto." << endl;
    cout << "Informe sua resposta (a b c d): " << endl;
    cin >> q1;

    cout << "Qual relicário você escolheria carregar em sua jornada?" << endl;
    cout << "a) Uma lâmina lendária forjada em fogo divino." << endl;
    cout << "b) Um tomo antigo repleto de segredos do cosmos." << endl;
    cout << "c) Um conjunto de adagas leves imbuídas com veneno silencioso." << endl;
    cout << "d) A proteção incondicional aos seus aliados." << endl;
    cout << "Informe sua resposta (a b c d): " << endl;
    cin >> q2;

    cout << "Qual virtude guia suas decisões nos momentos mais difíceis?" << endl;
    cout << "a) O destemor diante do perigo." << endl;
    cout << "b) A busca incansável pela verdade e pelo conhecimento." << endl;
    cout << "c) A capacidade de se adaptar e superar sem ser notado." << endl;
    cout << "d) A proteção incondicional aos seus aliados." << endl;
    cout << "Informe sua resposta (a b c d): " << endl;
    cin >> q3;

    cout << "Em uma masmorra escura, você encontra quatro caminhos. Qual escolhe seguir?" << endl;
    cout << "a) O caminho iluminado por tochas flamejantes, onde rugidos ecoam." << endl;
    cout << "b) A passagem decorada com runas brilhantes e energia mágica." << endl;
    cout << "c) O túnel estreito e silencioso coberto de sombras." << endl;
    cout << "d) A ponte de pedra maciça que exige resistência para ser atravessada." << endl;
    cout << "Informe sua resposta (a b c d): " << endl;
    cin >> q4;

    int grif = 0, corv = 0, sons = 0, lufa = 0;
    string casa;

    switch (q1) {
        case 'a':
            grif += 1;
            break;
        case 'b':
            corv += 1;
            break;
        case 'c':
            sons += 1;
            break;
        case 'd':
            lufa += 1;
            break;
    }

    switch (q2) {
        case 'a':
            grif += 1;
            break;
        case 'b':
            corv += 1;
            break;
        case 'c':
            sons += 1;
            break;
        case 'd':
            lufa += 1;
            break;
    }

    switch (q3) {
        case 'a':
            grif += 1;
            break;
        case 'b':
            corv += 1;
            break;
        case 'c':
            sons += 1;
            break;
        case 'd':
            lufa += 1;
            break;
    }

    switch (q4) {
        case 'a':
            grif += 1;
            break;
        case 'b':
            corv += 1;
            break;
        case 'c':
            sons += 1;
            break;
        case 'd':
            lufa += 1;
            break;
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

    cout << " =====================================================" << endl;
    cout << "HARRY POTTER" << endl;
    cout << "                              " << endl;
    cout << " =====================================================" << endl;
}

int main() {

    srand(time(0));

}
