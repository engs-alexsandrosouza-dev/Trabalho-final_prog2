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

// Desempilhar acao.
bool desempilharAcao(string& texto) {
    if (topoAcao == nullptr) {
        return false;
    }
    NoAcao* antigo = topoAcao;
    texto = antigo->descricao;
    topoAcao = topoAcao->prox;
    delete antigo;
    antigo = nullptr;
    return true;
}

// Mostrar historico.
void mostrarHistorico() {
    cout << "\n====================================" << endl;
    cout << "             HISTORICO              " << endl;
    cout << "====================================" << endl;
    cout << "Mais recente primeiro:" << endl;
    for (NoAcao* n = topoAcao; n != nullptr; n = n->prox) {
        cout << "- " << n->descricao << "" << endl;
    }
}

// Liberar pilha.
void liberarPilha() {
    string descartado;
    while (desempilharAcao(descartado)) {
    }
}
// ==========================================================
// 5. FILA - EVENTOS
// ==========================================================
// R7 - Fila: eventos sao atendidos na ordem em que aparecem (FIFO).
struct NoEvento {
    int indice;
    NoEvento* prox;
};
NoEvento* frenteEvento = nullptr;
NoEvento* trasEvento = nullptr;
// Enfileirar evento.
void enfileirarEvento(int id) {
    // O evento entra no final da fila e sai pela frente.
    NoEvento* novo = new NoEvento{id, nullptr};
    if (trasEvento != nullptr) {
        trasEvento->prox = novo;
    }
    else {
        frenteEvento = novo;
    }
    trasEvento = novo;
}

// Desenfileirar evento.
bool desenfileirarEvento(int& id) {
    if (frenteEvento == nullptr) {
        return false;
    }
    NoEvento* antigo = frenteEvento;
    id = antigo->indice;
    frenteEvento = antigo->prox;
    if (frenteEvento == nullptr) {
        trasEvento = nullptr;
    }
    delete antigo;
    antigo = nullptr;
    return true;
}

// Liberar fila.
void liberarFila() {
    int ignorado;
    while (desenfileirarEvento(ignorado)) {
    }
}
// ==========================================================
// 6. ARVORE BINARIA - BAUS
// ==========================================================
// R8 - Arvore binaria de busca de baus; insercao/busca/in-ordem.
struct NoBau {
    int chave;
    string premio;
    bool coletado = false;
    NoBau* esq;
    NoBau* dir;
};
NoBau* raizBaus = nullptr;
// Inserir bau.
NoBau* inserirBau(NoBau* raiz, int chave, const string& premio) {
    if (raiz == nullptr) {
        return new NoBau{chave, premio, false, nullptr, nullptr};
    }
    if (chave < raiz->chave) {
        raiz->esq = inserirBau(raiz->esq, chave, premio);
    }
    else if (chave > raiz->chave) {
        raiz->dir = inserirBau(raiz->dir, chave, premio);
    }
    return raiz;
}

// Buscar bau.
NoBau* buscarBau(NoBau* raiz, int chave, int& buscas) {
    if (raiz == nullptr) {
        return nullptr;
    }
    ++buscas;
    if (raiz->chave == chave) {
        return raiz;
    }
    if (chave < raiz->chave) {
        return buscarBau(raiz->esq, chave, buscas);
    }
    return buscarBau(raiz->dir, chave, buscas);
}

// Imprimir baus ordem.
void imprimirBausOrdem(NoBau* raiz) {
    if (raiz == nullptr) {
        return;
    }
    imprimirBausOrdem(raiz->esq);
    cout << raiz->chave << " ";
    imprimirBausOrdem(raiz->dir);
}

// Libera a arvore em pos-ordem e anula os ponteiros.
void liberarBaus(NoBau*& raiz) {
    if (raiz == nullptr) {
        return;
    }
    liberarBaus(raiz->esq);
    liberarBaus(raiz->dir);
    delete raiz;
    raiz = nullptr;
}
// ==========================================================
// 7. ORDENACAO E BUSCA
// ==========================================================
// R9 - Insertion sort manual, O(n^2) no pior caso; O(n) no melhor.
// Ordena por dano com Insertion Sort (O(n^2), melhor O(n)).
void ordenarFeiticosPorDano(feiticos itens[], int n) {
    for (int i = 1; i < n; ++i) {
        feiticos chave = itens[i];
        int j = i - 1;
        while (j >= 0 && itens[j].dano > chave.dano) {
            itens[j + 1] = itens[j];
            --j;
        }
        itens[j + 1] = chave;
    }
}
// R3 - Quick sort recursivo adicional, demonstrando recursividade.
// Ordena recursivamente por pivo (medio O(n log n), pior O(n^2)).
void quickSortDanos(int itens[], int inicio, int fim) {
    if (inicio >= fim) {
        return;
    }
    // O ultimo elemento e o pivo. pos marca onde colocar os valores menores.
    int pivo = itens[fim];
    int pos = inicio;
    for (int i = inicio; i < fim; ++i) {
        if (itens[i] <= pivo) {
            int temp = itens[pos];
            itens[pos] = itens[i];
            itens[i] = temp;
            pos++;
        }
    }
    int temp = itens[pos];
    itens[pos] = itens[fim];
    itens[fim] = temp;
    quickSortDanos(itens, inicio, pos - 1);
    quickSortDanos(itens, pos + 1, fim);
}
// R10 - buscas sequencial O(n) e binaria O(log n); contadores.
// Percorre o vetor contando comparacoes.
int buscarSequencial(const feiticos a[], int n, int dano, int& ops) {
    ops = 0;
    for (int i = 0; i < n; ++i) {
        ++ops;
        if (a[i].dano == dano) {
            return i;
        }
    }
    return -1;
}

// Busca dano em vetor ordenado contando comparacoes.
int buscarBinaria(const feiticos a[], int n, int dano, int& ops) {
    ops = 0;
    int e = 0;
    int d = n - 1;
    while (e <= d) {
        int meio = e + (d - e) / 2;
        ++ops;
        if (a[meio].dano == dano) {
            return meio;
        }
        if (a[meio].dano < dano) {
            e = meio + 1;
        }
        else {
            d = meio - 1;
        }
    }
    return -1;
}

// Demonstra Quick Sort recursivo.
void demonstrarQuickSort() {
    int valores[10];
    for (int i = 0; i < 10; ++i) {
        valores[i] = inventario_feitico[i].dano;
    }
    quickSortDanos(valores, 0, 9);
    cout << "Quick sort recursivo (danos): ";
    for (int i = 0; i < 10; i++) {
        cout << valores[i] << " ";
    }
    cout << "" << endl;
}

// Demonstrar ordenacao busca.
void demonstrarOrdenacaoBusca() {
    // Demonstra remocao e reinsercao sem alterar o inventario do jogador.
    if (removerFeiticoLista("Reducto")) {
        inserirFeiticoLista(inventario_feitico[0]);
    }
    feiticos copia[10];
    for (int i = 0; i < 10; ++i) {
        copia[i] = inventario_feitico[i];
    }
    ordenarFeiticosPorDano(copia, 10);
    cout << "\n====================================" << endl;
    cout << "    FEITICOS ORDENADOS POR DANO     " << endl;
    cout << "====================================" << endl;
    for (int i = 0; i < 10; i++) {
        cout << copia[i].name << ": " << copia[i].dano << "" << endl;
    }
    int dano = copia[7].dano;
    int a = 0;
    int b = 0;
    int pos1 = buscarSequencial(copia, 10, dano, a);
    int pos2 = buscarBinaria(copia, 10, dano, b);
    cout << "Busca pelo dano " << dano << ": sequencial=" << a
         << " comparacoes (indice " << pos1 << "), binaria=" << b
         << " comparacoes (indice " << pos2 << ").\n";
    demonstrarQuickSort();
}

// Valida uma escolha numerica dentro do intervalo.
int lerOpcao(int minimo, int maximo) {
    string linha;
    int valor;
    char sobra;
    while (getline(cin >> ws, linha)) {
        istringstream entrada(linha);
        if ((entrada >> valor) && !(entrada >> sobra) && valor >= minimo && valor <= maximo) {
            return valor;
        }
        cout << "Escolha um numero entre " << minimo << " e " << maximo << ": ";
    }
    throw runtime_error("Entrada encerrada.");
}
// ==========================================================
// 8. CONTROLE DE TELA - WINDOWS
// ==========================================================
// Verifica se o jogo esta em um terminal interativo.
bool terminalInterativo() {
    return _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
}

// Limpa apenas a tela do terminal, sem apagar o historico do jogo.
void limparTela() {
    if (!terminalInterativo() || getenv("HOGWARTS_TESTE")) {
        return;
    }
    system("cls");
}

// Aguarda a leitura da tela. Enter avanca; testes nao precisam aguardar.
void pausarTela() {
    if (!terminalInterativo() || getenv("HOGWARTS_TESTE")) {
        return;
    }
    cout << "\nPressione ENTER para continuar..." << flush;
    string linha;
    if (!getline(cin, linha)) {
        throw runtime_error("Entrada encerrada.");
    }
}

// Separa duas telas, sem esconder a anterior antes de sua leitura.
void novaTela() {
    pausarTela();
    limparTela();
}

// Pequena pausa visual opcional, sem obrigar o jogador a esperar.
void atrasoNarrativo() {
    if (terminalInterativo() && !getenv("HOGWARTS_TESTE") && getenv("HOGWARTS_DELAY")) {
        cout << flush;
        this_thread::sleep_for(chrono::milliseconds(250));
    }
}

// ==========================================================
// 9. MAPA E PAINEIS
// ==========================================================
// Retorna a marca do local ocupado pelo jogador.
char marcaMapa(const string& local) {
    if (campanha.posicao == local) {
        return '*';
    }
    string destinos[5] = {"Passagem secreta",
        "Clareira",
        "Cabana de Hagrid",
        "Corredores",
        "Entrada da camara"};
    for (int i = 0; i < 5; i++) {
        if (local == destinos[i] && campanha.eventosVistos[i]) {
            if (campanha.eventosVencidos[i]) {
                return '+';
            }
            return '?';
        }
    }
    return ' ';
}

// Monta o texto de uma sala.
string salaMapa(const string& local, const string& nome) {
    string texto = " ";
    texto += marcaMapa(local);
    texto += " " + nome;
    // Todas as salas possuem a mesma largura interna.
    while (texto.size() < 22) {
        texto += " ";
    }
    return "|" + texto + "|";
}

// Exibe tres salas conectadas.
void linhaSalas(const string& sala1, const string& sala2, const string& sala3) {
    string borda = "+----------------------+";
    cout << borda << "     " << borda << "     " << borda << endl;
    cout << sala1 << "-----" << sala2 << "-----" << sala3 << endl;
    cout << borda << "     " << borda << "     " << borda << endl;
}

// Desenha um mapa visual de Hogwarts.
void mapa_dos_marotos() {
    cout << "\n====================================" << endl;
    cout << "           MAPA DOS MAROTOS         " << endl;
    cout << "====================================" << endl;
    linhaSalas(salaMapa("Hogwarts", "HOGWARTS"), salaMapa("Passagem secreta", "PASSAGEM"), salaMapa("Floresta Proibida", "FLORESTA PROIBIDA"));
    cout << "                                                                     |" << endl;
    linhaSalas(salaMapa("Cabana de Hagrid", "CABANA DE HAGRID"), salaMapa("Gringotes", "GRINGOTES"), salaMapa("Clareira", "CLAREIRA"));
    cout << "           |" << endl;
    linhaSalas(salaMapa("Subsolo", "SUBSOLO"), salaMapa("Corredores", "CORREDORES"), salaMapa("Entrada da camara", "ENTRADA DA CAMARA"));
    cout << "                                                                     |" << endl;
    cout << string(58, ' ') << "+----------------------+" << endl;
    cout << string(58, ' ') << salaMapa("Coracao de Hogwarts", "CORACAO DE HOGWARTS") << endl;
    cout << string(58, ' ') << "+----------------------+" << endl;
    cout << "* Voce esta aqui: " << campanha.posicao << endl;
    cout << "? Desafio pendente | + Desafio concluido" << endl;
}

// Exibe o mapa com a posicao atual.
void mostrarMapaAtual() {
    mapa_dos_marotos();
}


// Mostrar painel.
void mostrarPainel(const jogador& p) {
    cout << "\n====================================" << endl;
    cout << "        STATUS DO PERSONAGEM        " << endl;
    cout << "====================================" << endl;
    cout << "Jogador: " << campanha.nomeJogador << endl;
    cout << p.nomeP << " | Casa: " << campanha.casa << "\n"
         << "HP: " << p.hp << "/" << 100 + bonusArmadura << " | Alimentacao: " << campanha.alimentacao
         << " | XP: " << p.xp << " | Level: " << p.level << "\n"
         << "Habilidade fisica: " << p.hf << " | Intelecto: " << p.hi << " | Habilidade magica: " << p.hm << "\n"
         << "Missoes: " << campanha.missaoConcluida << "/4 | Eventos: "
         << campanha.eventosConcluidos << "/5\n";
}

// Resumo da vida e progresso, sem repetir todos os atributos.
void mostrarResumo(const jogador &p) {
    cout << p.nomeP << " | HP: " << p.hp << "/" << 100 + bonusArmadura
         << " | Alimentacao: " << campanha.alimentacao
         << " | XP: " << p.xp << " | Level: " << p.level << endl;
    cout << "Missoes: " << campanha.missaoConcluida << "/4 | Eventos: "
         << campanha.eventosConcluidos << "/5 (minimo 3)" << endl;
    cout << "Chave: " << (campanha.chaveAcesso ? "sim" : "nao") << " | Fragmentos: "
         << campanha.fragmentos[0] + campanha.fragmentos[1] + campanha.fragmentos[2] << "/3" << endl;
}

