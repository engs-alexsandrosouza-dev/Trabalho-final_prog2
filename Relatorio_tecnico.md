# RELATÓRIO TÉCNICO — MYSTERY OF HOGWARTS

**Centro Universitário Campo Real**  
**Curso:** Engenharia de Software  
**Disciplina:** Programação e Sistemas Computacionais II  
**Professor:** Giovane Galvão  
**Integrantes:** Alexsandro, Fabricio, Lucas e Valentin

## 1. Introdução

Mystery of Hogwarts é um jogo de RPG desenvolvido em C++, inspirado no universo de Harry Potter. Sua proposta consiste em uma campanha interativa na qual o jogador assume o papel de um estudante de Hogwarts, participando de missões, enfrentando desafios e desenvolvendo suas habilidades ao longo da história.

A escolha do tema ocorreu pela possibilidade de aplicar os conteúdos estudados durante o semestre em diferentes mecânicas de um único sistema. Dessa maneira, buscamos utilizar as estruturas de dados e os algoritmos não apenas como requisitos da disciplina, mas como parte do funcionamento do jogo.

## 2. Organização e desenvolvimento do sistema

Inicialmente, definimos as regras da campanha, os atributos dos personagens, os feitiços, as recompensas e as condições necessárias para a conclusão do jogo. A partir dessas definições, organizamos o código em funções e estruturas responsáveis por armazenar e manipular essas informações.

As structs foram utilizadas para representar personagens, feitiços, armaduras, inimigos e o estado geral da campanha. Para informações com quantidade previamente definida, como os quatro personagens e os dez feitiços, optamos pela utilização de vetores.

A modularização permitiu reutilizar funcionalidades em diferentes partes do programa. Um exemplo é o sistema de combate, utilizado tanto nas missões principais quanto nos eventos secundários.

Também utilizamos passagem por valor e referência de acordo com a finalidade de cada função. A passagem por referência foi especialmente importante nas funções que precisam modificar permanentemente os atributos dos personagens, como vida, experiência e nível.

## 3. Estruturas de dados e decisões de projeto

### Lista encadeada

A lista encadeada foi utilizada para representar os feitiços desbloqueados durante a campanha. Escolhemos essa estrutura porque a quantidade de feitiços disponíveis ao jogador muda conforme sua progressão.

Diferentemente do vetor que armazena todos os feitiços cadastrados, a lista representa aqueles que estão disponíveis em determinado momento. Cada nó contém os dados de um feitiço e um ponteiro para o próximo elemento.

Foram implementadas operações de inserção, remoção e travessia. Como a inserção ocorre ao final da lista e exige seu percurso, apresenta complexidade O(n) no pior caso, assim como a busca de um elemento para remoção.

### Pilha

A pilha foi escolhida para armazenar o histórico das ações realizadas pelo jogador, como utilização de feitiços, consumo de itens e decisões tomadas durante a campanha.

Sua utilização se justifica pela necessidade de apresentar primeiro as ações mais recentes. A estrutura segue o princípio LIFO, no qual o último elemento inserido é o primeiro a ser removido.

A utilização de um ponteiro para o topo permite realizar as operações de inserção e remoção com complexidade O(1), sem precisar percorrer toda a estrutura. A exibição do histórico possui complexidade O(n).

### Fila

A fila foi utilizada para organizar os eventos da campanha, garantindo que sejam processados de acordo com sua ordem de inserção.

Essa estrutura segue o princípio FIFO, no qual o primeiro elemento inserido é o primeiro a ser removido. Sua implementação utiliza dois ponteiros, um para o início e outro para o final da fila.

Essa escolha permite adicionar e remover eventos com complexidade O(1), evitando a necessidade de percorrer todos os elementos a cada operação.

### Árvore binária de busca

A árvore binária de busca foi aplicada ao sistema de baús misteriosos, no qual as recompensas são associadas a chaves numéricas.

A estrutura foi escolhida por permitir a localização de elementos a partir da comparação entre suas chaves. Durante a busca, valores menores direcionam o percurso para a esquerda e valores maiores para a direita.

Além de realizar inserções e buscas, implementamos a travessia in-ordem, que permite apresentar as chaves em ordem crescente.

A quantidade de comparações realizadas durante a busca também foi incorporada à mecânica do desafio relacionado à obtenção da Espada da Grifinória.

A busca possui complexidade O(log n) em uma árvore equilibrada, podendo chegar a O(n) quando a estrutura está desbalanceada.

## 4. Algoritmos de ordenação, busca e recursividade

Para organizar os feitiços de acordo com seus valores de dano, implementamos o Insertion Sort. A escolha desse algoritmo ocorreu pela simplicidade de implementação e pela pequena quantidade de elementos cadastrados no jogo.

Também implementamos o Quick Sort como uma aplicação adicional de recursividade. Seu funcionamento consiste na escolha de um pivô, na divisão dos elementos e na ordenação recursiva das partições.

As ordenações são realizadas sobre cópias dos dados, preservando a organização original do inventário.

Para demonstrar diferentes estratégias de localização de elementos, implementamos a busca sequencial e a busca binária. Enquanto a primeira percorre os elementos individualmente, a segunda reduz o intervalo de busca a cada iteração, utilizando um vetor previamente ordenado.

O programa contabiliza as comparações realizadas pelos dois algoritmos, permitindo observar suas diferenças de desempenho.

Além do Quick Sort, a recursividade está presente nas operações de inserção, busca, travessia e liberação da árvore binária.

## 5. Ponteiros e gerenciamento de memória

A implementação manual das estruturas de dados exigiu a utilização de ponteiros e alocação dinâmica de memória.

Os ponteiros permitem armazenar os endereços dos nós e estabelecer as ligações entre os elementos. Dessa forma, as estruturas podem aumentar ou diminuir durante a execução do programa, sem depender de um tamanho fixo.

A criação dos nós ocorre através do operador `new`, enquanto a liberação da memória é realizada através do operador `delete`. O valor `nullptr` é utilizado para representar ponteiros que não possuem um endereço válido associado.

Foram desenvolvidas funções específicas para liberar a lista encadeada, a pilha, a fila e a árvore binária. No caso da árvore, utilizamos a liberação recursiva em pós-ordem, garantindo que os nós filhos sejam removidos antes de seus respectivos nós pais.

Esse gerenciamento é importante para evitar que regiões de memória permaneçam alocadas após deixarem de ser utilizadas pelo programa.
