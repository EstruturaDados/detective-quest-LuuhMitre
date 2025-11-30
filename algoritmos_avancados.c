#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definições de tamanho
#define TAM_HASH 10
#define TAM_STRING 50

// --- ESTRUTURAS DE DADOS ---

// 1. Árvore Binária (Mapa da Mansão)
typedef struct Sala {
    char nome[TAM_STRING];
    char pista[TAM_STRING]; // Pista encontrada nesta sala (pode ser vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// 2. Árvore de Busca Binária - BST (Inventário de Pistas)
typedef struct PistaNode {
    char texto[TAM_STRING];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// 3. Tabela Hash (Base de Conhecimento: Pista -> Suspeito)
typedef struct HashEntry {
    char pista[TAM_STRING];
    char suspeito[TAM_STRING];
    struct HashEntry *proximo; // Tratamento de colisão (Lista Encadeada)
} HashEntry;

// Variável Global para a Tabela Hash
HashEntry* tabelaHash[TAM_HASH];

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções da Hash
void inicializarHash();
int funcaoHash(char *chave);
void inserirHash(char *pista, char *suspeito);
char* buscarSuspeito(char *pista);

// Funções da BST
PistaNode* inserirBST(PistaNode *raiz, char *texto);
void exibirPistasEmOrdem(PistaNode *raiz);
int contarEvidenciasContraSuspeito(PistaNode *raiz, char *suspeitoAcusado);

// Funções da Árvore Binária (Mapa)
Sala* criarSala(char *nome, char *pista);
void explorarSalas(Sala *salaAtual, PistaNode **inventarioPistas);

// Auxiliares
void limparBuffer();

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {
    // Inicialização das estruturas
    inicializarHash();
    PistaNode *arvorePistas = NULL;

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    
    // Populando a "Base de Conhecimento" (Tabela Hash) antes do jogo começar
    // O jogo "sabe" qual pista leva a qual suspeito.
    inserirHash("Castical", "Sr. Mostarda");
    inserirHash("Luvas Sujas", "Sr. Mostarda");
    inserirHash("Adaga", "Dona Branca");
    inserirHash("Veneno", "Dona Branca");
    inserirHash("Corda", "Sr. Marinho");
    inserirHash("Pe de Cabra", "Sr. Marinho");

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    
    // Montagem manual do Mapa (Árvore Binária)
    // Estrutura:
    //          Hall
    //        /      \
    //   Cozinha    Biblioteca
    //   /    \      /      \
    // Jardim  Dispensa Escritorio Sótão
    
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *cozinha = criarSala("Cozinha", "Luvas Sujas");
    Sala *biblioteca = criarSala("Biblioteca", "Castical");
    Sala *jardim = criarSala("Jardim", "Corda");
    Sala *dispensa = criarSala("Dispensa", "Veneno");
    Sala *escritorio = criarSala("Escritorio", "Adaga");
    Sala *sotao = criarSala("Sotao", "Pe de Cabra");

    // Conectando as salas
    hall->esquerda = cozinha;
    hall->direita = biblioteca;

    cozinha->esquerda = jardim;
    cozinha->direita = dispensa;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = sotao;

    printf("=== DETECTIVE QUEST: NIVEL MESTRE ===\n");
    printf("Bem-vindo, Detetive. Explore a mansao, colete pistas e descubra o culpado.\n");
    printf("------------------------------------------------------------------------\n");

    // Iniciando a exploração
    explorarSalas(hall, &arvorePistas);

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    // - Exiba a lista de pistas coletadas ao final.
    printf("\n--- FIM DA EXPLORACAO ---\n");
    printf("Pistas coletadas (Ordem Alfabetica):\n");
    if (arvorePistas == NULL) {
        printf("- Nenhuma pista encontrada.\n");
    } else {
        exibirPistasEmOrdem(arvorePistas);
    }

    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    
    printf("\n--- FASE DE ACUSACAO ---\n");
    printf("Suspeitos possiveis: Sr. Mostarda, Dona Branca, Sr. Marinho.\n");
    printf("Quem voce acusa? ");
    
    char acusado[TAM_STRING];
    fgets(acusado, TAM_STRING, stdin);
    acusado[strcspn(acusado, "\n")] = 0; // Remove \n

    // Verifica quantas evidências coletadas apontam para o acusado
    int evidencias = contarEvidenciasContraSuspeito(arvorePistas, acusado);

    printf("\nAnalisando evidencias...\n");
    printf("Pistas contra %s: %d\n", acusado, evidencias);

    if (evidencias >= 2) {
        printf("RESULTADO: PARABENS! Voce encontrou provas suficientes (%d) para prender %s!\n", evidencias, acusado);
        printf("Caso encerrado com sucesso.\n");
    } else {
        printf("RESULTADO: FALHA. Voce nao reuniu provas suficientes (minimo 2) para condenar %s.\n", acusado);
        printf("O culpado escapou!\n");
    }

    // Liberação de memória (simplificada para o escopo do exercício) seria ideal aqui.
    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

// ---------------------------------------------------------
// TABELA HASH (Nível Mestre)
// ---------------------------------------------------------

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// Função de hash simples: soma dos caracteres ASCII % tamanho
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

void inserirHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    
    HashEntry *novo = (HashEntry*) malloc(sizeof(HashEntry));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = NULL;

    // Tratamento de colisão: inserção no início da lista encadeada
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novo;
    } else {
        novo->proximo = tabelaHash[indice];
        tabelaHash[indice] = novo;
    }
}

// Busca quem é o suspeito ligado a uma pista específica
char* buscarSuspeito(char *pista) {
    int indice = funcaoHash(pista);
    HashEntry *atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Pista não cadastrada na base de conhecimento
}

// ---------------------------------------------------------
// ÁRVORE DE BUSCA BINÁRIA - BST (Nível Aventureiro)
// ---------------------------------------------------------

PistaNode* inserirBST(PistaNode *raiz, char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    // Ordenação alfabética
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirBST(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirBST(raiz->direita, texto);
    }
    // Se for igual, não insere duplicata
    return raiz;
}

void exibirPistasEmOrdem(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        
        // Recupera o suspeito associado à pista usando a Hash
        char *suspeito = buscarSuspeito(raiz->texto);
        printf("- Pista: [%s] -> Aponta para: %s\n", raiz->texto, suspeito ? suspeito : "Desconhecido");
        
        exibirPistasEmOrdem(raiz->direita);
    }
}