// Mostra apenas os consumiveis.
void mostrarSuprimentos() {
    cout << "Pocoes de cura: " << campanha.pocoesCura
         << " | Polissuco: " << campanha.polissuco
         << " | Feijoes magicos: " << campanha.feijoes
         << " | Sapos de chocolate: " << campanha.sapos << endl;
}

// Mostrar inventario completo.
void mostrarInventarioCompleto() {
    cout << "Chave de acesso: " << (campanha.chaveAcesso ? "sim" : "nao") << " | Fragmentos: "
         << campanha.fragmentos[0] + campanha.fragmentos[1] + campanha.fragmentos[2] << "/3" << endl;
    cout << "\n====================================" << endl;
    cout << "             INVENTARIO             " << endl;
    cout << "====================================" << endl;
    mostrarSuprimentos();
    cout << "Armaduras:" << endl;
    for (int i = 0; i < 2; ++i)
        if (slot_ocupado[i]) cout << i + 1 << " - " << Ainventario[i].nome
            << " | Bonus de vida: " << Ainventario[i].defesa << (armadura_equipada == i ? " (equipada)" : "") << endl;
    cout << "Espada da Grifinoria: " << (campanha.espada ? "sim" : "nao") << endl;
}

// ==========================================================
// 10. INVENTARIO E RECURSOS
// ==========================================================
// Vida maxima.
int vidaMaxima() {
    return 100 + bonusArmadura;
}

// Alimentar.
void alimentar(jogador& p) {
    if (campanha.alimentacao >= 100) {
        cout << "Alimentacao ja esta completa." << endl;
        return;
    }
    campanha.alimentacao = min(100, campanha.alimentacao + 25);
    cout << "Voce descansou e recuperou alimentacao." << endl;
    empilharAcao("Descanso e alimentacao");
    p.hp = min(vidaMaxima(), p.hp + 5);
}

// Menu item.
int menuItem() {
    mostrarInventarioCompleto();
    cout << "\n====================================" << endl;
    cout << "             USAR ITEM              " << endl;
    cout << "====================================" << endl;
    cout << "1 - Cura (+50 HP)" << endl;
    cout << "2 - Polissuco (bonus temporario)" << endl;
    cout << "3 - Feijoes (+30/-30 HP)" << endl;
    cout << "4 - Sapo (+5 HP)" << endl;
    cout << "0 - Voltar" << endl;
    cout << "Escolha: ";
    return lerOpcao(0, 4);
}

// Aplica os cinco pontos temporarios em cada habilidade.
void usarPolissuco(jogador &p) {
    --campanha.polissuco;
    p.hf += 5;
    p.hm += 5;
    p.hi += 5;
    campanha.polissucoAteLevel = p.level + 1;
    cout << "Polissuco: bonus de habilidades ate subir de nivel." << endl;
}

// Consumir item.
void consumirItem(jogador& p) {
    int item = menuItem();
    int bonus = 0;
    limparTela();
    if (item == 1 && campanha.pocoesCura > 0) {
        --campanha.pocoesCura;
        bonus = 50;
    }
    else if (item == 2 && campanha.polissuco > 0 && p.level < 10 && campanha.polissucoAteLevel == 0) {
        usarPolissuco(p);
    } else if (item == 3 && campanha.feijoes > 0) {
        campanha.feijoes--;
        if (rand() % 100 < 70) {
            bonus = 30;
        } else {
            bonus = -30;
        }
    } else if (item == 4 && campanha.sapos > 0) {
        --campanha.sapos;
        bonus = 5;
    } else {
        if (item != 0) {
            cout << "Item indisponivel!" << endl;
        }
        return;
    }
    p.hp = max(0, min(vidaMaxima(), p.hp + bonus));
    empilharAcao("Utilizou item " + to_string(item));
    cout << "Vida atual: " << p.hp << "" << endl;
}
// Pausa professor.
void pausaProfessor(jogador& p) {
    cout << "\n" << (p.nomeP == "Neville Longbottom" ? "Luna" : "Neville") << ": Quer continuar ou procurar o professor?\n"
         << "1-Seguir 2-Ir para o professor: ";
    int escolha = lerOpcao(1, 2);
    if (escolha == 2) {
        cout << "\nPROFESSOR - CADERNO DE FEITICOS" << endl;
        for (int i = 0; i < 10; i++) {
            feiticos f = inventario_feitico[i];
            cout << f.name << " | dano " << f.dano << " | XP minimo "
                 << f.xp_desbloqueio << " | ";
            if (f.desbloqueado) {
                cout << "liberado" << "\n";
            } else {
                cout << "bloqueado" << "\n";
            }
        }
        cout << "O professor oferece um descanso e alimentacao." << endl;
        alimentar(p);
        empilharAcao("Visitou o professor");
    } else {
        empilharAcao("Continuou a historia");
    }
}

// Remove a armadura equipada sem permitir uma derrota acidental no inventario.
bool retirarArmadura(jogador &p) {
    if (p.hp - bonusArmadura <= 0) {
        cout << "Recupere vida antes de remover esta armadura equipada." << endl;
        return false;
    }
    p.hp = min(100, p.hp - bonusArmadura);
    bonusArmadura = 0;
    armadura_equipada = -1;
    return true;
}

// Guardar armadura.
void guardarArmadura(jogador &p, int id, int nivel) {
    int niveisExigidos[5] = {1, 5, 4, 8, 6};
    int exigido = niveisExigidos[id];
    armadura premio = armadura_jogo[id];
    if (id == 4 && rand() % 2 == 0) {
        premio.defesa = -70;
    }
    if (nivel < exigido) {
        cout << "Armadura encontrada, mas nivel insuficiente." << endl;
        return;
    }
    for (int i = 0; i < 2; ++i) {
        if (!slot_ocupado[i]) {
            Ainventario[i] = premio;
            slot_ocupado[i] = true;
            cout << "Armadura recebida: " << Ainventario[i].nome << " | Bonus de vida: " << premio.defesa << endl;
            return;
        }
    }
    cout << "Premio encontrado: " << premio.nome << " | Bonus de vida: " << premio.defesa << endl;
    cout << "Inventario de armaduras cheio. 1 ou 2 para substituir, 0 descartar: ";
    int i = lerOpcao(0, 2);
    if (i) {
        if (armadura_equipada == i - 1 && !retirarArmadura(p)) {
            return;
        }
        Ainventario[i - 1] = premio;
        slot_ocupado[i - 1] = true;
    }
}

// Mostra pistas para encontrar a espada.
void pistaEspada(int chave) {
    if (chave < 61 || chave > 67 || campanha.espada) {
        return;
    }
    cout << "Tentativas usadas: " << campanha.tentativasEspada << "/3" << endl;
    if (campanha.tentativasEspada >= 3) {
        cout << "As runas da espada perderam o brilho." << endl;
        return;
    }
    if (chave < campanha.chaveEspada) {
        cout << "As runas indicam uma chave MAIOR." << endl;
    }
    if (chave > campanha.chaveEspada) {
        cout << "As runas indicam uma chave MENOR." << endl;
    }
}

// Entrega o premio uma unica vez.
void receberBau(NoBau *bau, int buscas) {
    bau->coletado = true;
    cout << "Encontrado em " << buscas << " comparacoes: " << bau->premio << endl;
    if (bau->premio == "Espada da Grifinoria") {
        if (buscas <= 3 && campanha.tentativasEspada <= 3) {
            campanha.espada = true;
            cout << "Espada conquistada! Bonus de dano nas proximas batalhas." << endl;
        } else {
            cout << "Limite de tentativas excedido." << endl;
        }
    } else if (bau->premio == "Pocao") {
        ++campanha.pocoesCura;
    }
    empilharAcao("Explorou bau " + to_string(bau->chave));
}

// Mostra o desafio e as pistas iniciais do bau.
void mostrarDesafioBau() {
    cout << "\n====================================" << endl;
    cout << "           BAU MISTERIOSO           " << endl;
    cout << "====================================" << endl;
    cout << "A espada esta entre 61 e 67. Encontre-a em 3 tentativas." << endl;
    cout << "Outros baus: 10, 25, 40, 50 e 75." << endl;
    cout << "Digite a chave do bau (0 para sair): ";
}

// Explorar bau com busca em arvore.
void explorarBau(jogador& p) {
    mostrarDesafioBau();
    int chave = lerOpcao(0, 999);
    int buscas = 0;
    campanha.tentouBau = chave != 0;
    if (chave == 0) {
        return;
    }
    if (chave >= 61 && chave <= 67 && !campanha.espada) {
        ++campanha.tentativasEspada;
    }
    NoBau* bau = buscarBau(raizBaus, chave, buscas);
    if (bau == nullptr) {
        cout << "Bau nao encontrado. Comparacoes: " << buscas << endl;
        pistaEspada(chave);
        return;
    }
    if (bau->coletado) {
        cout << "Bau ja explorado." << endl;
        return;
    }
    receberBau(bau, buscas);
    if (campanha.espada || campanha.tentativasEspada >= 3) {
        cout << "Chaves em ordem: ";
        imprimirBausOrdem(raizBaus);
        cout << endl;
    }
    (void)p;
}
// ==========================================================
// 11. MECANICA DE COMBATE
// ==========================================================
// Combate reutilizavel para as novas missoes e eventos.
// Indice personagem.
int indicePersonagem(const jogador& p) {
    for (int i = 0; i < 4; i++) {
        if (p.nomeP == personagens[i].nomeP) {
            return i;
        }
    }
    return 0;
}

// Identifica a primeira missao original de Luna ou Neville.
bool combateOriginal(const jogador& p) {
    (void)p;
    return false;
}

// Exibe as reacoes originais da primeira missao.
void reacaoOriginal(const jogador& p, int acao) {
    if (acao == 0 && indicePersonagem(p) == 0)
        cout << "\nTalvez eu fosse melhor nisso\ndo que imaginava." << endl;
    else if (acao == 0)
        cout << "\nOs encontros estavam surtindo efeito!\nA resistencia vai sobreviver e vai vencer" << endl;
    if (acao == 1) {
        cout << "Minha mira nao parecia muito boa.\nOu talvez ele estivesse se mexendo demais." << endl;
    }
    if (acao == 2) {
        cout << "\nProtego!\nUma barreira surgiu diante de mim.\nEra bom saber que algumas coisas\nainda funcionavam." << endl;
    }
    if (acao == 3) {
        cout << "\nParei por alguns instantes.\nEle parecia nervoso.\nPessoas nervosas costumam errar.\nVoce analisou o adversario!\nBonus de acerto no proximo feitico: " << p.hi / 4 << "%" << endl;
    }
    if (acao == 4) {
        cout << "\nConsegui desviar do feitico!\nPor pouco. Muito pouco." << endl;
    }
}

// Reacao combate.
void reacaoCombate(const jogador& p, int acao) {
    if (combateOriginal(p)) {
        reacaoOriginal(p, acao);
        return;
    }
    string falas[4][5] = {
        {"A magia encontrou seu caminho.",
            "Minha varinha parece distraida hoje.",
            "Uma pequena bolha de tranquilidade.",
            "Sempre ha algo que passa despercebido.",
            "Ainda estou aqui. Que bom."},
        {"Dessa vez eu consegui!",
            "Tudo bem. Posso tentar outra vez.",
            "Preciso permanecer de pe.",
            "O professor teria pedido calma.",
            "Nao vou abandonar meus amigos."},
        {"Ha! Funcionou sem explodir tudo!",
            "Ora, essa nao estava nos meus planos.",
            "Protego! Antes que algo voe por ai.",
            "Certo... pensar antes de agir.",
            "Essa passou perto demais!"},
        {"Um feitico preciso. Como no treino.",
            "Preciso recuperar a concentracao.",
            "A defesa tambem exige disciplina.",
            "Devo entender o que tenho diante de mim.",
            "Ainda posso cumprir minha promessa."}
    };
    cout << p.nomeP << ": " << falas[indicePersonagem(p)][acao] << endl;
}


// Menu combate.
int menuCombate(const jogador& p, const inimigo& oponente) {
    cout << "\n" << oponente.nome << " | HP " << oponente.vida << " | Sua vida " << p.hp << endl;
    cout << "Level: " << p.level << " | Alimentacao: " << campanha.alimentacao << endl;
    cout << "\n====================================" << endl;
    cout << "        O QUE DESEJA FAZER?         " << endl;
    cout << "====================================" << endl;
    cout << "1 - Lancar feitico" << endl;
    cout << "2 - Defender" << endl;
    cout << "3 - Observar o inimigo" << endl;
    cout << "4 - Usar item" << endl;
    cout << "Escolha: ";
    return lerOpcao(1, 4);
}

// Lista feiticos, dano e chance de acerto.
void listarFeiticosCombate(const jogador &p, int bonusAcerto) {
    cout << "\n====================================" << endl;
    cout << "       FEITICOS DISPONIVEIS         " << endl;
    cout << "====================================" << endl;
    for (int i = 0; i < 10; i++) {
        if (inventario_feitico[i].desbloqueado) {
            int chance = min(100, inventario_feitico[i].chance_acerto + p.hm / 10 + bonusAcerto);
            cout << i + 1 << "-" << inventario_feitico[i].name << " (" << inventario_feitico[i].dano << ")";
            cout << " | Acerto: " << chance << "%" << endl;
        }
    }
    if (campanha.espada) {
        cout << "Espada da Grifinoria: +15 de dano." << endl;
    }
    cout << "Dica: novos feiticos aparecem quando voce sobe de nivel." << endl;
    cout << "\nFeitico: ";
}

// Exibe o resultado do feitico no padrao dos duelos originais.
void mensagemAcerto(const jogador& p, const inimigo& oponente, const feiticos& f, int dano) {
    if (combateOriginal(p)) {
        cout << "\nO feitico acertou!" << endl;
        cout << "Dano causado: " << dano << endl;
    } else if (oponente.nome == "Voldemort") {
        cout << "\nVoce acertou " << f.name << "!" << endl;
        cout << "Dano causado: " << dano << endl;
    } else if (oponente.nome == "Ranrok") {
        cout << "\nVoce acertou!" << endl;
        cout << "Dano causado: " << dano << endl;
    } else {
        cout << "Voce acertou! Dano: " << dano << endl;
    }
}

