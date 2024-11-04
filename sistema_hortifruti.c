#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

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

void limpar_tela();
void exibir_cabecalho(const char* titulo);
int buscar_produto_por_nome(const char* nome);

void selecionar_usuario();
void menu_principal(int usuario_tipo);
void cadastrar_produto();
void realizar_venda();
void emitir_nota_fiscal(Venda venda);
void relatorio_vendas();
void gerenciar_estoque();

int login_administrador();
int login_caixa();

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_ACP);

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

void exibir_cabecalho(const char* titulo) {
    limpar_tela();
    printf("\n=== %s ===\n", titulo);
}

int buscar_produto_por_nome(const char* nome) {
    for (int i = 0; i < num_produtos; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void selecionar_usuario() {
    int usuario_tipo;
    do {
        limpar_tela();
        printf("=== SISTEMA DE GESTÃO HORTIFRUTI ===\n\n");
        printf("1. Caixa (necessário login)\n");
        printf("2. Administrador (necessário login)\n");
        printf("0. Sair\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &usuario_tipo);
        getchar();

        if (usuario_tipo == 1) {
            if (!login_caixa()) {
                printf("Login falhou. Pressione ENTER para continuar...");
                getchar();
                continue;
            }
        } else if (usuario_tipo == 2) {
            if (!login_administrador()) {
                printf("Login falhou. Pressione ENTER para continuar...");
                getchar();
                continue;
            }
        } else if (usuario_tipo == 0) {
            printf("Saindo do sistema...\n");
            Sleep(1500);
            break;
        } else {
            printf("Opção inválida! Pressione ENTER para continuar...");
            getchar();
            continue;
        }

        menu_principal(usuario_tipo);
    } while (usuario_tipo != 0);
}

int login_administrador() {
    char usuario[20], senha[20];
    int tentativas = 0;
    
    limpar_tela();
    printf("=== LOGIN DO ADMINISTRADOR ===\n\n");
    
    while (tentativas < 3) {
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);
        getchar();
        
        if (strcmp(usuario, "administrador") == 0 && strcmp(senha, "1234") == 0) {
            printf("\nBem-vindo, ADMINISTRADOR!\n");
            Sleep(1500);
            return 1;
        } else {
            tentativas++;
            printf("\nCredenciais inválidas! Você tem %d tentativas restantes.\n\n", 3 - tentativas);
            Sleep(1500);
        }
    }
    
    printf("Número máximo de tentativas atingido! Acesso bloqueado.\n");
    Sleep(2000);
    return 0;
}

int login_caixa() {
    char usuario[20], senha[20];
    int tentativas = 0;
    
    limpar_tela();
    printf("=== LOGIN DO CAIXA ===\n\n");
    
    while (tentativas < 3) {
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);
        getchar();
        
        if (strcmp(usuario, "caixa") == 0 && strcmp(senha, "1234") == 0) {
            printf("\nBem-vindo, Caixa!\n");
            Sleep(1500);
            return 1;
        } else {
            tentativas++;
            printf("\nCredenciais inválidas! Você tem %d tentativas restantes.\n\n", 3 - tentativas);
            Sleep(1500);
        }
    }
    
    printf("Número máximo de tentativas atingido! Acesso bloqueado.\n");
    Sleep(2000);
    return 0;
}

void menu_principal(int usuario_tipo) {
    int opcao;

    do {
        exibir_cabecalho("SISTEMA DE GESTÃO HORTIFRUTI");

        if (usuario_tipo == 2) {
            printf("\n1. Cadastrar Produto\n");
            printf("2. Gerenciar Estoque\n");
            printf("3. Relatório de Vendas\n");
        } else { 
            printf("\n1. Realizar Venda\n");
            printf("2. Emitir Última Nota Fiscal\n");
        }
        
        printf("0. Voltar ao painel inicial\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 

        if (usuario_tipo == 2) {
            switch(opcao) {
                case 1:
                    cadastrar_produto();
                    break;
                case 2:
                    gerenciar_estoque();
                    break;
                case 3:
                    relatorio_vendas();
                    break;
                case 0:
                    return;
                default:
                    printf("Opção inválida!\n");
                    Sleep(1500);
            }
        } else {
            switch(opcao) {
                case 1:
                    realizar_venda();
                    break;
                case 2:
                    if (num_vendas > 0) {
                        limpar_tela();
                        emitir_nota_fiscal(vendas[num_vendas - 1]);
                    } else {
                        printf("Nenhuma venda registrada para emitir nota fiscal.\n");
                        Sleep(1500);
                    }
                    break;
                case 0:
                    return;
                default:
                    printf("Opção inválida!\n");
                    Sleep(1500);
            }
        }
    } while(opcao != 0);
}

void cadastrar_produto() {
    if (num_produtos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        Sleep(1500);
        return;
    }
    
    Produto novo;
    exibir_cabecalho("CADASTRO DE PRODUTO");
    
    novo.codigo = num_produtos + 1;
    
    printf("\nNome do produto: ");
    scanf(" %[^\n]s", novo.nome);
    getchar();
    
    do {
        printf("Preço (positivo): R$ ");
        scanf("%f", &novo.preco);
        getchar();
        
        if (novo.preco <= 0) {
            printf("O preço deve ser positivo!\n");
        }
    } while (novo.preco <= 0);
    
    do {
        printf("Quantidade em estoque (positivo): ");
        scanf("%d", &novo.estoque);
        getchar();
        
        if (novo.estoque < 0) {
            printf("A quantidade deve ser positiva!\n");
        }
    } while (novo.estoque < 0);
    
    printf("Unidade (ex.: kg, unidade): ");
    scanf("%s", novo.unidade);
    getchar();
    
    produtos[num_produtos] = novo;
    num_produtos++;
    
    printf("\nProduto cadastrado com sucesso!\n");
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void realizar_venda() {
    if (num_vendas >= MAX_VENDAS) {
        printf("Limite de vendas atingido!\n");
        Sleep(1500);
        return;
    }
    
    Venda nova;
    float total = 0;
    char nome_produto[50];
    int quantidade;
    int produto_encontrado;
    
    exibir_cabecalho("NOVA VENDA");
    nova.codigo = num_vendas + 1;
    
    printf("\nNome do cliente: ");
    scanf(" %[^\n]s", nova.cliente);
    getchar();
    
    do {
        printf("\nNome do produto (digite 'fim' para finalizar): ");
        scanf(" %[^\n]s", nome_produto);
        getchar();
        
        if (strcmp(nome_produto, "fim") == 0) {
            break;
        }

        produto_encontrado = buscar_produto_por_nome(nome_produto);
        
        if (produto_encontrado != -1) {
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            getchar();
            
            if (quantidade <= 0) {
                printf("Quantidade inválida!\n");
                continue;
            }
            
            if (quantidade <= produtos[produto_encontrado].estoque) {
                float subtotal = produtos[produto_encontrado].preco * quantidade;
                total += subtotal;
                produtos[produto_encontrado].estoque -= quantidade;
                printf("Subtotal: R$ %.2f\n", subtotal);
            } else {
                printf("Estoque insuficiente! Disponível: %d %s\n", 
                       produtos[produto_encontrado].estoque,
                       produtos[produto_encontrado].unidade);
            }
        } else {
            printf("Produto não encontrado!\n");
            printf("1. Continuar a venda\n");
            printf("2. Voltar para a tela anterior\n");
            printf("Escolha uma opção: ");
            int opcao;
            scanf("%d", &opcao);
            getchar();

            if (opcao == 2) {
                return;
            }
        }
    } while (1);
    
    if (total > 0) {
        nova.total = total;
        
        printf("\nTotal da venda: R$ %.2f\n", total);
        printf("\nForma de pagamento:\n");
        printf("1. Dinheiro\n");
        printf("2. Cartão\n");
        printf("3. Pix\n");
        printf("Escolha uma opção: ");
        scanf("%d", &nova.forma_pagamento);
        getchar();
        
        while (nova.forma_pagamento < 1 || nova.forma_pagamento > 3) {
            printf("Opção de pagamento inválida. Tente novamente: ");
            scanf("%d", &nova.forma_pagamento);
            getchar();
        }
        
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(nova.data, sizeof(nova.data), "%d/%m/%Y", tm);
        
        vendas[num_vendas] = nova;
        num_vendas++;
        
        printf("\nVenda realizada com sucesso!\n");
        printf("\nPressione ENTER para ver a nota fiscal...");
        getchar();
        
        emitir_nota_fiscal(nova);
    } else {
        printf("\nNenhum produto foi vendido. A venda não pode ser finalizada.\n");
        printf("\nPressione ENTER para continuar...");
        getchar();
    }
}

void emitir_nota_fiscal(Venda venda) {
    limpar_tela();
    printf("\n=====================================\n");
    printf("            NOTA FISCAL             \n");
    printf("=====================================\n");
    printf("Código da venda: %d\n", venda.codigo);
    printf("Cliente: %s\n", venda.cliente);
    printf("Total: R$ %.2f\n", venda.total);
    printf("Data: %s\n", venda.data);
    
    char* formas_pagamento[] = {"", "Dinheiro", "Cartão", "Pix"};
    printf("Forma de pagamento: %s\n", formas_pagamento[venda.forma_pagamento]);
    
    printf("=====================================\n");
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void relatorio_vendas() {
    float total_geral = 0;
    int total_vendas = 0;
    
    exibir_cabecalho("RELATÓRIO DE VENDAS");
    
    if (num_vendas == 0) {
        printf("\nNenhuma venda registrada.\n");
    } else {
        printf("\nListagem de todas as vendas:\n\n");
        
        for (int i = 0; i < num_vendas; i++) {
            printf("Venda %d:\n", vendas[i].codigo);
            printf("  Cliente: %s\n", vendas[i].cliente);
            printf("  Total: R$ %.2f\n", vendas[i].total);
            printf("  Data: %s\n", vendas[i].data);
            printf("  Forma de pagamento: %s\n", 
                   vendas[i].forma_pagamento == 1 ? "Dinheiro" :
                   vendas[i].forma_pagamento == 2 ? "Cartão" : "Pix");
            printf("-------------------------------------\n");
            
            total_geral += vendas[i].total;
            total_vendas++;
        }
        
        printf("\nResumo:\n");
        printf("Total de vendas: %d\n", total_vendas);
        printf("Valor total: R$ %.2f\n", total_geral);
        printf("Ticket médio: R$ %.2f\n", total_geral / total_vendas);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void gerenciar_estoque() {
    exibir_cabecalho("GERENCIAR ESTOQUE");
    for (int i = 0; i < num_produtos; i++) {
        printf("Produto %d: Nome: %s, Preço: R$ %.2f, Estoque: %d %s\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque, produtos[i].unidade);
    }
    if (num_produtos == 0) {
        printf("Nenhum produto cadastrado.\n");
    }
    getchar();
}
