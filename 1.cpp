#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

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

void personagens(){
    cout << endl << endl;
    cout << "1 - Harry" << endl;
    cout << "1 - Ron" << endl;
    cout << "1 - Hermione" << endl;
    cout << "1 - Sirius Black" << endl;
    cout << "1 - Severus Snape" << endl;
    cout << "Informe o personagem escolido: " << endl;
    cout << " =====================================================" << endl;
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
    inicio();
    personagens();
    int user = 0;
    cin >> user;
    if(user != 1 && user != 2 && user != 3 && user != 4 && user != 5){
        cout << "Ops, opçao invalida!!";
        return 1;
    }

    ler_historia(user);

    int opcao_historia = 0;
    cin >> opcao_historia;
    mostrar_historia(opcao_historia);


}