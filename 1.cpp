#include <iostream>
#include <cmath>
using namespace std;

void inicio(){
    cout << "                              " << endl;
    cout  << "HARRY POTTER" << endl;
    cout << "                              " << endl;
    cout << "Escolha seu personagem!";
}

void personagens(){
    cout << endl << endl;
    cout << "1 - Harry" << endl;
    cout << "1 - Ron" << endl;
    cout << "1 - Hermione" << endl;
    cout << "1 - Sirius Black" << endl;
    cout << "1 - Severus Snape" << endl;
    cout << "Informe o personagem escolido: " << endl;
}

void ler_historia(int user){
    cout << endl << endl;
    cout << "História 1"<< endl;
    cout << "História 1"<< endl;
    cout << "História 1"<< endl;
    cout << endl << endl;
    cout << "Informe a história escolhida para conhecer sua missão! " << endl;
}

void mostrar_historia(int opcao_historia){
    if (opcao_historia == 1){
        cout << "História 1";
    }
    else if (opcao_historia == 2){
        cout << "História 2";
    }
    else if (opcao_historia == 3){
        cout << "História 3";
    }
    else {
        cout << "Opção inválida!! "
        return 0;
    }
}



int main (){
    inicio();
    personagens();
    int user = 0;
    cin >> user;
    if(user != 1 && user != 2 && user != 3 && user != 4 && user != 5){
        cout << "Ops, opção inválida!!"
        return 1;
    }

    ler_historia(user);
    
    int opcao_historia = 0;
    cin >> opcao_historia;
    mostrar_historia(opcao_historia);




}