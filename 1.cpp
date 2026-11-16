#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstdio>
#include <io.h>
using namespace std;

// Prototipo: permite chamar a funcao antes de sua implementacao.
int lerOpcao(int minimo, int maximo);
// ==========================================================
// 1. CADASTROS E ATRIBUTOS
// ==========================================================
struct armadura {
    string nome;
    int defesa;
};
// Dados do personagem: HP = vida, HF = habilidade fisica, HI = intelecto, HM = habilidade magica.
struct jogador {
    string nomeP;
    int hp;
    int hf;
    int hi;
    int hm;
    int xp;
    int level;
};
void prepararProximaEtapa(jogador& p);
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
int armadura_equipada = -1;
int bonusArmadura = 0;
// Cadastrar feiticos.
void cadastrar_feiticos() {
    inventario_feitico[0] = {"Reducto", 10, 0, 85, true};
    inventario_feitico[1] = {"Estupefaca", 20, 0, 85, true};
    inventario_feitico[2] = {"Incendio", 15, 0, 85, true};
    inventario_feitico[3] = {"Sectumsempra", 25, 140, 80, false};
    inventario_feitico[4] = {"Petrificus Totalus", 30, 200, 70, false};
    inventario_feitico[5] = {"Bombarda", 40, 400, 60, false};
    inventario_feitico[6] = {"Glacius", 65, 700, 50, false};
    inventario_feitico[7] = {"Crucius", 80, 840, 40, false};
    inventario_feitico[8] = {"Imperium", 80, 950, 40, false};
    inventario_feitico[9] = {"Avada Kedavra", 120, 1200, 30, false};
}

// Cadastrar os atributos de um personagem.
void cadastrar_personagem(int id, string nome, int hf, int hm, int hi) {
    personagens[id].nomeP = nome;
    personagens[id].hp = 100;
    personagens[id].hf = hf;
    personagens[id].hm = hm;
    personagens[id].hi = hi;
    personagens[id].xp = 0;
    personagens[id].level = 1;
}

// Cadastra os quatro personagens.
void cadastrar_persona() {
    cadastrar_personagem(0, "Luna", 75, 50, 80);
    cadastrar_personagem(1, "Neville Longbottom", 80, 50, 60);
    cadastrar_personagem(2, "Simas Finnigan", 80, 45, 60);
    cadastrar_personagem(3, "Cedrico Diggory", 90, 50, 75);
}

// Cadastrar armadura.
void cadastrar_armadura() {
    armadura_jogo[0] = {"Manto inicial", 10};
    armadura_jogo[1] = {"Tenis voador", 25};
    armadura_jogo[2] = {"Elmo viking", 15};
    armadura_jogo[3] = {"Capa da invisibilidade", 70};
    armadura_jogo[4] = {"Calcinha do Dumbledore", 70};
}

// Exibe as armaduras disponiveis.
void mostrarArmaduras() {
    cout << "\n=== ARMADURAS ===" << endl;
    for (int i = 0; i < 2; i++) {
        if (slot_ocupado[i]) {
            cout << i + 1 << " - "
                 << Ainventario[i].nome
                 << " | Bonus de vida: "
                 << Ainventario[i].defesa
                 << endl;
        }
    }
    cout << "0 - Voltar" << endl;
}

// Equipar armadura.
void equipar_armadura(jogador &p) {
    int escolha;
    mostrarArmaduras();
    cout << "Escolha uma armadura: ";
    escolha = lerOpcao(0, 2);
    if (escolha == 0) {
        return;
    }
    escolha--;
    if (escolha < 0 || escolha >= 2 || !slot_ocupado[escolha]) {
        cout << "Armadura invalida!" << endl;
        return;
    }
    if (armadura_equipada == escolha) {
        return;
    }
    int novaVida = p.hp - bonusArmadura + Ainventario[escolha].defesa;
    if (novaVida <= 0 && Ainventario[escolha].defesa >= 0) {
        cout << "Recupere vida antes da troca." << endl;
        return;
    }
    armadura_equipada = escolha;
    bonusArmadura = Ainventario[escolha].defesa;
    p.hp = max(0, min(100 + bonusArmadura, novaVida));
    cout << "Efeito de vida: " << bonusArmadura << endl;
    cout << "Armadura equipada: " << Ainventario[armadura_equipada].nome << endl;
}