// Apresenta a fala anterior ao feitico quando se usa a narrativa original.
void anunciarFeitico(jogador &p) {
    if (combateOriginal(p)) {
        cout << "\nEu ergui minha varinha." << endl;
        cout << "Nunca gostei muito de machucar pessoas." << endl;
        cout << "Mas algumas nao deixam muitas escolhas." << endl;
    }
}

// Aplica o feitico selecionado e atualiza o adversario.
bool lancarFeitico(jogador& p, inimigo& oponente, int& bonusAcerto) {
    listarFeiticosCombate(p, bonusAcerto);
    int id = lerOpcao(1, 10) - 1;
    if (!inventario_feitico[id].desbloqueado || p.xp < inventario_feitico[id].xp_desbloqueio) {
        cout << "Feitico indisponivel!" << endl;
        return false;
    }
    feiticos escolhido = inventario_feitico[id];
    int chanceFinal = min(100, escolhido.chance_acerto + p.hm / 10 + bonusAcerto);
    anunciarFeitico(p);
    cout << "\n" << escolhido.name << "!" << endl;
    if (rand() % 100 < chanceFinal) {
        int dano = escolhido.dano;
        if (campanha.espada) {
            dano += 15;
        }
        oponente.vida = max(0, oponente.vida - dano);
        mensagemAcerto(p, oponente, escolhido, dano);
        reacaoCombate(p, 0);
    } else {
        cout << (oponente.nome == "Voldemort" || oponente.nome == "Ranrok" ? "O feitico falhou!" : "O feitico errou!") << endl;
        reacaoCombate(p, 1);
    }
    bonusAcerto = 0;
    empilharAcao("Usou feitico " + escolhido.name);
    return true;
}

// Mostra a reacao original quando Luna ou Neville recebem um ataque.
void reacaoDanoOriginal(const jogador& p, bool protegendo) {
    if (protegendo) {
        cout << "\nMinha protecao funcionou." << endl;
        cout << "Nao perfeitamente." << endl;
        cout << "Mas funcionou." << endl;
    } else {
        cout << "\nIsso nao foi agradavel." << endl;
        if (indicePersonagem(p) == 0) {
            cout << "Definitivamente nao." << endl;
        }
        else {
            cout << "Comensal da morte ou bobo-da-corte?" << endl;
        }
    }
}

// Restaura as mensagens dos adversarios.
void mensagemDano(const jogador& p, const inimigo& oponente, int dano) {
    if (combateOriginal(p)) {
        cout << "\nO inimigo acertou!" << endl;
        cout << "Dano recebido: " << dano << endl;
        reacaoDanoOriginal(p, dano < oponente.dano);
    } else if (oponente.nome == "Voldemort" || oponente.nome == "Ranrok") {
        cout << "\n" << oponente.nome << " atacou!" << endl;
        cout << "Voce recebeu " << dano << " de dano." << endl;
    } else {
        cout << "O " << oponente.nome << " acertou o ataque!" << endl;
        cout << "Voce perdeu " << dano << " HP." << endl;
    }
    cout << "Vida atual: " << p.hp << endl;
}

// Calcula a resposta do adversario e atualiza a vida.
void contraAtaque(jogador& p, const inimigo& oponente, int danoRecebido) {
    if (combateOriginal(p)) {
        cout << "\nO comensal apontou sua varinha!" << endl;
    }
    if (rand() % 100 < oponente.chanceAtaque && rand() % 100 >= p.hf / 4) {
        p.hp = max(0, p.hp - danoRecebido);
        mensagemDano(p, oponente, danoRecebido);
    } else {
        cout << "O " << oponente.nome << " errou o ataque!" << endl;
        cout << "Voce nao recebeu dano!" << endl;
        reacaoCombate(p, 4);
    }
}

// ==========================================================
// 12. DIALOGOS DO ROTEIRO
// ==========================================================
// Dialogos adicionais do roteiro, por cena, personagem e momento.
// [cena][personagem][momento]: antes = 0, durante = 1, depois = 2.
string dialogos[12][4][3] = {
    {
        {"Gina: Você também recebeu a mensagem?\nLuna: Sim. Minha moeda parecia mais nervosa do que eu.",
            "Seguidor: Entregue o pergaminho!\nLuna: Ele não parece querer ir com você.",
            "Gina: O símbolo da chave está brilhando.\nLuna: Então Hogwarts ainda sabe nos encontrar."},
        {"Gina: Os alunos estão esperando sua resposta.\nNeville: Então vamos ouvi-los primeiro.",
            "Seguidor: Acha que pode substituir Potter?\nNeville: Não estou tentando ser ele.",
            "Gina: A chave fica com você.\nNeville: Certo. Mas me ajude a entender o caminho."},
        {"Gina: Não faça barulho.\nSimas: Finalmente uma missão feita sob medida para outra pessoa.",
            "Seguidor: Acabou a reunião!\nSimas: Engraçado, ainda nem serviram o chá.",
            "Gina: A chave aponta para a floresta.\nSimas: E eu que esperava uma indicação para a cozinha."},
        {"Gina: Precisamos de um plano.\nCedrico: Quero ouvir o que os outros descobriram.",
            "Seguidor: Diggory, ainda bancando o campeão?\nCedrico: Hoje ninguém está competindo.",
            "Gina: Guardaremos a sala. Leve a chave.\nCedrico: Confio em vocês. Voltarei com respostas."}
    },
    {
        {"Bandido: Tudo aqui tem preço.\nLuna: Até devolver algo que nunca foi seu?",
            "Bandido: Pare de fazer perguntas!\nLuna: Mas você ainda não respondeu à primeira.",
            "Estudante: Encontrou nossos pacotes!\nLuna: Eles estavam tentando voltar para vocês."},
        {"Bandido: Não é problema seu.\nNeville: Conheço quem precisa desses mantimentos.",
            "Bandido: Está tremendo.\nNeville: Isso não muda o que vim fazer.",
            "Estudante: Como podemos agradecer?\nNeville: Dividam as poções com quem precisar."},
        {"Bandido: Pague para passar.\nSimas: Aceita reclamações por escrito?",
            "Bandido: Você se acha engraçado?\nSimas: No momento, só quero a caixa.",
            "Estudante: E essa bússola?\nSimas: Pelo menos alguém aqui sabe para onde vamos."},
        {"Bandido: São meus agora.\nCedrico: Podemos encerrar isso se você os devolver.",
            "Bandido: Não recebo ordens suas.\nCedrico: Então não deixarei que leve os suprimentos.",
            "Estudante: Você recuperou tudo?\nCedrico: Confiram juntos. Não quero que falte nada."}
    },
    {
        {"Aliado: Estas raízes estão fechando a entrada.\nLuna: Talvez estejam apontando para ela.",
            "Luna: Guardião, a chave veio do castelo.\nGuardião: A memória não pertence a quem exige.",
            "Dumbledore: A segunda parte está em Gringotes.\nLuna: Espero que o próximo guardião goste de conversar."},
        {"Aliado: Devemos afastar as plantas?\nNeville: Espere. Elas estão protegendo a inscrição.",
            "Guardião: Prove que merece a memória.\nNeville: Não vim provar nada. Vim protegê-la.",
            "Dumbledore: Procure o contato indicado pela chave.\nNeville: Vou compartilhar a mensagem com Gina."},
        {"Aliado: A chave mudou de cor.\nSimas: Ao menos uma coisa resolveu funcionar.",
            "Guardião: Retire-se.\nSimas: Podíamos ter começado com essa placa na entrada.",
            "Dumbledore: O próximo fragmento está em Gringotes.\nSimas: Ótimo. Desta vez espero um chão que não se mexa."},
        {"Aliado: Posso ajudar na busca?\nCedrico: Sim, confira a inscrição comigo.",
            "Guardião: Apenas os dignos atravessam.\nCedrico: Não peço prêmio algum. Precisamos da memória.",
            "Dumbledore: A magia nunca deve pertencer a um só.\nCedrico: Então levarei essa descoberta aos outros."}
    },
    {
        {"Aliado: O trasgo está bloqueando a trilha.\nLuna: Talvez ele também esteja procurando a saída.",
            "Aliado: Ele não entende nossas palavras!\nLuna: Então precisamos deixar que o caminho fale.",
            "Aliado: A criatura voltou às árvores.\nLuna: Espero que encontre um lugar tranquilo."},
        {"Aliado: Não podemos atravessar.\nNeville: Vou tentar abrir espaço sem assustá-lo ainda mais.",
            "Aliado: A névoa o confunde.\nNeville: Continuem juntos. A magia vai passar.",
            "Aliado: A trilha está livre.\nNeville: Vamos embora antes que ele se assuste outra vez."},
        {"Aliado: Um trasgo!\nSimas: Eu estava começando a sentir falta dos problemas pequenos.",
            "Aliado: Cuidado com a clareira!\nSimas: Estou tentando fazer uma visita breve.",
            "Aliado: Podemos seguir?\nSimas: Sim, e ninguém diga que eu fiz amizade com ele."},
        {"Aliado: Podemos usar outra trilha.\nCedrico: Se ajudarmos, a passagem ficará livre para os demais.",
            "Aliado: A criatura está assustada.\nCedrico: Vamos manter distância e proteger uns aos outros.",
            "Aliado: Acabou.\nCedrico: Confiram se todos estão bem antes de sair."}
    },
    {
        {"Duende: O cofre só responde à chave.\nLuna: Algumas portas são bastante seletivas com as amizades.",
            "Armadura: O poder deve permanecer oculto.\nLuna: Mesmo quando escondê-lo coloca a escola em perigo?",
            "Duende: A inscrição menciona Ranrok.\nLuna: Uma história esquecida ainda pode ser ouvida."},
        {"Duende: Você reconhece a fotografia?\nNeville: São meus pais. Não sabia que havia uma cópia aqui.",
            "Armadura: Você não possui autoridade.\nNeville: Possuo a chave e uma responsabilidade.",
            "Duende: O terceiro fragmento está sob Hogwarts.\nNeville: Obrigado. Agora sei para onde voltar."},
        {"Duende: Não toque nos objetos.\nSimas: Nem nos que parecem prestes a cair?",
            "Armadura: Invasor detectado.\nSimas: Até as armaduras têm problemas de atendimento.",
            "Duende: Ranrok deixou uma história complicada.\nSimas: Desta vez vou ouvir antes de fazer piada."},
        {"Duende: O ouro não é o que viemos buscar.\nCedrico: Então estamos de acordo.",
            "Armadura: O fragmento não é seu.\nCedrico: Não pretendo ficar com ele.",
            "Duende: Ranrok disputava o acesso à magia.\nCedrico: Precisamos entender essa história antes de decidir."}
    },
    {
        {"Hagrid: Bicuço não consegue sair.\nLuna: Vou cumprimentá-lo para que saiba que estamos aqui.",
            "Pettigrew: A lealdade não salva ninguém.\nLuna: Talvez você nunca tenha entendido o que ela significa.",
            "Hagrid: Ele está livre!\nLuna: Acho que gostaria de um pouco de sossego agora."},
        {"Hagrid: Ainda bem que veio, Neville.\nNeville: Onde está Bicuço?",
            "Pettigrew: Seus amigos vão abandoná-lo.\nNeville: Não cabe a você decidir isso.",
            "Hagrid: Obrigado por ajudar.\nNeville: Cuide dele. Preciso voltar ao castelo."},
        {"Hagrid: É uma espécie de fantasma.\nSimas: Fantástico. Até as lembranças resolveram dar trabalho.",
            "Pettigrew: Não zombe de mim!\nSimas: Então deixe Bicuço ir.",
            "Hagrid: Ele gostou de você.\nSimas: Finalmente alguém aprovou minha companhia."},
        {"Hagrid: Preciso de ajuda.\nCedrico: Vamos libertá-lo juntos.",
            "Pettigrew: Eu não tinha escolha.\nCedrico: Sempre podemos escolher não abandonar alguém.",
            "Hagrid: Volte para os seus amigos.\nCedrico: E você fique com Bicuço. Contamos com vocês."}
    },
    {
        {"Gina: Os fragmentos estão reagindo.\nLuna: Parece que finalmente se reconheceram.",
            "Sentinela: A passagem está proibida.\nLuna: Essa ordem é sua ou de alguém que está usando você?",
            "Gina: Encontrou o último?\nLuna: Sim. Agora o castelo pode voltar a escolher."},
        {"Gina: Os alunos menores estão seguros.\nNeville: Então podemos nos concentrar na passagem.",
            "Sentinela: Não avançará.\nNeville: Não podemos abandonar o castelo agora.",
            "Gina: Você conseguiu!\nNeville: Conseguimos. Preciso que continue protegendo os alunos."},
        {"Gina: Siga o que a chave indicar.\nSimas: Essa é a parte em que você me pede para não improvisar.",
            "Sentinela: A proteção foi alterada.\nSimas: Então vamos devolvê-la ao normal.",
            "Gina: Nada desabou?\nSimas: Nem uma parede. Pode registrar isso no meu histórico."},
        {"Gina: Deixe que cuidemos da superfície.\nCedrico: Está bem. Avise se precisar de mim.",
            "Sentinela: O acesso pertence ao novo mestre.\nCedrico: Hogwarts não pertence a um mestre.",
            "Gina: O terceiro fragmento está seguro?\nCedrico: Está. E vocês fizeram isso possível."}
    },
    {
        {"Professor: Pense em algo que lhe traga alegria.\nLuna: As tardes em que conversava com meus amigos.",
            "Professor: Você consegue manter essa lembrança?\nLuna: Sim. Ela é mais forte que este silêncio.",
            "Professor: O frio passou.\nLuna: Ainda existem coisas bonitas por aqui."},
        {"Professor: Não precisa fingir que não sente medo.\nNeville: Vou lembrar de quando meus amigos me esperaram.",
            "Professor: Continue concentrado nessa lembrança.\nNeville: Eles acreditam em mim.",
            "Professor: A passagem está livre.\nNeville: Obrigado. Estou pronto para seguir."},
        {"Professor: Lembre-se de um momento feliz.\nSimas: Quando conseguimos rir mesmo numa semana horrível.",
            "Professor: Concentre-se nessa alegria.\nSimas: Ainda teremos outras tardes assim.",
            "Professor: Você conseguiu.\nSimas: Sem nenhuma piada. Isso deve valer pontos extras."},
        {"Professor: Uma lembrança positiva, Cedrico.\nCedrico: As pessoas que me acolheram quando voltei do torneio.",
            "Professor: Mantenha essa lembrança por perto.\nCedrico: Não preciso enfrentar tudo sozinho.",
            "Professor: Já pode continuar.\nCedrico: Obrigado por permanecer aqui comigo."}
    },
    {
        {"Gina: Precisamos de ajuda na entrada.\nLuna: Hogwarts escolheu uma noite bastante movimentada.",
            "Comensal: Afastem-se da câmara.\nLuna: Acho que o castelo não o convidou.",
            "Gina: A passagem está livre.\nLuna: Volte com os outros. Vou cuidar dos fragmentos."},
        {"Gina: Estávamos esperando você.\nNeville: Vamos terminar isso juntos.",
            "Comensal: A resistência acabou.\nNeville: Olhe ao redor. Ainda estamos aqui.",
            "Gina: Pode entrar.\nNeville: Obrigado por manter a passagem protegida."},
        {"Gina: Finalmente!\nSimas: Reclamações sobre atrasos serão aceitas depois da guerra.",
            "Comensal: Vocês perderam.\nSimas: Curioso, ninguém nos avisou.",
            "Gina: Qual é o próximo passo?\nSimas: Restaurar o selo. Sem aumentar a lista de reparos."},
        {"Gina: Precisamos manter a entrada.\nCedrico: Diga onde posso ajudar.",
            "Comensal: Não podem defender todos.\nCedrico: É por isso que estamos juntos.",
            "Gina: A câmara está à sua frente.\nCedrico: Confio a passagem a vocês."}
    },
    {
        {"Voldemort: O castelo responderá apenas a mim.\nLuna: Não acho que ele goste de receber ordens.",
            "Voldemort: Você não compreende este poder.\nLuna: Compreendo que ele não foi feito para uma pessoa.",
            "Luna: Os fragmentos voltaram ao seu lugar.\nGina: Então ele perdeu o controle da fonte?\nLuna: Sim. Hogwarts voltou a respirar."},
        {"Voldemort: Longbottom, quem lhe deu autoridade?\nNeville: Ninguém. Escolhi estar aqui.",
            "Voldemort: Seus esforços são inúteis.\nNeville: Cada fragmento diz o contrário.",
            "Neville: O vínculo se desfez.\nGina: Você voltou!\nNeville: Agora precisamos continuar juntos."},
        {"Voldemort: Dumbledore desperdiçou este poder.\nSimas: Talvez ele só soubesse a diferença entre uma escola e um trono.",
            "Voldemort: Silêncio!\nSimas: Pela primeira vez, concordamos. Preciso me concentrar no selo.",
            "Simas: Está selado!\nGina: E o castelo?\nSimas: Inteiro. Pode conferir."},
        {"Voldemort: Você deveria ter desaparecido naquele torneio.\nCedrico: O futuro não é seu para decidir.",
            "Voldemort: Quer vingança?\nCedrico: Quero que os estudantes tenham amanhã.",
            "Cedrico: Ele perdeu a ligação com a fonte.\nGina: Você conseguiu voltar.\nCedrico: Graças ao trabalho de todos."}
    },
    {
        {"Ranrok: Ninguém se lembra do que nos foi negado.\nLuna: Então conte. Mas não machuque Hogwarts para ser ouvido.",
            "Ranrok: A magia será livre!\nLuna: Livre não significa obedecer apenas a você.",
            "Luna: A voz desapareceu.\nGina: E o selo?\nLuna: Está inteiro. A história dele não precisa ser esquecida."},
        {"Ranrok: Os bruxos sempre nos diminuíram.\nNeville: Conheço a sensação de ser considerado incapaz.",
            "Ranrok: Então fique do meu lado!\nNeville: Posso reconhecer a injustiça sem aceitar sua escolha.",
            "Neville: A fonte está protegida.\nGina: Você conseguiu.\nNeville: Ainda temos muito a aprender."},
        {"Ranrok: Séculos de silêncio terminarão hoje.\nSimas: Eu preferia uma conversa, mas imagino que você não aceite.",
            "Ranrok: Não me interrompa!\nSimas: Tudo bem. Vou prestar atenção nos fragmentos.",
            "Simas: O último cristal se apagou.\nGina: E Ranrok?\nSimas: O eco sumiu. A história ficou."},
        {"Ranrok: Quem decide quem merece magia?\nCedrico: Não deveria ser uma única pessoa.",
            "Ranrok: Então deixe a fonte aberta!\nCedrico: Não quando isso ameaça todos os estudantes.",
            "Cedrico: O selo foi restaurado.\nGina: Acabou?\nCedrico: O perigo imediato, sim. A questão dele merece ser lembrada."}
    },
    {
        {"Gina: Você está ouvindo alguma coisa?\nLuna: Risadas. Fazia tempo.",
            "Gina: A guerra ainda não acabou.\nLuna: Eu sei. Mas podemos guardar este instante.",
            "Gina: Vamos voltar?\nLuna: Vamos. Talvez os quadros tenham novidades."},
        {"Gina: Você não precisa resolver tudo hoje.\nNeville: Estou começando a entender isso.",
            "Estudante: Obrigado por ter ficado.\nNeville: Obrigado por terem ficado comigo.",
            "Gina: Amanhã seguimos?\nNeville: Juntos."},
        {"Gina: Você vai contar essa história para todo mundo?\nSimas: Só a parte em que fui extremamente responsável.",
            "Gina: Então vai ser curta.\nSimas: Injusto, mas provavelmente correto.",
            "Gina: Vamos descansar.\nSimas: A melhor ordem que recebi a noite inteira."},
        {"Gina: O que pretende fazer agora?\nCedrico: Ajudar quem ainda precisa.",
            "Gina: E descansar?\nCedrico: Também. Prometo não esquecer dessa parte.",
            "Gina: Temos um amanhã.\nCedrico: Então vamos fazer valer a pena."}
    },
};