// Função auxiliar para contar votos na fase final
// Percorre a BST e verifica na Hash se a pista aponta para o acusado
int contarEvidenciasContraSuspeito(PistaNode *raiz, char *suspeitoAcusado) {
    if (raiz == NULL) return 0;

    int contagem = 0;
    
    // Verifica a pista atual
    char *suspeitoReal = buscarSuspeito(raiz->texto);
    if (suspeitoReal != NULL && strcmp(suspeitoReal, suspeitoAcusado) == 0) {
        contagem = 1;
    }

    // Soma com os filhos (recursão)
    return contagem + 
           contarEvidenciasContraSuspeito(raiz->esquerda, suspeitoAcusado) + 
           contarEvidenciasContraSuspeito(raiz->direita, suspeitoAcusado);
}

// ---------------------------------------------------------
// ÁRVORE BINÁRIA DE MAPA (Nível Novato)
// ---------------------------------------------------------

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala *salaAtual, PistaNode **inventarioPistas) {
    if (salaAtual == NULL) {
        printf(">> Caminho bloqueado. Voce voltou.\n");
        return;
    }

    char opcao;
    int explorando = 1;

    while (explorando) {
        printf("\nVoce esta em: [%s]\n", salaAtual->nome);
        
        // Verifica se há pista na sala e se ela já foi pega (lógica simplificada: sempre tenta inserir)
        if (strlen(salaAtual->pista) > 0) {
            printf("(!) Voce encontrou uma pista: %s\n", salaAtual->pista);
            *inventarioPistas = inserirBST(*inventarioPistas, salaAtual->pista);
            // Para não exibir a pista repetidamente, poderíamos limpar a string da sala,
            // mas manteremos para simular que a pista fica lá.
        }

        // Se for folha (sem saídas)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Esta sala nao tem outras saidas. Voce deve voltar.\n");
            return;
        }

        printf("Opcoes: (e)squerda, (d)ireita, (s)air da mansao: ");
        scanf(" %c", &opcao);
        limparBuffer();

        if (opcao == 'e') {
            explorarSalas(salaAtual->esquerda, inventarioPistas);
        } else if (opcao == 'd') {
            explorarSalas(salaAtual->direita, inventarioPistas);
        } else if (opcao == 's') {
            printf("Saindo da mansao...\n");
            explorando = 0; // Quebra o loop para sair
        } else {
            printf("Opcao invalida.\n");
        }
        
        // Se o usuário escolheu 's' dentro da recursão, precisamos propagar a saída
        if (opcao == 's') break; 
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}