// Verificar level.
void verificar_level(jogador &p) {
    int xpPorNivel[10] = {0, 140, 200, 400, 580, 700, 840, 950, 1000, 1200};
    int feiticoPorNivel[11] = {-1, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9};
    while (p.level < 10 && p.xp >= xpPorNivel[p.level]) {
        p.level++;
        cout << "\nLEVEL UP! Nivel " << p.level << " alcancado!" << endl;
        int indice = feiticoPorNivel[p.level];
        if (indice != -1) {
            inventario_feitico[indice].desbloqueado = true;
            cout << "Novo feitico desbloqueado: " << inventario_feitico[indice].name << endl;
        }
    }
}

// Adicionar xp.
void adicionar_xp(jogador &p, int quantidade) {
    if (quantidade <= 0) {
        return;
    }
    p.xp += quantidade;
    cout << "\nXP recebido: " << quantidade << endl;
    cout << "XP total: " << p.xp << endl;
    verificar_level(p);
}

// Mostrar feitico.
void mostrar_feitico() {
    cout << "====================================" << endl;
    cout << "        FEITICOS DISPONIVEIS        " << endl;
    cout << "====================================" << endl;
    for (int i = 0; i < 10; i++) {
        if (inventario_feitico[i].desbloqueado) {
            cout << inventario_feitico[i].name << endl;
        }
    }
    cout << "====================================" << endl;
}

// Escolher persona.
int escolher_persona() {
    int escolha;
    cout << "Escolha seu personagem: 1-Luna, 2-Neville, 3-Simas, 4-Cedrico" << endl;
    escolha = lerOpcao(1, 4);
    return escolha - 1;
}

// Exibe uma pergunta e valida a resposta.
char perguntarCasa(const string perguntas[], int inicio) {
    for (int i = 0; i < 5; i++) {
        cout << perguntas[inicio + i] << endl;
    }
    string linha;
    while (getline(cin >> ws, linha)) {
        istringstream entrada(linha);
        char resposta, sobra;
        if (!(entrada >> resposta)) {
            continue;
        }
        if (resposta >= 'A' && resposta <= 'D') {
            resposta = resposta + ('a' - 'A');
        }
        if (!(entrada >> sobra) && resposta >= 'a' && resposta <= 'd') {
            return resposta;
        }
        cout << "Opcao invalida. Escolha de a ate d: ";
    }
    throw runtime_error("Entrada encerrada.");
}

// As quatro perguntas e suas alternativas, na ordem do ritual.
const string perguntas[20] = {
    "Diante de um monstro antigo guardando um tesouro, qual e seu impulso?",
        "a) Combate frontal",
        "b) Estrategia e conhecimento",
        "c) Atacar pelas sombras",
        "d) Proteger o grupo",
    "Qual relicario escolheria?",
        "a) Uma lamina lendaria",
        "b) Um tomo antigo",
        "c) Adagas silenciosas",
        "d) Protecao aos aliados",
    "Qual virtude guia suas decisoes?",
        "a) Destemor",
        "b) Conhecimento",
        "c) Adaptabilidade",
        "d) Lealdade",
    "Qual caminho escolheria na masmorra?",
        "a) Caminho com tochas",
        "b) Caminho com runas",
        "c) Tunel silencioso",
        "d) Ponte de pedra",
};

// Escolher casa.
string escolher_casa() {
    string casas[4] = {"Grifinoria",
        "Corvinal",
        "Sonserina",
        "Lufa-Lufa"};
    int pontos[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        pontos[perguntarCasa(perguntas, i * 5) - 'a']++;
    }
    int maior = 0;
    for (int i = 1; i < 4; i++) {
        if (pontos[i] > pontos[maior]) {
            maior = i;
        }
    }
    cout << "\n====================================" << endl;
    cout << "Sua casa e: " << casas[maior] << endl;
    cout << "====================================" << endl;
    return casas[maior];
}