// Mostra somente as falas do personagem escolhido.
void mostrarDialogo(const jogador &p, int cena, int momento) {
    if (cena < 0) {
        return;
    }
    cout << endl;
    cout << dialogos[cena][indicePersonagem(p)][momento] << endl;
    atrasoNarrativo();
}

// Processa defesa ou observacao.
void prepararTurno(jogador &p, int escolha, int &danoRecebido, int &bonusAcerto) {
    if (escolha == 2) {
        danoRecebido /= 2;
        empilharAcao("Defendeu ataque");
        reacaoCombate(p, 2);
    }
    if (escolha == 3) {
        bonusAcerto = p.hi / 4;
        reacaoCombate(p, 3);
        if (!combateOriginal(p)) {
            cout << "Bonus de acerto no proximo feitico: " << bonusAcerto << "%" << endl;
        }
    }
}

// Executa turnos do duelo ate a vitoria ou derrota.
bool combate(jogador& p, inimigo oponente, int cena = -1) {
    bool falou = false;
    p.hp = min(p.hp, vidaMaxima());
    int bonusAcerto = 0;
    while (p.hp > 0 && oponente.vida > 0 ) {
        int escolha = menuCombate(p, oponente);
        limparTela();
        int danoRecebido = oponente.dano;
        if (escolha == 4) {
            consumirItem(p);
            continue;
        }
        prepararTurno(p, escolha, danoRecebido, bonusAcerto);
        if (escolha == 1 && !lancarFeitico(p, oponente, bonusAcerto)) {
            continue;
        }
        if (!falou) {
            mostrarDialogo(p, cena, 1);
            falou = true;
        }
        if (oponente.vida > 0) {
            contraAtaque(p, oponente, danoRecebido);
        }
    }
    return p.hp > 0 && oponente.vida == 0;
}

// Verificar fim polissuco.
void verificarFimPolissuco(jogador& p) {
    if (campanha.polissucoAteLevel && p.level >= campanha.polissucoAteLevel) {
        p.hf -= 5;
        p.hm -= 5;
        p.hi -= 5;
        campanha.polissucoAteLevel = 0;
        cout << "O efeito do Polissuco acabou." << endl;
    }
}

// Concluir missao.
void concluirMissao(jogador& p, int id, int xp) {
    campanha.missaoConcluida = id;
    cout << "\n====================================" << endl;
    cout << "MISSAO " << id << " CONCLUIDA!" << endl;
    cout << "====================================" << endl;
    empilharAcao("Concluiu missao " + to_string(id));
    adicionar_xp(p, xp);
    verificarFimPolissuco(p);
    sincronizarListaFeiticos();
    mostrarResumo(p);
}

struct Narrativa { string abertura; string encerramento; };

