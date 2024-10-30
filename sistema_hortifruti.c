#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PRODUTOS 100
#define MAX_VENDAS 1000

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
    char unidade[20];
} Produto;

typedef struct {
    int codigo;
    char cliente[50];
    float total;
    char data[20];
    int forma_pagamento;
} Venda;

Produto produtos[MAX_PRODUTOS];
Venda vendas[MAX_VENDAS];
int num_produtos = 0;
int num_vendas = 0;

// Funções
void limpar_tela();
void selecionar_usuario();
void menu_principal(int usuario_tipo);
void cadastrar_produto();
void realizar_venda();
void emitir_nota_fiscal(Venda venda);
void relatorio_vendas();
void gerenciar_estoque();
int login_gestor();
void exibir_cabecalho(const char* titulo);

int main() {
    selecionar_usuario();
    return 0;
}

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void selecionar_usuario() {
    int usuario_tipo;
    do {
        limpar_tela();
        printf("=== Escolha a Conta ===\n");
        printf("1. Caixa\n");
        printf("2. Gestor (necessario login)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &usuario_tipo);

        if (usuario_tipo == 2) {
            if (!login_gestor()) {
                printf("Login falhou. Tente novamente.\n");
                continue;
            }
        }
        
        if (usuario_tipo == 0) {
            printf("Saindo do sistema...\n");
            break;
        }

        menu_principal(usuario_tipo);
    } while (usuario_tipo != 0);
}

int login_gestor() {
    char usuario[20], senha[20];
    
    printf("=== LOGIN DO GESTOR ===\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);
    
    if (strcmp(usuario, "gestor") == 0 && strcmp(senha, "1234") == 0) {
        printf("Bem-vindo, Gestor!\n");
        return 1; // Login bem-sucedido
    } else {
        printf("Credenciais inválidas!\n");
        return 0; // Falha no login
    }
}

void exibir_cabecalho(const char* titulo) {
    printf("\n=== %s ===\n", titulo);
}

void menu_principal(int usuario_tipo) {
    int opcao;

    do {
        exibir_cabecalho("SISTEMA DE GESTAO HORTIFRUTI");
        if (usuario_tipo == 2) {
            printf("1. Cadastrar Produto\n");
            printf("2. Gerenciar Estoque\n");
            printf("3. Relatorio de Vendas\n");
        } else {
            printf("1. Realizar Venda\n");
            printf("2. Emitir Nota Fiscal\n");
        }
        printf("0. Voltar ao painel inicial\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (usuario_tipo == 2) {
            switch(opcao) {
                case 1:
                    cadastrar_produto();
                    break; // Retorna ao menu principal após cadastrar produto
                case 2:
                    gerenciar_estoque();
                    break; // Retorna ao menu principal após gerenciar estoque
                case 3:
                    relatorio_vendas();
                    break;
                case 0:
                    return;  // Volta ao painel inicial
                default:
                    printf("Opcao inválida!\n");
            }
        } else {
            switch(opcao) {
                case 1:
                    realizar_venda();
                    break;
                case 2:
                    if (num_vendas > 0) {
                        emitir_nota_fiscal(vendas[num_vendas - 1]);
                    } else {
                        printf("Nenhuma venda registrada para emitir nota fiscal.\n");
                    }
                    break;
                case 0:
                    return;  // Volta ao painel inicial
                default:
                    printf("Opcao inválida!\n");
            }
        }
    } while(opcao != 0);
}

void cadastrar_produto() {
    if (num_produtos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        return;
    }
    
    Produto novo;
    exibir_cabecalho("CADASTRO DE PRODUTO");
    
    novo.codigo = num_produtos + 1;
    printf("Nome do produto: ");
    scanf(" %[^\n]s", novo.nome);
    
    do {
        printf("Preco (positivo): R$ ");
        scanf("%f", &novo.preco);
    } while (novo.preco < 0);
    
    do {
        printf("Quantidade em estoque (positivo): ");
        scanf("%d", &novo.estoque);
    } while (novo.estoque < 0);
    
    printf("Unidade (ex.: kg, unidade): ");
    scanf("%s", novo.unidade);
    
    produtos[num_produtos] = novo;
    num_produtos++;
    
    printf("Produto cadastrado com sucesso!\n");
}

void realizar_venda() {
    if (num_vendas >= MAX_VENDAS) {
        printf("Limite de vendas atingido!\n");
        return;
    }
    
    Venda nova;
    float total = 0;
    char nome_produto[50];
    int quantidade;
    int produto_encontrado;
    
    exibir_cabecalho("NOVA VENDA");
    nova.codigo = num_vendas + 1;
    
    printf("Nome do cliente: ");
    scanf(" %[^\n]s", nova.cliente);
    
    do {
        printf("\nNome do produto (digite 'fim' para finalizar): ");
        scanf(" %[^\n]s", nome_produto);
        
        if (strcmp(nome_produto, "fim") == 0) {
            break;
        }

        produto_encontrado = -1;
        for (int i = 0; i < num_produtos; i++) {
            if (strcmp(produtos[i].nome, nome_produto) == 0) {
                produto_encontrado = i;
                break;
            }
        }
        
        if (produto_encontrado != -1) {
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            
            if (quantidade <= produtos[produto_encontrado].estoque) {
                total += produtos[produto_encontrado].preco * quantidade;
                produtos[produto_encontrado].estoque -= quantidade;
            } else {
                printf("Estoque insuficiente!\n");
            }
        } else {
            printf("Produto nao encontrado!\n");
        }
    } while (1);
    
    nova.total = total;
    
    printf("\nForma de pagamento:\n");
    printf("1. Dinheiro\n");
    printf("2. Cartao\n");
    printf("3. Pix\n");
    scanf("%d", &nova.forma_pagamento);
    
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(nova.data, sizeof(nova.data), "%d/%m/%Y", tm);
    
    vendas[num_vendas] = nova;
    num_vendas++;
    
    emitir_nota_fiscal(nova);
}

void emitir_nota_fiscal(Venda venda) {
    exibir_cabecalho("NOTA FISCAL");
    printf("Codigo da venda: %d\n", venda.codigo);
    printf("Data: %s\n", venda.data);
    printf("Cliente: %s\n", venda.cliente);
    printf("Total: R$ %.2f\n", venda.total);
    printf("Forma de pagamento: ");
    
    switch(venda.forma_pagamento) {
        case 1:
            printf("Dinheiro\n");
            break;
        case 2:
            printf("Cartao\n");
            break;
        case 3:
            printf("Pix\n");
            break;
    }
}

void relatorio_vendas() {
    float total_periodo = 0;
    int vendas_dinheiro = 0, vendas_cartao = 0, vendas_pix = 0;
    
    exibir_cabecalho("RELATORIO DE VENDAS");

    if (num_vendas == 0) {
        printf("Nenhuma venda registrada até o momento.\n");
        return;
    }
    
    for (int i = 0; i < num_vendas; i++) {
        total_periodo += vendas[i].total;
        
        switch(vendas[i].forma_pagamento) {
            case 1: vendas_dinheiro++; break;
            case 2: vendas_cartao++; break;
            case 3: vendas_pix++; break;
        }
    }
    
    printf("Total de vendas: R$ %.2f\n", total_periodo);
    printf("Dinheiro: %d vendas\n", vendas_dinheiro);
    printf("Cartao: %d vendas\n", vendas_cartao);
    printf("Pix: %d vendas\n", vendas_pix);
}

void gerenciar_estoque() {
    int opcao_estoque, codigo, quantidade;

    exibir_cabecalho("GERENCIAR ESTOQUE");
    printf("Produtos cadastrados:\n");
    
    for (int i = 0; i < num_produtos; i++) {
        printf("Codigo: %d | Produto: %s | Estoque: %d | Unidade: %s | Preco: R$ %.2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].estoque, produtos[i].unidade, produtos[i].preco);
    }

    printf("\n1. Atualizar estoque\n");
    printf("0. Voltar ao painel inicial\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao_estoque);

    if (opcao_estoque == 1) {
        printf("Codigo do produto para atualizar: ");
        scanf("%d", &codigo);
        printf("Quantidade para adicionar (ou subtrair): ");
        scanf("%d", &quantidade);

        if (codigo > 0 && codigo <= num_produtos) {
            produtos[codigo - 1].estoque += quantidade;
            printf("Estoque atualizado com sucesso!\n");
        } else {
            printf("Codigo inválido!\n");
        }
    }
}