// Inicio.
void inicio() {
    cout << "=====================================================" << endl;
    cout << "              MYSTERY OF HOGWARTS                   " << endl;
    cout << "=====================================================" << endl;
}

// Barra info.
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

// Mostrar historias.
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
    escolha_historia = lerOpcao(1, 2);
    if (escolha_historia == 1) {
        cout << "Historia escolhida: Os Instrumentos Mortais" << endl;
    }
    else {
        cout << "Historia escolhida: Os Artificios das Trevas" << endl;
    }
    return escolha_historia;
}

// ==========================================================
// 2. ESTADO DA CAMPANHA
// ==========================================================
// Estado geral: guarda a posicao, os recursos e as conquistas da partida.
struct EstadoCampanha {
    string nomeJogador;
    string casa;
    string posicao;
    int historia = 1;
    int missaoConcluida = 0;
    int eventosConcluidos = 0;
    int alimentacao = 100;
    int pocoesCura = 5;
    int polissuco = 1;
    int feijoes = 3;
    int sapos = 4;
    int polissucoAteLevel = 0;
    int tentativasEspada = 0;
    int chaveEspada = 0;
    bool espada = false;
    bool tentouBau = false;
    bool recompensaMissao3 = false;
    bool chaveAcesso = false;
    bool fragmentos[3] = {false, false, false};
    bool eventosVistos[5] = {false, false, false, false, false};
    bool eventosVencidos[5] = {false, false, false, false, false};
    bool selo = false;
};
EstadoCampanha campanha;
struct inimigo {
    string nome;
    int vida;
    int dano;
    int chanceAtaque;
};
// ==========================================================
// 3. LISTA ENCADEADA
// ==========================================================
// R5 - Lista encadeada de feiticos: insercao, remocao, travessia.
struct NoFeitico {
    feiticos valor;
    NoFeitico* prox;
};
NoFeitico* listaFeiticos = nullptr;
// Inserir feitico lista.
void inserirFeiticoLista(feiticos f) {
    // new cria o no na memoria dinamica; prox comeca vazio.
    NoFeitico* novo = new NoFeitico{f, nullptr};
    if (listaFeiticos == nullptr) {
        listaFeiticos = novo;
        return;
    }
    NoFeitico* atual = listaFeiticos;
    while (atual->prox != nullptr) {
        atual = atual->prox;
    }
    atual->prox = novo;
}

// Remover feitico lista.
bool removerFeiticoLista(const string& nome) {
    NoFeitico* atual = listaFeiticos;
    NoFeitico* anterior = nullptr;
    while (atual != nullptr && atual->valor.name != nome) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == nullptr) {
        return false;
    }
    // Se for o primeiro no, atualiza o inicio da lista.
    if (anterior == nullptr) {
        listaFeiticos = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    delete atual;
    atual = nullptr;
    return true;
}

// Percorrer lista feiticos.
void percorrerListaFeiticos() {
    for (NoFeitico* n = listaFeiticos; n != nullptr; n = n->prox)
        cout << n->valor.name << " | Dano: " << n->valor.dano << "" << endl;
}

// Liberar lista feiticos.
void liberarListaFeiticos() {
    while (listaFeiticos != nullptr) {
        NoFeitico* remover = listaFeiticos;
        listaFeiticos = listaFeiticos->prox;
        delete remover;
        remover = nullptr;
    }
    listaFeiticos = nullptr;
}

// Sincronizar lista feiticos.
void sincronizarListaFeiticos() {
    liberarListaFeiticos();
    for (int i = 0; i < 10; i++) {
        if (inventario_feitico[i].desbloqueado) {
            inserirFeiticoLista(inventario_feitico[i]);
        }
    }
}
// ==========================================================
// 4. PILHA - HISTORICO
// ==========================================================
// R6 - Pilha: as ultimas decisoes aparecem primeiro (LIFO).
struct NoAcao {
    string descricao;
    NoAcao* prox;
};
NoAcao* topoAcao = nullptr;
// Empilhar acao.
void empilharAcao(const string& texto) {
    // O novo no aponta para o antigo topo: o ultimo entra e sai primeiro.
    topoAcao = new NoAcao{texto, topoAcao};
}