Narrativa cenas[11][4] = {
    {
        {"Eu sempre gostava de caminhar durante a madrugada. Os quadros descansavam e Hogwarts parecia respirar. Naquela noite... não. Os duendes felizes haviam sumido. Gina abriu o pergaminho. — O castelo parece triste, Gina. — É justamente sobre isso que precisamos conversar. Notei que um símbolo pulsava quando aproximava minha moeda. Quando a porta se abriu, pedi que Gina guardasse o papel. Precisávamos proteger a reunião.",
            "Depois, a luz do símbolo voltou. Talvez o castelo tivesse gostado de ser ouvido."},
        {"Eu nunca fui bom em dar ordens. Agora todos me olhavam. — Neville, precisamos decidir — disse Gina. Vi alunos assustados perto da saída e pedi que ficassem juntos. Minha voz falhou na primeira tentativa; na segunda, consegui falar. Precisávamos impedir que a reunião fosse desfeita.",
            "Quando o último se afastou, Gina me entregou a chave de acesso. — Não foi tão ruim, foi? — Foi horrível. Mas conseguimos."},
        {"Hogwarts conseguiu ficar ainda pior. Nem explodir caldeirões por acidente parecia permitido. — Trouxe sua varinha? — perguntou Gina. — Quando foi a última vez que alguma coisa deu certo quando você perguntou isso? O pergaminho começou a brilhar; pensei que fosse mais uma das minhas experiências. Quando os Carrow apareceram, pensei em como afastá-los sem danificar a sala.",
            "— Viu? Dessa vez nada quebrou. Gina apontou para uma cadeira caída. — Quase nada."},
        {"Desde o Torneio Tribruxo, aprendi que sobreviver nem sempre significa vencer. — Precisamos da sua ajuda — disse Gina. Pedi que ela explicasse o símbolo aos demais enquanto eu mantinha a passagem protegida. Os bruxos chegaram e, por um instante, lembrei-me do torneio. Desta vez não estava sozinho.",
            "Aceitei a ajuda dos colegas e vencemos. — Você não precisa fazer tudo — Gina lembrou. Guardei aquela frase junto com a chave."}
    },
    {
        {"— Você deve ser muito solitário para precisar de tantas coisas que não lhe pertencem. Ele riu. Percebi que a bússola apontava para o chão, não para o norte.",
            "Depois de afastá-lo com magia, devolvi os pacotes aos estudantes. — Objetos perdidos gostam de encontrar o caminho de casa."},
        {"Reconheci a caixa de mantimentos que Gina havia preparado. — Isso pertence aos alunos. Ele tentou me intimidar, mas eu mantive minha posição.",
            "Após o duelo, conferi os nomes nas etiquetas. Nenhum estudante ficaria sem sua parte por minha causa."},
        {"— Cobrar pedágio numa passagem secreta? Que modelo de negócio horrível. Ele ergueu a varinha. Precisava recuperar os pacotes.",
            "A bússola começou a girar. — Ótimo, até o nosso equipamento tem senso de humor."},
        {"— Ainda podemos resolver isso sem uma batalha. Ofereci que devolvesse os suprimentos. Ele recusou.",
            "Após o confronto, deixei que se retirasse e entreguei a bússola à resistência. Não havia motivo para prolongar o conflito."}
    },
    {
        {"A floresta estava sussurrando. Não com palavras, obviamente. As árvores raramente utilizavam palavras. — Vocês estão tentando me mostrar alguma coisa? A luz da chave acompanhou as raízes até a inscrição. O guardião se moveu quando entrei.",
            "Depois de neutralizar sua magia, coloquei a mão sobre o fragmento. A voz de Dumbledore surgiu. — Ah... então era você que estava tentando conversar."},
        {"Reconheci as plantas que a professora Sprout havia mostrado nas aulas. Elas protegiam a construção, não a destruíam. Afastei as folhas com cuidado e encontrei a inscrição. O guardião despertou. Respirei fundo. Precisava manter a calma.",
            "Quando a memória de Dumbledore apareceu, pensei que talvez minhas aulas tivessem servido para muito mais do que uma prova."},
        {"— Uma floresta escura, uma ruína e uma chave que brilha. Nada suspeito. A chave indicou uma pedra que não combinava com o restante. Ao tocá-la, o guardião acordou. — Evidentemente.",
            "Depois do duelo, a voz de Dumbledore surgiu. — Professor, francamente, uma carta teria sido mais prática."},
        {"As marcas lembravam os desafios do Torneio Tribruxo. Desta vez não havia uma taça esperando por mim. Examinei a inscrição e pedi que os companheiros aguardassem fora da ruína.",
            "Enfrentei o guardião e recuperei o fragmento. A memória de Dumbledore falou sobre poderes que não deveriam pertencer a uma só pessoa. Compreendi que proteger algo também significava saber compartilhá-lo."}
    },
    {
        {"— Acho que ele está perdido. O trasgo respondeu com um grunhido. Tentei acalmá-lo, mas a magia da clareira o mantinha inquieto. Precisava encontrar uma maneira de afastá-lo.",
            "Quando a trilha ficou livre, deixei que retornasse à floresta."},
        {"Vi marcas recentes entre as árvores e percebi que a criatura havia saído de seu caminho habitual. — Vamos deixar espaço para ele passar.",
            "A passagem estava livre. Não precisava transformar um animal desorientado em inimigo permanente."},
        {"— Por que nunca encontramos uma criatura pequena e educada? O trasgo avançou.",
            "Quando ele recuou, respirei aliviado."},
        {"A criatura estava tão assustada quanto qualquer um de nós. Precisava proteger a passagem.",
            "Quando tudo terminou, conferi se os companheiros estavam bem antes de procurar qualquer recompensa."}
    },
    {
        {"— Você acredita que objetos sentem saudades? O duende respondeu que objetos possuem valor, não sentimentos. Examinei o baú; seus números mudavam como se brincassem comigo. Mais adiante, uma armadura guardava o fragmento.",
            "Depois de desfazer seu encantamento, encontrei o fragmento. Ele guardava uma voz diferente da de Dumbledore. Uma voz que havia sido esquecida."},
        {"Encontrei uma fotografia antiga de Hogwarts. Meus pais estavam nela. Por um momento, esqueci o motivo da visita. — Vocês ficariam orgulhosos? O contato me chamou de volta.",
            "Recuperei o fragmento. Guardei a lembrança da fotografia; não precisava ser outra pessoa para continuar."},
        {"— Não toque em nada — disse o duende. — Sou extremamente responsável. Um objeto caiu. — Isso não fui eu. O baú apresentou uma sequência de números; finalmente um problema que não dependia de sorte. A armadura parecia prestes a se mexer. — Claro, o mobiliário também tem opinião.",
            "Ao recuperar o fragmento, ouvi um nome estranho: Ranrok. Pela primeira vez, não tive uma piada pronta."},
        {"Vi fortunas esquecidas enquanto estudantes dividiam comida em Hogwarts. O duende explicou que o cofre guardava mais do que ouro.",
            "Enfrentei a armadura que protegia o fragmento. A inscrição revelou Ranrok e a disputa pela magia antiga. Entendi que aquele conflito não começara com Voldemort."}
    },
    {
        {"Bicuço inclinou a cabeça. Retribuí o gesto. Pettigrew apareceu como uma lembrança que não sabia que era lembrança. — Vocês ainda acreditam na lealdade? — Você deve ter confundido lealdade com medo.",
            "Após dispersar o eco, o encantamento que prendia Bicuço pôde ser desfeito. Ele voltou para Hagrid."},
        {"Pensei em todos os que haviam confiado em Pettigrew. — Algumas pessoas ainda sabem escolher os próprios amigos. A voz dele tentou me fazer duvidar, mas continuei.",
            "Depois do confronto, Bicuço se aproximou. Acompanhei sua volta à cabana."},
        {"— Você fala demais para alguém que passou anos escondido. Pettigrew pareceu ofendido, o que era impressionante para uma memória.",
            "Usei magia para dispersá-lo. Bicuço me encarou. — Tudo bem, eu também não gostei dele."},
        {"— Sempre existe uma escolha. Você fez a sua. O eco tentou justificar suas decisões. Não aceitei a desculpa, mas tampouco fiquei discutindo.",
            "Quando a magia se desfez, conduzi Bicuço de volta a Hagrid. Dessa vez, ninguém precisou enfrentar o problema sozinho."}
    },
    {
        {"Hogwarts parecia respirar com dificuldade. Toquei a pedra da passagem e os fragmentos brilharam juntos. — Não precisamos ter medo um do outro — murmurei ao sentinela. Sua magia não respondeu. Procurei a luz original que ainda existia nele.",
            "Quando o encantamento se desfez, encontrei o terceiro fragmento. — Pronto. Agora podemos ouvir o que o castelo tem a dizer."},
        {"Gina me perguntou se eu tinha certeza. — Não. Mas sei que precisamos continuar. Vi os alunos menores seguindo os professores e senti que havia algo concreto a proteger. A chave abriu a passagem.",
            "O sentinela despertou e eu mantive minha posição até recuperar o fragmento. Quando voltei a ouvir a voz de Gina, percebi que já não estava esperando Harry decidir por mim."},
        {"— Passagens subterrâneas, guardiões e magia antiga. Hogwarts deveria oferecer créditos extras por isso. Os fragmentos reagiram de uma vez. Tive vontade de improvisar, mas lembrei do que acontecera na floresta. Desta vez, precisava ter cuidado.",
            "O sentinela parou. — Gina, você não vai acreditar: conseguimos manter a passagem. — Realmente não vou."},
        {"Quis pedir que todos aguardassem na superfície, mas Gina me interrompeu. — Você não precisa fazer tudo. Desta vez escutei. Enquanto os colegas protegiam a retirada, abri a passagem.",
            "Recuperei o fragmento e compartilhei a descoberta com o grupo. Não era minha missão particular. Era nossa."}
    },
    {
        {"Lembrei-me de minha mãe. Depois, das conversas com meus amigos, de tardes em que o mundo parecia gentil.",
            "Quando o frio passou, pensei: as coisas que amamos não precisam estar diante dos olhos para continuarem existindo."},
        {"Pensei em meus pais e na estufa da professora Sprout. Também me lembrei dos amigos me esperando na Sala Precisa.",
            "— Eu ainda estou aqui — disse, quando a passagem ficou livre."},
        {"O frio me fez pensar em perder meus amigos. Lembrei-me da última vez que todos rimos juntos. Não havia piada capaz de resolver tudo.",
            "Quando o dementador se afastou, decidi que ainda teríamos outras tardes assim."},
        {"O torneio voltou à minha memória. Pensei em quem me acolheu depois dele e nas pessoas que me permitiram recomeçar.",
            "Quando a magia do dementador cessou, compreendi que meu futuro não precisava ser uma dívida."}
    },
    {
        {"— Você demorou! — Gina disse. — O castelo tem muitas coisas para contar. Vi que os colegas estavam cansados e ofereci ajuda.",
            "Depois de afastarmos os Comensais, pedi que voltassem à superfície. Era hora de devolver ao castelo sua tranquilidade."},
        {"Gina estava esperando. — Precisamos de você. Desta vez não hesitei. Era preciso organizar a retirada dos alunos.",
            "Após a vitória, agradeci a todos. Não era necessário que uma única pessoa fosse responsável por tudo."},
        {"— Você demorou! — gritou Gina. — Tecnicamente, ainda cheguei antes do fim. Gina precisava de ajuda para proteger a passagem.",
            "Quando os Comensais recuaram, ela me perguntou se eu tinha um plano. — Tenho. E a melhor parte é que não envolve explodir o castelo."},
        {"Encontrei os colegas mantendo a entrada protegida. Em vez de mandar que todos saíssem imediatamente, perguntei como poderia ajudar.",
            "Juntos afastamos os Comensais. — Agora é a sua vez — disse Gina. Aceitei a confiança deles."}
    },
    {
        {"— Você deve estar terrivelmente cansado. — Cansado? — De viver fugindo de coisas que não consegue compreender. Enquanto a magia antiga tremia, concentrei-me nas luzes dos fragmentos.",
            "Quando o vínculo se rompeu, o silêncio deixou de parecer assustador. Hogwarts estava livre para respirar."},
        {"— Você não pode controlar tudo. — E quem me impedirá? Você? — Talvez. Mas não estou sozinho. Pensei na Armada e nos professores.",
            "Mantive minha magia até que o selo voltasse a funcionar. Quando Voldemort se retirou, não senti que havia me tornado Harry. Senti que finalmente era Neville."},
        {"— Tanto trabalho para invadir uma escola. Você realmente não gostava das aulas, não é? A magia dele fez a câmara tremer. Ignorei a vontade de improvisar e segui as indicações da chave.",
            "Quando o vínculo cedeu, observei as paredes. — Inteiras. Gina vai ficar surpresa."},
        {"— Você já tentou tirar meu futuro uma vez. Não permitirei que faça o mesmo com todos eles. Não procurei uma revanche; concentrei-me em restaurar o selo.",
            "Quando Voldemort deixou a câmara, compreendi que havia protegido o futuro de outras pessoas, não apenas o meu."}
    },
    {
        {"— Ser esquecido deve ser horrível. Mas ser temido não é o mesmo que ser lembrado com carinho. Escutei sua história sem aceitar sua escolha.",
            "Quando o selo foi restaurado, o eco desapareceu como uma voz que finalmente terminara sua frase."},
        {"— Sei como é ser tratado como insignificante. Mas não precisamos nos tornar iguais a quem nos diminuiu. A magia antiga pulsava diante dos fragmentos.",
            "Mantive a concentração até a câmara se acalmar. O poder não era o que me tornava importante."},
        {"— Você desapareceu há mais de cem anos. Não podia ter escolhido uma semana mais tranquila? O eco não achou graça. Usei a chave para reorganizar a magia, sem tentar controlá-la sozinho.",
            "Quando tudo se acalmou, admiti: — Talvez algumas coisas precisem mais de paciência do que de talento."},
        {"— Compreender sua revolta não significa aceitar suas escolhas. A memória de Ranrok refletia uma injustiça que não podia ser esquecida.",
            "Restaurei o selo sem negar essa história. Ao sair, decidi que proteger Hogwarts também significava questionar seus erros."}
    }
};

string titulos[] = {
    "O castelo que esqueceu de sorrir",
    "O preço da passagem",
    "Aquilo que a floresta guarda",
    "O gigante perdido",
    "As memórias que o ouro não compra",
    "As asas da liberdade",
    "A última noite de Hogwarts",
    "As coisas que o medo nos tira",
    "Aqueles que permaneceram",
    "O fim dos que temem a morte",
    "A magia dos esquecidos"
};
string locais[] = {
    "Hogwarts",
    "Passagem secreta",
    "Floresta Proibida",
    "Clareira",
    "Gringotes",
    "Cabana de Hagrid",
    "Subsolo",
    "Corredores",
    "Entrada da camara",
    "Coracao de Hogwarts"
};
string contextos[] = {
    "Uma moeda encantada transmite: “O castelo está mudando. Encontrem-me na Sala Precisa. Não confiem nos retratos.” Gina apresenta o pergaminho de Dumbledore, mas os seguidores dos Carrow interrompem a reunião.",
    "Um bruxo intercepta suprimentos da resistência e exige pagamento. Entre os objetos há uma bússola e poções.",
    "A chave reage a uma construção coberta de raízes. A inscrição diz que a floresta entrega sua memória a quem sabe escutar. Um guardião de pedra bloqueia o Fragmento da Memória.",
    "A magia da floresta desorienta um trasgo, que bloqueia a trilha. Há uma rota secundária disponível para quem recusar.",
    "O contato da resistência abre um cofre antigo mediante a chave. No interior estão artefatos de antigos diretores, um baú mágico e o Fragmento do Poder, protegido por uma armadura encantada.",
    "Hagrid pede ajuda com Bicuço. Um eco de Pedro Pettigrew, formado pelas memórias do Coração, mantém a criatura presa por magia. Hagrid e outros membros da resistência também estão tentando ajudá-la.",
    "Professores reforçam as proteções; estudantes mais jovens são encaminhados à segurança. A chave e os dois fragmentos revelam uma passagem. O sentinela que guarda o Fragmento da Proteção está corrompido pela instabilidade.",
    "O frio de um dementador toma a passagem. Os professores mantêm uma rota alternativa protegida.",
    "Comensais descobrem a entrada. Gina e a Armada mantêm a passagem protegida. Uma segunda rota foi aberta pelos colegas.",
    "Voldemort tenta usar a fonte para controlar as proteções do castelo. O objetivo não é derrotá-lo definitivamente, mas romper sua ligação com o Coração. — Dumbledore desperdiçou o verdadeiro potencial deste lugar.",
    "Ranrok manifesta-se como uma memória da magia antiga. Deseja libertar toda a energia, independentemente das consequências para a escola. — Durante séculos, os bruxos decidiram quem merecia possuir magia."
};
string transicoes[] = {
    "A chave indica a Floresta Proibida. Gina aponta a rota para a floresta. Uma bússola seria útil, mas já sabemos para onde ir.",
    "Gina: A floresta fica adiante. Sigam as indicações da chave.",
    "A memória indica um cofre antigo em Gringotes e revela o nome do contato autorizado a abrir a passagem.",
    "O grupo segue ao encontro do contato que dará acesso ao cofre de Gringotes.",
    "A inscrição revela o terceiro fragmento no subsolo de Hogwarts. O contato permite a viagem de volta; uma mensagem de Hagrid chega ao grupo.",
    "Hagrid indica a passagem para o castelo. A chave aponta para o subsolo.",
    "A entrada da câmara finalmente se abre. Ainda precisamos atravessar os corredores e alcançar sua porta.",
    "A entrada do Coração de Hogwarts está próxima.",
    "Gina: Podemos voltar pelo mapa caso ainda falte alguma coisa. Estarei aqui quando estiver pronto.",
    "Voldemort se retira; o Coração fica selado e a ameaça de Ranrok permanece adormecida. A guerra externa continua.",
    "O eco de Ranrok desaparece. Voldemort perde a oportunidade de usar o Coração, mas continua sendo uma ameaça externa."
};
string epilogos[] = {
    "Hogwarts ainda estava triste. Mas as árvores voltaram a sussurrar, os quadros conversaram e, por alguns segundos, ouvi risadas. Talvez os duendes felizes estivessem retornando. Ou talvez... nunca tivessem ido embora.",
    "Durante muito tempo, acreditei que coragem fosse não sentir medo. Estava errado. Eu ainda sentia medo. Mas aquelas pessoas não esperavam que eu fosse Harry. Esperavam que eu fosse Neville. E isso era suficiente.",
    "O castelo continuava inteiro. A maior parte dele, pelo menos. Gina perguntou se eu havia causado algum problema. — Nenhum que possa ser comprovado. Pela primeira vez em muito tempo, todos riram.",
    "O futuro nunca havia me parecido tão incerto. Nem tão importante. Não precisava provar que merecia uma segunda oportunidade. Precisava apenas fazer alguma coisa boa com ela."
};
string cenaComum = "Gina comunica à Armada que a fonte está protegida. Os estudantes retornam às tarefas de resistência. Ao longe, chega uma mensagem de que a guerra se aproxima de Hogwarts. Por enquanto, o Coração estava seguro.";

