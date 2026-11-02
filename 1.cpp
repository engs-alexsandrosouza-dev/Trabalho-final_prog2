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

struct jogador{
	string nomeP;
	int hp;
	int hf;
	int hi;
	int hm;
};

jogador personagens[4];
int total_armadura = 2;
armadura armadura_jogo[5];
armadura Ainventario[2];    
bool slot_ocupado[2] = {false, false};

void cadastrar_persona(){
personagens[0].nomeP = " Luna ";
personagens[0].hp = 100;
personagens[0].hf = 75;
personagens[0].hm =50;
personagens[0].hi = 80;

personagens[1].nomeP = "Niville Longbotton";
personagens[1].hp = 100;
personagens[1].hf = 80;
personagens[1].hm = 50;
personagens[1].hi = 60;

personagens[2].nomeP = "Simas Finnigan";
personagens[2].hp  = 100;
personagens[2].hf  = 80;
personagens[2].hm  = 45;
personagens[2].hi  = 60;

personagens[3].nomeP = "Cedrico Diggory";
personagens[3].hp  = 100;
personagens[3].hf  = 90;
personagens[3].hm = 50;
personagens[3].hi = 75;
}

void cadastrar_armadura(){
armadura_jogo[0].nome = "manto inicial";
armadura_jogo[0].defesa =  5;

armadura_jogo[1].nome = " tenis voador";
armadura_jogo[1].defesa = 25;

armadura_jogo[2].nome = "elmo viking";
armadura_jogo[2].defesa = 15;

armadura_jogo[3].nome = "capa da invisibilidade";
armadura_jogo[3].defesa = 70;

armadura_jogo[4].nome = "calcinha do dumbledore";
armadura_jogo[4].defesa = 70;

}
void mapa_dos_marotos(){
    char mapa [30][60];
    for (int i = 0; i < 30; i++){
        for (int j = 0; j < 60; j++){
            mapa [i][j] = '_';
        }
    }

for (int i = 0; i < 30; i++){
    for (int j = 0; j < 60; j++){
        cout << mapa[i][j];
        cout << " ";
    }
    cout << endl;
}
}



void inicio(){
    cout << " =====================================================" << endl;
    cout  << "HARRY POTTER" << endl;
    cout << "                              " << endl;
    cout << "Escolha seu personagem!" << endl;
    cout << " =====================================================" << endl;
}

void personagem(){
	for(int i = 0; i < 4; i++){
	cout << i + 1 << " - " << personagens[i].nomeP << endl;
	}
cout << "escolha os peresonagens";
}

void ler_historia(int user){
    cout << endl << endl;
    cout << "História 1"<< endl;
    if (user == 1){
        cout << "tatata";
    }
    cout << "Historia 1"<< endl;
    cout << "Historia 1"<< endl;
    cout << endl << endl;
    cout << "Informe a história escolhida para conhecer sua missão! " << endl;
    cout << " =====================================================" << endl;
}

int mostrar_historia(int opcao_historia){
    switch (opcao_historia){
        case 1: 
            cout << "historia 1" << endl;
            mapa_dos_marotos();
            break;

        case 2:
            cout << "Historia 2";
            mapa_dos_marotos();
            break;

        case 3: 
            cout << "Historia 3";
            mapa_dos_marotos();
            break;
    }

    if (opcao_historia != 1 || opcao_historia != 2 || opcao_historia != 3) {
        cout << "Opçao invalida!!";
        return 0;
    }
}



int main (){
    cadastrar_persona();
    inicio();
    personagem();


}