// Exibe um trecho do roteiro sem mudar suas palavras.
void mostrarNarrativa(const string& texto) {
    cout << endl;
    for (size_t i = 0; i < texto.size(); i++) {
        if (i > 0 && texto.compare(i, 4, " —") == 0) {
            cout << endl;
            i++;
        }
        cout << texto[i];
        if (texto[i] == '.' && i + 1 < texto.size() && texto[i + 1] == ' ' &&
            (i == 0 || texto[i - 1] != '.')) {
            cout << endl;
            i++;
            atrasoNarrativo();
        }
    }
    cout << endl;
}

// Atualiza local, mapa e painel.
void deslocar(jogador &p, string local) {
    novaTela();
    campanha.posicao = local;
    mostrarMapaAtual();
    mostrarResumo(p);
    novaTela();
}

// Conversas exclusivas de cada cena, com duas alternativas por momento.
struct Conversa {
    string pergunta;
    string opcao1;
    string resposta1;
    string opcao2;
    string resposta2;
};
Conversa conversas[11][4][2] = {
    // Missao 1
    {
        // Luna
        {
            {
                "Gina: A moeda e o pergaminho reagem juntos. O que voce percebe?",
                "A moeda parece inquieta.",
                "Luna: Talvez ela esteja tentando avisar as outras moedas.\nGina: Vou testar com a minha.",
                "Os retratos estao calados demais.",
                "Luna: Ate os mais tagarelas. Isso nao e normal.\nGina: Entao vamos evitar o corredor dos retratos."
            },
            {
                "Gina: O que devemos contar aos alunos?",
                "Que o castelo ainda nos escuta.",
                "Luna: Eles precisam saber que nao estao esquecidos.\nGina: Vou transmitir isso pela moeda.",
                "Que a chave aponta para a floresta.",
                "Luna: Melhor contar o que sabemos, nao o que imaginamos.\nGina: Vou enviar apenas a rota."
            }
        },
        // Neville
        {
            {
                "Gina: Todos querem uma resposta, Neville.",
                "Ouvir primeiro os alunos.",
                "Neville: Quem viu os simbolos mudar?\nGina: Boa ideia. Temos testemunhas.",
                "Pedir que Gina me ajude a organizar a sala.",
                "Neville: Nao sei por onde comecar sozinho.\nGina: Entao comecamos juntos."
            },
            {
                "Gina: A quem confiamos a mensagem?",
                "Aos alunos que ficaram na sala.",
                "Neville: Eles merecem participar das decisoes.\nGina: Vou reuni-los.",
                "Aos professores aliados.",
                "Neville: Precisamos de alguem que conheca as protecoes.\nGina: Envio a noticia agora."
            }
        },
        // Simas
        {
            {
                "Gina: Por favor, nada de experiencias com o pergaminho.",
                "Examinar a moeda sem tocar no papel.",
                "Simas: Posso ser responsavel por cinco minutos.\nGina: Vou marcar no relogio.",
                "Perguntar quem escreveu os simbolos.",
                "Simas: Alguem deixou uma tarefa sem instrucoes.\nGina: Dumbledore, provavelmente."
            },
            {
                "Gina: Como explicamos a confusao?",
                "Contar sobre a cadeira caida.",
                "Simas: Uma baixissima taxa de danos materiais.\nGina: Vou omitir essa parte.",
                "Registrar apenas o que aprendemos.",
                "Simas: A chave funciona. Um milagre verificavel.\nGina: Finalmente um relatorio util."
            }
        },
        // Cedrico
        {
            {
                "Gina: O grupo espera que voce lidere.",
                "Distribuir as tarefas.",
                "Cedrico: Cada um viu uma parte. Vamos reunir as descobertas.\nGina: Assim ninguem fica de fora.",
                "Pedir que Gina conduza a reuniao.",
                "Cedrico: Voce conhece melhor essas pessoas.\nGina: E voce pode ajudar com a chave."
            },
            {
                "Gina: O que faremos com a descoberta?",
                "Compartilhar com a Armada.",
                "Cedrico: Nao e um premio individual.\nGina: Todos receberao a mensagem.",
                "Pedir que os professores protejam a sala.",
                "Cedrico: Ninguem deveria perder o refugio.\nGina: Vou falar com eles."
            }
        }
    },
    // Minigame 1
    {
        // Luna
        {
            {
                "Estudante: A caixa tem nossos nomes. O que fazemos?",
                "Perguntar ao bandido por que guardou tudo.",
                "Luna: As coisas devem sentir falta de seus donos.\nBandido: Nao quero ouvir sobre sentimentos.",
                "Examinar a bussola.",
                "Luna: Ela aponta para baixo, como se procurasse algo.\nEstudante: Vou lembrar disso."
            },
            {
                "Estudante: Como vamos devolver os pacotes?",
                "Ler as etiquetas com cuidado.",
                "Luna: Cada nome e uma pequena historia.\nEstudante: Vou ajudar a separar.",
                "Deixar a bussola com Gina.",
                "Luna: Ela sabe encontrar pessoas.\nEstudante: Levo a bussola ate ela."
            }
        },
        // Neville
        {
            {
                "Estudante: Sem as caixas nao temos suprimentos.",
                "Conferir os nomes.",
                "Neville: Sei exatamente quem estava esperando isso.\nEstudante: Entao sabemos a quem devolver.",
                "Oferecer uma conversa ao bandido.",
                "Neville: Voce pode devolver as caixas e ir embora.\nBandido: Nao negocio com alunos."
            },
            {
                "Estudante: Quem recebe as pocoes?",
                "Os alunos que mais precisam.",
                "Neville: Nao vamos guardar tudo para nos.\nEstudante: Vou separar.",
                "Pedir um inventario coletivo.",
                "Neville: Assim ninguem fica esquecido.\nEstudante: Eu anoto os nomes."
            }
        },
        // Simas
        {
            {
                "Bandido: Nenhum aluno atravessa sem pagar.",
                "Questionar o pedagio.",
                "Simas: Tem recibo? Quero reclamar com a direcao.\nBandido: Voce fala demais.",
                "Perguntar pela bussola.",
                "Simas: Essa coisa funciona ou tambem cobra taxa?\nEstudante: Ela aponta para a floresta."
            },
            {
                "Estudante: Recuperamos as caixas. E agora?",
                "Conferir se nada quebrou.",
                "Simas: Desta vez vou contar antes de fazer piada.\nEstudante: Tudo inteiro.",
                "Pedir que distribuam as pocoes.",
                "Simas: Melhor beber uma pocao do que pagar pedagio.\nEstudante: Concordo."
            }
        },
        // Cedrico
        {
            {
                "Estudante: Podemos perder os mantimentos.",
                "Oferecer um acordo ao bandido.",
                "Cedrico: Nao precisamos prolongar isso.\nBandido: Nao aceito.",
                "Organizar a devolucao antecipadamente.",
                "Cedrico: Anotem os nomes para quando recuperarmos tudo.\nEstudante: Ja estou fazendo."
            },
            {
                "Estudante: O que faremos com a bussola?",
                "Entregar a Gina.",
                "Cedrico: A resistencia deve decidir como usa-la.\nEstudante: Levo agora.",
                "Explicar o caminho aos demais.",
                "Cedrico: A rota nao pode depender de uma pessoa.\nEstudante: Vou desenhar a passagem."
            }
        }
    },
 // Missao 2
    {
        // Luna
        {
            {
                "Aliado: A chave brilha perto das raizes.",
                "Escutar a floresta.",
                "Luna: O silencio tambem tem direcao.\nAliado: As raizes seguem ate a pedra.",
                "Examinar a inscricao.",
                "Luna: Talvez as letras sejam uma forma de lembrar.\nAliado: Vou ler com voce."
            },
            {
                "Dumbledore: O que voce leva dessa memoria?",
                "Que o castelo pertence a todos.",
                "Luna: Ate quem nunca fala merece ser escutado.\nAliado: Vamos contar a Gina.",
                "Que a chave nao deve ficar sozinha.",
                "Luna: Objetos tambem precisam de companhia.\nAliado: Guardaremos os fragmentos juntos."
            }
        },
        // Neville
        {
            {
                "Aliado: Essas plantas estao cobrindo a entrada.",
                "Identificar as plantas.",
                "Neville: Sprout ensinou que nem toda raiz e obstaculo.\nAliado: Entao deixamos as folhas no lugar.",
                "Ler a inscricao com o grupo.",
                "Neville: Talvez alguem perceba algo que eu nao vi.\nAliado: Aqui esta o simbolo da chave."
            },
            {
                "Dumbledore: A memoria precisa ser compartilhada.",
                "Enviar a descoberta a Gina.",
                "Neville: Ela precisa saber que estamos bem.\nAliado: Vou usar a moeda.",
                "Pedir que Sprout conheca a descoberta.",
                "Neville: As aulas dela nos trouxeram ate aqui.\nAliado: Vou registrar isso."
            }
        },
        // Simas
        {
            {
                "Aliado: A pedra esta se movendo.",
                "Verificar a chave primeiro.",
                "Simas: Nao quero acordar mais nenhum movel.\nAliado: O simbolo coincide.",
                "Ler o aviso da ruina.",
                "Simas: Uma placa teria economizado metade da caminhada.\nAliado: Pelo menos encontramos uma."
            },
            {
                "Dumbledore: O proximo destino e Gringotes.",
                "Perguntar pelo contato.",
                "Simas: Quero alguem que saiba abrir portas sem sustos.\nAliado: A chave indica o nome.",
                "Avisar Gina sobre a ruina.",
                "Simas: Talvez ela goste de saber que segui instrucoes.\nAliado: Essa noticia merece registro."
            }
        },
        // Cedrico
        {
            {
                "Aliado: A inscricao parece um teste.",
                "Conferir a leitura juntos.",
                "Cedrico: Nao quero confundir um aviso com um desafio.\nAliado: A floresta fala de memoria.",
                "Pedir que guardem a rota.",
                "Cedrico: Se houver problemas, todos precisam conhecer a saida.\nAliado: Vou marcar no mapa."
            },
            {
                "Dumbledore: O poder nao pertence a um so.",
                "Dividir a descoberta com a Armada.",
                "Cedrico: Nenhum de nos deve decidir sozinho.\nAliado: Envio a mensagem.",
                "Agradecer ao companheiro.",
                "Cedrico: Sem sua leitura eu teria passado direto.\nAliado: Foi uma descoberta nossa."
            }
        }
    },
    // Minigame 2
    {
        // Luna
        {
            {
                "Aliado: O trasgo parece perdido.",
                "Observar de onde veio.",
                "Luna: Talvez queira voltar para as arvores.\nAliado: Ha marcas na trilha.",
                "Tentar acalma-lo com palavras.",
                "Luna: Nao precisa entender tudo. Basta ouvir o tom.\nAliado: Vou esperar."
            },
            {
                "Aliado: A criatura finalmente se afastou.",
                "Deixar a trilha tranquila.",
                "Luna: Ela ja teve uma noite estranha.\nAliado: Vamos evitar barulho.",
                "Avisar os proximos viajantes.",
                "Luna: Ninguem deve assusta-la outra vez.\nAliado: Marco a rota secundaria."
            }
        },
        // Neville
        {
            {
                "Aliado: A nevoa o deixou confuso.",
                "Pedir que os colegas recuem.",
                "Neville: Vamos dar espaco para a criatura.\nAliado: Todos juntos, entao.",
                "Examinar as marcas da trilha.",
                "Neville: O caminho habitual foi encoberto.\nAliado: Agora vejo as pegadas."
            },
            {
                "Aliado: Podemos atravessar.",
                "Conferir se todos estao bem.",
                "Neville: Ninguem ficou para tras?\nAliado: Estamos todos aqui.",
                "Avisar Sprout sobre a nevoa.",
                "Neville: Talvez ela saiba por que apareceu.\nAliado: Vou levar a noticia."
            }
        },
        // Simas
        {
            {
                "Aliado: Temos um trasgo na frente.",
                "Sugerir uma retirada silenciosa dos colegas.",
                "Simas: Hoje ninguem vai testar meu talento para confusao.\nAliado: Sensato, pela primeira vez.",
                "Observar o elmo abandonado.",
                "Simas: Que bom gosto. Definitivamente nao e meu tamanho.\nAliado: Depois veremos isso."
            },
            {
                "Aliado: A trilha ficou livre.",
                "Fazer uma piada sobre a visita.",
                "Simas: Dou duas estrelas. Atendimento barulhento.\nAliado: Vamos antes que ele volte.",
                "Admitir que o susto foi real.",
                "Simas: Esta bem, eu fiquei nervoso.\nAliado: Todos ficamos."
            }
        },
        // Cedrico
        {
            {
                "Aliado: Ha uma rota secundaria.",
                "Pedir que o grupo mantenha distancia.",
                "Cedrico: Ninguem precisa ficar perto da criatura.\nAliado: Vou organizar os outros.",
                "Procurar a origem da nevoa.",
                "Cedrico: A criatura nao escolheu estar aqui.\nAliado: As marcas chegam da ruina."
            },
            {
                "Aliado: A clareira esta segura.",
                "Checar os companheiros.",
                "Cedrico: Recompensas podem esperar.\nAliado: Todos estao bem.",
                "Deixar um aviso na trilha.",
                "Cedrico: Outros estudantes podem passar por aqui.\nAliado: Vou escrever um sinal."
            }
        }
    },
    // Missao 3
    {
        // Luna
        {
            {
                "Duende: O cofre guarda objetos muito antigos.",
                "Perguntar pela voz dos objetos.",
                "Luna: As vezes uma lembranca fica onde ninguem espera.\nDuende: Prefiro catalogos a conversas com objetos.",
                "Perguntar pelo simbolo na porta.",
                "Luna: A chave parece reconhecer o lugar.\nDuende: Foi feita para esse cofre."
            },
            {
                "Duende: Ranrok e um nome antigo.",
                "Perguntar por sua historia.",
                "Luna: Uma voz esquecida merece ser ouvida.\nDuende: Nem todos concordam sobre o passado.",
                "Pedir que Gina saiba do terceiro fragmento.",
                "Luna: Ela deve estar esperando a mensagem.\nDuende: O contato pode envia-la."
            }
        },
        // Neville
        {
            {
                "Duende: Voce reconhece aquela fotografia?",
                "Perguntar de onde veio.",
                "Neville: Meus pais estavam aqui?\nDuende: O cofre recebeu arquivos de antigos diretores.",
                "Guardar a lembranca sem tocar na foto.",
                "Neville: Basta saber que ela existe.\nDuende: Entendo."
            },
            {
                "Duende: O proximo fragmento esta sob o castelo.",
                "Pedir uma copia da inscricao.",
                "Neville: Nao quero esquecer nenhum detalhe.\nDuende: Vou preparar uma transcricao.",
                "Agradecer pela fotografia.",
                "Neville: Hoje encontrei mais do que vim buscar.\nDuende: A memoria tambem tem valor."
            }
        },
        // Simas
        {
            {
                "Duende: Nao toque em nada.",
                "Perguntar como funciona o bau.",
                "Simas: Instrucoes, finalmente!\nDuende: Observe as pistas numericas, nao os enfeites.",
                "Afastar-se dos objetos frageis.",
                "Simas: Vou preservar minha excelente reputacao.\nDuende: Agradeco a sensatez."
            },
            {
                "Duende: A inscricao fala de Ranrok.",
                "Ouvir sem interromper.",
                "Simas: Esta historia parece grande demais para uma piada.\nDuende: E uma historia de desacordo.",
                "Perguntar como voltar ao castelo.",
                "Simas: Prefiro uma viagem sem mobiliario animado.\nDuende: O contato espera do lado de fora."
            }
        },
        // Cedrico
        {
            {
                "Duende: Este cofre nao guarda apenas ouro.",
                "Perguntar sobre os antigos diretores.",
                "Cedrico: Quem decidiu guardar essas coisas?\nDuende: Houve varios responsaveis.",
                "Pedir que expliquem as regras do bau.",
                "Cedrico: Nao quero tomar uma recompensa que nao nos cabe.\nDuende: As pistas sao iguais para todos."
            },
            {
                "Duende: Ranrok disputava o acesso a magia.",
                "Perguntar sobre as injusticas.",
                "Cedrico: Nao podemos proteger a escola ignorando seu passado.\nDuende: Essa pergunta merece tempo.",
                "Compartilhar a inscricao com os aliados.",
                "Cedrico: Precisamos decidir juntos.\nDuende: Levem a transcricao."
            }
        }
    },
    // Minigame 3
    {
        // Luna
        {
            {
                "Hagrid: Bicuco esta assustado.",
                "Cumprimenta-lo de longe.",
                "Luna: Ele precisa saber que nao somos mais um susto.\nHagrid: Isso, devagar.",
                "Perguntar sobre o eco.",
                "Luna: Uma lembranca pode esquecer que nao e real?\nHagrid: A coisa apareceu junto com a luz do castelo."
            },
            {
                "Hagrid: Ele precisa descansar.",
                "Pedir que o deixe em sossego.",
                "Luna: Nem toda alegria precisa de uma festa.\nHagrid: Vou preparar um canto tranquilo.",
                "Contar que a magia antiga esta despertando.",
                "Luna: Talvez outras lembrancas aparecam.\nHagrid: Vou ficar atento."
            }
        },
        // Neville
        {
            {
                "Hagrid: Ainda bem que voce veio.",
                "Perguntar como Bicuco esta.",
                "Neville: Quero saber se ele consegue caminhar.\nHagrid: Esta assustado, mas consegue.",
                "Pedir que Hagrid fique por perto.",
                "Neville: Vou precisar de voce quando o encanto ceder.\nHagrid: Pode contar comigo."
            },
            {
                "Hagrid: Ele voltou para casa.",
                "Agradecer a ajuda.",
                "Neville: Nao teria conseguido fazer tudo sozinho.\nHagrid: E para isso que servem os amigos.",
                "Avisar sobre o subsolo.",
                "Neville: Preciso proteger o castelo agora.\nHagrid: Cuidado, garoto."
            }
        },
 // Simas
        {
            {
                "Hagrid: Parece um fantasma, mas nao e.",
                "Perguntar se lembrancas costumam dar trabalho.",
                "Simas: As minhas normalmente so rendem detencao.\nHagrid: Essa aqui e diferente.",
                "Perguntar por Bicuco.",
                "Simas: Primeiro o hipogrifo, depois a aula de fantasmas.\nHagrid: Ele esta ali."
            },
            {
                "Hagrid: Bicuco gostou de voce.",
                "Fazer uma reverencia exageradamente formal.",
                "Simas: Finalmente, um publico exigente.\nHagrid: Ele merece respeito.",
                "Pedir que Hagrid conte a Gina.",
                "Simas: Quero uma testemunha de que ajudei sem quebrar nada.\nHagrid: Vou contar."
            }
        },
        // Cedrico
        {
            {
                "Hagrid: Nao consigo desfazer sozinho o encantamento.",
                "Combinar como receber Bicuco.",
                "Cedrico: Assim que estiver livre, ele precisa de voce.\nHagrid: Estarei aqui.",
                "Perguntar se outros alunos sabem do problema.",
                "Cedrico: Alguem deve conhecer a rota de volta.\nHagrid: Ja avisei a resistencia."
            },
            {
                "Hagrid: Obrigado por voltar.",
                "Compartilhar a responsabilidade.",
                "Cedrico: Voce ficou com ele durante todo esse tempo.\nHagrid: E agora esta seguro.",
                "Pedir que avise os demais.",
                "Cedrico: Precisamos saber se outros ecos aparecerem.\nHagrid: Vou mandar uma mensagem."
            }
        }
    },
    // Missao 4
    {
        // Luna
        {
            {
                "Gina: O subsolo esta reagindo aos fragmentos.",
                "Perguntar se o castelo sente medo.",
                "Luna: A pedra parece respirar com dificuldade.\nGina: Entao vamos devolver sua tranquilidade.",
                "Juntar os fragmentos perto da chave.",
                "Luna: Eles parecem reconhecer uns aos outros.\nGina: A passagem respondeu."
            },
            {
                "Gina: Temos os tres fragmentos.",
                "Pedir que guarde a lembranca da noite.",
                "Luna: Um dia os alunos vao querer saber como foi.\nGina: Nao deixaremos isso desaparecer.",
                "Perguntar pelos estudantes.",
                "Luna: Precisam de um lugar onde possam rir.\nGina: Os professores estao com eles."
            }
        },
        // Neville
        {
            {
                "Gina: Os menores chegaram ao refugio.",
                "Confirmar se todos chegaram.",
                "Neville: Nao quero que reste ninguem nos corredores.\nGina: Conferimos os nomes.",
                "Contar que ainda sente medo.",
                "Neville: Nao tenho certeza de que estou pronto.\nGina: Nao precisa ter certeza de tudo."
            },
            {
                "Gina: A entrada esta aberta.",
                "Agradecer aos colegas.",
                "Neville: Nenhum fragmento veio parar aqui sozinho.\nGina: Eles vao gostar de ouvir isso.",
                "Combinar a mensagem de retorno.",
                "Neville: Se a moeda esfriar, saberemos que acabou.\nGina: Vou esperar o sinal."
            }
        },
        // Simas
        {
            {
                "Gina: Leia as indicacoes da chave.",
                "Ler tudo antes de agir.",
                "Simas: Nao ria. Estou seguindo o manual.\nGina: Vou guardar esse momento.",
                "Perguntar se a passagem tem saida.",
                "Simas: Prefiro nao passar a noite dentro de uma parede.\nGina: A chave tambem abre o retorno."
            },
            {
                "Gina: O sentinela voltou ao normal.",
                "Contar que seguiu as instrucoes.",
                "Simas: Admito, elas eram uteis.\nGina: Isso merece uma comemoração.",
                "Perguntar se os outros estao seguros.",
                "Simas: Brincadeiras depois. Como estao todos?\nGina: Estao protegidos."
            }
        },
        // Cedrico
        {
            {
                "Gina: Voce pode confiar na Armada.",
                "Aceitar que ela coordene a superficie.",
                "Cedrico: Entao deixo essa parte com voce.\nGina: Finalmente.",
                "Perguntar o que os colegas precisam.",
                "Cedrico: Nao quero decidir por eles.\nGina: Precisam que resolvamos a passagem."
            },
            {
                "Gina: O ultimo fragmento esta seguro.",
                "Compartilhar o merito.",
                "Cedrico: Essa descoberta tem muitos autores.\nGina: Vou contar a todos.",
                "Pedir que a Armada preserve a rota.",
                "Cedrico: Precisamos poder voltar juntos.\nGina: A passagem ficara protegida."
            }
        }
    },
    // Minigame 4
    {
        // Luna
        {
            {
                "Professor: Qual lembranca ajuda a afastar o frio?",
                "Conversar sobre a mae.",
                "Luna: Ela me ensinou a procurar beleza no inesperado.\nProfessor: Guarde essa lembranca.",
                "Recordar as tardes com amigos.",
                "Luna: Quando conversamos, o mundo fica menos estranho.\nProfessor: Isso pode ajudar."
            },
            {
                "Professor: O frio passou.",
                "Perguntar se ele tambem sentiu medo.",
                "Luna: Nao precisa esconder de mim.\nProfessor: Todos sentimos, Luna.",
                "Agradecer pela companhia.",
                "Luna: Foi bom nao ouvir apenas o silencio.\nProfessor: Voce nao estava sozinha."
            }
        },
        // Neville
        {
            {
                "Professor: Nao precisa fingir coragem.",
                "Falar sobre os pais.",
                "Neville: Quero continuar tentando por eles.\nProfessor: E por voce tambem.",
                "Lembrar a Sala Precisa.",
                "Neville: Meus amigos me esperaram mesmo quando duvidei.\nProfessor: Confie nessa lembranca."
            },
            {
                "Professor: A passagem esta livre.",
                "Admitir o medo.",
                "Neville: Ainda estou assustado.\nProfessor: E mesmo assim voce continuou.",
                "Agradecer pela orientacao.",
                "Neville: Agora sei que posso pedir ajuda.\nProfessor: Nunca esqueca disso."
            }
        },
        // Simas
        {
            {
                "Professor: Encontre uma lembranca feliz.",
                "Recordar uma noite em que todos riram.",
                "Simas: Nem lembro da piada, so das risadas.\nProfessor: Isso basta.",
                "Falar das conversas em casa.",
                "Simas: Quero voltar a contar historias sem assustar ninguem.\nProfessor: Pense nisso."
            },
            {
                "Professor: Voce conseguiu se concentrar.",
                "Fazer uma piada discreta.",
                "Simas: Nenhum caldeirao foi prejudicado.\nProfessor: Um excelente resultado.",
                "Falar seriamente sobre os amigos.",
                "Simas: Quero que todos cheguem ao fim desta noite.\nProfessor: E um bom motivo."
            }
        },
        // Cedrico
        {
            {
                "Professor: Pense em quem o acolheu.",
                "Lembrar o retorno do torneio.",
                "Cedrico: Alguem me lembrou que eu nao precisava ser um campeao.\nProfessor: Voce continua sendo voce.",
                "Lembrar os colegas que ficaram.",
                "Cedrico: Eles nao exigiram que eu fosse perfeito.\nProfessor: Isso e importante."
            },
            {
                "Professor: Ja pode continuar.",
                "Agradecer por nao ficar sozinho.",
                "Cedrico: Achei que devia enfrentar tudo sem ajuda.\nProfessor: Nao precisa.",
                "Pedir que acompanhe os alunos.",
                "Cedrico: Eles tambem podem precisar dessa orientacao.\nProfessor: Vou procura-los."
            }
        }
    },
    // Minigame 5
    {
        // Luna
        {
            {
                "Gina: A Armada esta cansada.",
                "Perguntar o que ouviram da camara.",
                "Luna: As pedras estao contando alguma coisa?\nGina: O selo vibra quando chegamos perto.",
                "Perguntar pelos colegas.",
                "Luna: Precisamos que descansem depois disso.\nGina: Assim que a passagem estiver livre."
            },
            {
                "Gina: A camara esta acessivel.",
                "Pedir que espere pelo sinal da moeda.",
                "Luna: Assim a gente nao precisa gritar para o castelo inteiro.\nGina: Combinado.",
                "Deixar uma mensagem de esperanca.",
                "Luna: Digam que ainda existem coisas bonitas aqui.\nGina: Vou lembrar."
            }
        },
        // Neville
        {
            {
                "Gina: Precisamos de uma ultima ajuda.",
                "Ouvir o plano da Armada.",
                "Neville: O que voces ja conseguiram fazer?\nGina: Mantivemos a rota aberta.",
                "Perguntar se alguem precisa de apoio.",
                "Neville: Ninguem deve ficar esquecido.\nGina: Os professores estao chegando."
            },
            {
                "Gina: Voce pode entrar.",
                "Agradecer a todos.",
                "Neville: Nao esperem que eu vire outra pessoa.\nGina: Queremos apenas voce.",
                "Combinar onde se reencontrar.",
                "Neville: Na Sala Precisa, quando terminar.\nGina: Estaremos la."
            }
        },
        // Simas
        {
            {
                "Gina: Voce demorou.",
                "Explicar a confusao do caminho.",
                "Simas: Ate o mapa parecia estar de mau humor.\nGina: Guarde essa desculpa para depois.",
                "Perguntar qual e a prioridade.",
                "Simas: Desta vez quero saber o plano inteiro.\nGina: Manter a entrada livre."
            },
            {
                "Gina: Tem certeza sobre o selo?",
                "Confirmar que vai seguir a chave.",
                "Simas: Ate eu sei quando parar de improvisar.\nGina: Vou acreditar.",
                "Pedir que conte as boas noticias.",
                "Simas: Se tudo der certo, quero uma rodada de risadas.\nGina: Combinado."
            }
        },
        // Cedrico
        {
            {
                "Gina: A entrada esta quase livre.",
                "Perguntar como ajudar.",
                "Cedrico: Voces conhecem melhor a situacao.\nGina: Precisamos manter a passagem.",
                "Pedir que organizem o retorno.",
                "Cedrico: Ninguem fica sozinho aqui.\nGina: Ja dividimos as tarefas."
            },
            {
                "Gina: A camara esta a sua frente.",
                "Aceitar a confianca.",
                "Cedrico: Vou fazer minha parte. Sei que farao a de voces.\nGina: Pode seguir.",
                "Pedir que avisem os professores.",
                "Cedrico: O que acontecer la dentro deve ser compartilhado.\nGina: Vou enviar a mensagem."
            }
        }
    },

// Minigame 4
    {
        // Luna
        {
            {
                "Professor: Qual lembranca ajuda a afastar o frio?",
                "Conversar sobre a mae.",
                "Luna: Ela me ensinou a procurar beleza no inesperado.\nProfessor: Guarde essa lembranca.",
                "Recordar as tardes com amigos.",
                "Luna: Quando conversamos, o mundo fica menos estranho.\nProfessor: Isso pode ajudar."
            },
            {
                "Professor: O frio passou.",
                "Perguntar se ele tambem sentiu medo.",
                "Luna: Nao precisa esconder de mim.\nProfessor: Todos sentimos, Luna.",
                "Agradecer pela companhia.",
                "Luna: Foi bom nao ouvir apenas o silencio.\nProfessor: Voce nao estava sozinha."
            }
        },
        // Neville
        {
            {
                "Professor: Nao precisa fingir coragem.",
                "Falar sobre os pais.",
                "Neville: Quero continuar tentando por eles.\nProfessor: E por voce tambem.",
                "Lembrar a Sala Precisa.",
                "Neville: Meus amigos me esperaram mesmo quando duvidei.\nProfessor: Confie nessa lembranca."
            },
            {
                "Professor: A passagem esta livre.",
                "Admitir o medo.",
                "Neville: Ainda estou assustado.\nProfessor: E mesmo assim voce continuou.",
                "Agradecer pela orientacao.",
                "Neville: Agora sei que posso pedir ajuda.\nProfessor: Nunca esqueca disso."
            }
        },
        // Simas
        {
            {
                "Professor: Encontre uma lembranca feliz.",
                "Recordar uma noite em que todos riram.",
                "Simas: Nem lembro da piada, so das risadas.\nProfessor: Isso basta.",
                "Falar das conversas em casa.",
                "Simas: Quero voltar a contar historias sem assustar ninguem.\nProfessor: Pense nisso."
            },
            {
                "Professor: Voce conseguiu se concentrar.",
                "Fazer uma piada discreta.",
                "Simas: Nenhum caldeirao foi prejudicado.\nProfessor: Um excelente resultado.",
                "Falar seriamente sobre os amigos.",
                "Simas: Quero que todos cheguem ao fim desta noite.\nProfessor: E um bom motivo."
            }
        },
        // Cedrico
        {
            {
                "Professor: Pense em quem o acolheu.",
                "Lembrar o retorno do torneio.",
                "Cedrico: Alguem me lembrou que eu nao precisava ser um campeao.\nProfessor: Voce continua sendo voce.",
                "Lembrar os colegas que ficaram.",
                "Cedrico: Eles nao exigiram que eu fosse perfeito.\nProfessor: Isso e importante."
            },
            {
                "Professor: Ja pode continuar.",
                "Agradecer por nao ficar sozinho.",
                "Cedrico: Achei que devia enfrentar tudo sem ajuda.\nProfessor: Nao precisa.",
                "Pedir que acompanhe os alunos.",
                "Cedrico: Eles tambem podem precisar dessa orientacao.\nProfessor: Vou procura-los."
            }
        }
    },
    // Minigame 5
    {
        // Luna
        {
            {
                "Gina: A Armada esta cansada.",
                "Perguntar o que ouviram da camara.",
                "Luna: As pedras estao contando alguma coisa?\nGina: O selo vibra quando chegamos perto.",
                "Perguntar pelos colegas.",
                "Luna: Precisamos que descansem depois disso.\nGina: Assim que a passagem estiver livre."
            },
            {
                "Gina: A camara esta acessivel.",
                "Pedir que espere pelo sinal da moeda.",
                "Luna: Assim a gente nao precisa gritar para o castelo inteiro.\nGina: Combinado.",
                "Deixar uma mensagem de esperanca.",
                "Luna: Digam que ainda existem coisas bonitas aqui.\nGina: Vou lembrar."
            }
        },
        // Neville
        {
            {
                "Gina: Precisamos de uma ultima ajuda.",
                "Ouvir o plano da Armada.",
                "Neville: O que voces ja conseguiram fazer?\nGina: Mantivemos a rota aberta.",
                "Perguntar se alguem precisa de apoio.",
                "Neville: Ninguem deve ficar esquecido.\nGina: Os professores estao chegando."
            },
            {
                "Gina: Voce pode entrar.",
                "Agradecer a todos.",
                "Neville: Nao esperem que eu vire outra pessoa.\nGina: Queremos apenas voce.",
                "Combinar onde se reencontrar.",
                "Neville: Na Sala Precisa, quando terminar.\nGina: Estaremos la."
            }
        },
        // Simas
        {
            {
                "Gina: Voce demorou.",
                "Explicar a confusao do caminho.",
                "Simas: Ate o mapa parecia estar de mau humor.\nGina: Guarde essa desculpa para depois.",
                "Perguntar qual e a prioridade.",
                "Simas: Desta vez quero saber o plano inteiro.\nGina: Manter a entrada livre."
            },
            {
                "Gina: Tem certeza sobre o selo?",
                "Confirmar que vai seguir a chave.",
                "Simas: Ate eu sei quando parar de improvisar.\nGina: Vou acreditar.",
                "Pedir que conte as boas noticias.",
                "Simas: Se tudo der certo, quero uma rodada de risadas.\nGina: Combinado."
            }
        },
        // Cedrico
        {
            {
                "Gina: A entrada esta quase livre.",
                "Perguntar como ajudar.",
                "Cedrico: Voces conhecem melhor a situacao.\nGina: Precisamos manter a passagem.",
                "Pedir que organizem o retorno.",
                "Cedrico: Ninguem fica sozinho aqui.\nGina: Ja dividimos as tarefas."
            },
            {
                "Gina: A camara esta a sua frente.",
                "Aceitar a confianca.",
                "Cedrico: Vou fazer minha parte. Sei que farao a de voces.\nGina: Pode seguir.",
                "Pedir que avisem os professores.",
                "Cedrico: O que acontecer la dentro deve ser compartilhado.\nGina: Vou enviar a mensagem."
            }
        }
    },
    // Final Voldemort
    {
        // Luna
        {
            {
                "Voldemort: Por que insiste em proteger este lugar?",
                "Falar das lembrancas do castelo.",
                "Luna: Ha coisas aqui que voce nunca tentou compreender.\nVoldemort: Lembrancas nao governam magia.",
                "Falar das pessoas que o frequentam.",
                "Luna: Hogwarts e mais do que suas paredes.\nVoldemort: Entao nao entendeu seu potencial."
            },
            {
                "Gina: O selo voltou a funcionar.",
                "Contar que ouviu o castelo.",
                "Luna: Ele parece menos cansado agora.\nGina: Espero que voce tambem consiga descansar.",
                "Lembrar que a guerra continua.",
                "Luna: Por hoje encontramos um pouco de paz.\nGina: Vamos guarda-la."
            }
        },
        // Neville
        {
            {
                "Voldemort: Quem lhe deu o direito de estar aqui?",
                "Falar da Armada.",
                "Neville: Nao vim sozinho, mesmo que voce nao veja os outros.\nVoldemort: Aliados sao frageis.",
                "Falar de sua propria escolha.",
                "Neville: Nao preciso de um titulo para saber por que vim.\nVoldemort: Palavras vazias."
            },
            {
                "Gina: O vinculo foi rompido.",
                "Admitir que ainda sente medo.",
                "Neville: Achei que coragem fosse outra coisa.\nGina: Voce encontrou a sua.",
                "Agradecer por ela ter esperado.",
                "Neville: Saber que voces estavam aqui fez diferenca.\nGina: Sempre estaremos juntos."
            }
        },
        // Simas
        {
            {
                "Voldemort: A escola desperdicou seu potencial.",
                "Questionar a ideia de um trono.",
                "Simas: Uma escola nao precisa de rei.\nVoldemort: Nao compreende a grandeza.",
                "Concentrar-se na chave.",
                "Simas: Vou ouvir as instrucoes em vez do discurso.\nVoldemort: Insolente."
            },
            {
                "Gina: O castelo esta inteiro?",
                "Contar que seguiu o plano.",
                "Simas: Uma noite historica para os meus professores.\nGina: Vou anotar.",
                "Perguntar pelos colegas antes de brincar.",
                "Simas: Estao todos bem?\nGina: Sim. Agora pode respirar."
            }
        },
        // Cedrico
        {
            {
                "Voldemort: O passado nao lhe ensinou nada?",
                "Falar do futuro dos alunos.",
                "Cedrico: Ensinou que o amanha importa.\nVoldemort: O futuro pertence aos fortes.",
                "Recusar transformar a noite em revanche.",
                "Cedrico: Nao vim acertar contas pessoais.\nVoldemort: Entao nao sabe o que deseja."
            },
            {
                "Gina: Voce voltou.",
                "Compartilhar a vitoria.",
                "Cedrico: Todos fizeram parte do selo.\nGina: E todos precisam saber disso.",
                "Pedir que os colegas descansem.",
                "Cedrico: Amanhã ainda vamos precisar uns dos outros.\nGina: Inclusive de voce."
            }
        }
    },
    // Final Ranrok
    {
        // Luna
        {
            {
                "Ranrok: Ninguem se lembra do que nos foi negado.",
                "Pedir que conte sua historia.",
                "Luna: Posso ouvir sem concordar com tudo.\nRanrok: Ouvir nao desfaz o passado.",
                "Questionar a liberdade da fonte.",
                "Luna: Se ela obedecer apenas a voce, ainda sera livre?\nRanrok: Voce nao conhece nossa historia."
            },
            {
                "Gina: O eco desapareceu.",
                "Pedir que registrem sua historia.",
                "Luna: Uma voz nao precisa virar ameaca para ser lembrada.\nGina: Vamos preservar o relato.",
                "Escutar o silencio da camara.",
                "Luna: Agora parece um silencio diferente.\nGina: Um silencio tranquilo."
            }
        },
        // Neville
        {
            {
                "Ranrok: Voce sabe o que e ser diminuido?",
                "Reconhecer a injustica.",
                "Neville: Sei. Mas nao quero devolver isso aos outros.\nRanrok: Suas palavras nao mudam seculos.",
                "Falar da responsabilidade atual.",
                "Neville: Ha estudantes que nao escolheram esse conflito.\nRanrok: E quantos foram esquecidos antes deles?"
            },
            {
                "Gina: A fonte esta protegida.",
                "Compartilhar o relato de Ranrok.",
                "Neville: Nao podemos fingir que ele nunca existiu.\nGina: Vamos registrar a historia.",
                "Refletir sobre a propria coragem.",
                "Neville: Nao preciso dominar nada para ser importante.\nGina: Nunca precisou."
            }
        },
        // Simas
        {
            {
                "Ranrok: Hoje a magia sera de todos.",
                "Questionar as consequencias.",
                "Simas: De todos, inclusive de quem esta em cima desta camara?\nRanrok: O preco nao me interessa.",
                "Pedir uma explicacao antes do confronto.",
                "Simas: Por uma vez prefiro uma aula a uma confusao.\nRanrok: Entao escute o que nos foi negado."
            },
            {
                "Gina: O ultimo cristal perdeu a luz.",
                "Admitir que aprendeu a ter paciencia.",
                "Simas: Talvez seja uma habilidade que eu devia estudar.\nGina: Finalmente.",
                "Pedir que guardem a historia.",
                "Simas: Nao quero que o eco seja a unica coisa lembrada.\nGina: Vou registrar o que aconteceu."
            }
        },
        // Cedrico
        {
            {
                "Ranrok: Quem decidiu que voces merecem magia?",
                "Reconhecer que nenhuma pessoa deveria decidir.",
                "Cedrico: A escola tambem precisa aprender com seus erros.\nRanrok: Aprender nao basta.",
                "Recusar a abertura descontrolada da fonte.",
                "Cedrico: Nao posso aceitar que outros paguem por isso.\nRanrok: Sempre ha um preco."
            },
            {
                "Gina: O perigo imediato acabou.",
                "Pedir que investiguem a historia.",
                "Cedrico: Nao podemos confundir proteger com esquecer.\nGina: Vou conversar com os professores.",
                "Agradecer aos colegas.",
                "Cedrico: O selo foi restaurado por todos nos.\nGina: E assim que deve ser contado."
            }
        }
    }
};
