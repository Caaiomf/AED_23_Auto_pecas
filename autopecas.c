#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

typedef struct {
    char rua[40], bairro[30];
    int numero;
    char cep[10];
} ENDERECO;

typedef struct {
    int numeroPedido;
    char descricao[50];
    char marca[30];
    int quantidade;
    float valor;
    ENDERECO entrega;
} PEDIDO;

int menu() {
    int opcao;
    printf("\n====== MENU AUTOPECAS ======");
    printf("\n1 - Cadastrar Pedido");
    printf("\n2 - Listar Todos os Pedidos");
    printf("\n3 - Consultar por Descricao");
    printf("\n4 - Listar por Marca");
    printf("\n5 - Alterar Valor ou Quantidade");
    printf("\n6 - Excluir ");
    printf("\n7 - Sair");
    printf("\nOpcao: ");
    scanf("%d", &opcao);
    return opcao;
}

int busca(FILE *arquivo, int numeroPedido) {
    PEDIDO p;
    rewind(arquivo);
    fread(&p, sizeof(PEDIDO), 1, arquivo);
    while (!feof(arquivo) && p.numeroPedido != numeroPedido) {
        fread(&p, sizeof(PEDIDO), 1, arquivo);
    }
    if (!feof(arquivo))
        return (ftell(arquivo) - sizeof(PEDIDO));
    else
        return -1;
}

void cadastrar() {
    FILE *arquivo;
    PEDIDO p;
    int pos;
    arquivo = fopen("autopecas.bin", "ab+");
    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo!");
    else {
        do {
            system("cls");
            printf("\nNumero do Pedido: ");
            scanf("%d", &p.numeroPedido);
            pos = busca(arquivo, p.numeroPedido);
            if (pos == -1) {
                printf("Descricao: ");fflush(stdin);
                gets(p.descricao);
                printf("Marca: ");fflush(stdin);
                gets(p.marca);
                printf("Quantidade: ");
                scanf("%d", &p.quantidade);
                printf("Valor: ");
                scanf("%f", &p.valor);
                printf("Rua: ");fflush(stdin);
                gets(p.entrega.rua);
                printf("Numero: ");
                scanf("%d", &p.entrega.numero);
                printf("Bairro: ");fflush(stdin);
                gets(p.entrega.bairro);
                printf("CEP: ");
                gets(p.entrega.cep);
                fwrite(&p, sizeof(PEDIDO), 1, arquivo);
            } else {
                printf("\nPedido ja cadastrado!");
            }
            printf("\nDeseja continuar (S/N)? ");
        } while (toupper(getche()) == 'S');
        fclose(arquivo);
    }
}

void listar() {
    FILE *arquivo;
    PEDIDO p;
    float total;
    arquivo = fopen("autopecas.bin", "rb");
    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo!");
    else {
        system("cls");
        while (fread(&p, sizeof(PEDIDO), 1, arquivo) == 1) {
            total = p.quantidade * p.valor;
            printf("\nPedido: %d", p.numeroPedido);
            printf("\nDescricao: %s", p.descricao);
            printf("\nMarca: %s", p.marca);
            printf("\nQuantidade: %d", p.quantidade);
            printf("\nValor: %.2f", p.valor);
            printf("\nTotal: %.2f", total);
            printf("\nEntrega: %s, %d, %s, CEP: %s",
                   p.entrega.rua, p.entrega.numero, p.entrega.bairro, p.entrega.cep);
            printf("\n------------------------------------");
        }
        fclose(arquivo);
    }
    system("pause");
    system("cls");
}

void consultarDescricao() {
    FILE *arquivo;
    PEDIDO p;
    char desc[50];
    arquivo = fopen("autopecas.bin", "rb");
    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo!");
    else {
        fflush(stdin);
        printf("\nDescricao para busca: ");
        gets(desc);
        system("cls");
        while (fread(&p, sizeof(PEDIDO), 1, arquivo) == 1) {
            if (stricmp(p.descricao, desc) == 0) {
                printf("\nPedido: %d", p.numeroPedido);
                printf("\nMarca: %s", p.marca);
                printf("\nQuantidade: %d", p.quantidade);
                printf("\nValor: %.2f", p.valor);
                printf("\n--------------------------------");
            }
        }
        fclose(arquivo);
    }
    system("pause");
    system("cls");
}

void listarMarca() {
    FILE *arquivo;
    PEDIDO p;
    char marcaBusca[30];
    arquivo = fopen("autopecas.bin", "rb");
    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo!");
    else {
        fflush(stdin);
        printf("\nMarca para listar: ");
        gets(marcaBusca);
        system("cls");
        while (fread(&p, sizeof(PEDIDO), 1, arquivo) == 1) {
            if (stricmp(p.marca, marcaBusca) == 0) {
                printf("\nPedido: %d", p.numeroPedido);
                printf("\nDescricao: %s", p.descricao);
                printf("\nQuantidade: %d", p.quantidade);
                printf("\nValor: %.2f", p.valor);
                printf("\n--------------------------------");
            }
        }
        fclose(arquivo);
    }
    system("pause");
    system("cls");
}

void alterar() {
    FILE *arquivo;
    PEDIDO p;
    int pos, numero, op;
    arquivo = fopen("autopecas.bin", "rb+");
    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo!");
    else {
        printf("\nNumero do pedido para alterar: ");
        scanf("%d", &numero);
        pos = busca(arquivo, numero);
        if (pos == -1)
            printf("Pedido nao encontrado!");
        else {
            fseek(arquivo, pos, 0);
            fread(&p, sizeof(PEDIDO), 1, arquivo);
            printf("\nPedido: %d", p.numeroPedido);
            printf("\nDescricao: %s", p.descricao);
            printf("\nMarca: %s", p.marca);
            printf("\n1 - Alterar Quantidade");
            printf("\n2 - Alterar Valor");
            printf("\nOpcao: ");
            scanf("%d", &op);
            if (op == 1) {
                printf("Nova Quantidade: ");
                scanf("%d", &p.quantidade);
            } else if (op == 2) {
                printf("Novo Valor: ");
                scanf("%f", &p.valor);
            }
            fseek(arquivo, pos, 0);
            fwrite(&p, sizeof(PEDIDO), 1, arquivo);
            printf("\nDados alterados com sucesso!");
        }
        fclose(arquivo);
    }
    system("pause");
    system("cls");
}
void excluir()
{
	FILE *arquivo;
	PEDIDO p;
	int pos, nPed;
	float total;
	arquivo=fopen("autopecas.bin","rb");
	if(arquivo==NULL)
		printf("Erro no arquivo");
	else
	{
		printf("\nInforme o numero do pedido a ser excluido ");
		scanf("%d",nPed);
		pos=busca(arquivo,nPed);
		if(pos== -1)
			printf("\nRegistro nao encontrado ");
		else
		{
			fseek(arquivo,pos,0);
			fread(&p,sizeof(PEDIDO),1,arquivo);
			total = p.quantidade * p.valor;
            printf("\nPedido: %d", p.numeroPedido);
            printf("\nDescricao: %s", p.descricao);
            printf("\nMarca: %s", p.marca);
            printf("\nQuantidade: %d", p.quantidade);
            printf("\nValor: %.2f", p.valor);
            printf("\nTotal: %.2f",total);
            printf("\nEntrega: %s, %d, %s, CEP: %s",p.entrega.rua, p.entrega.numero, p.entrega.bairro, p.entrega.cep);
            printf("\nDeseja Excluir S/N ?");
            if(toupper(getch())=='S')
            {
            	FILE *temp;
            	temp=fopen("temp.bin","wb");
            	if(temp==NULL)
            		printf("Erro no arquivo ");
            	else
            	{
            		rewind(arquivo);
            		fread(&p,sizeof(PEDIDO),1,arquivo);
            		while(!feof(arquivo))
            		{
            			if(nPed!=p.numeroPedido)
            				fwrite(&p,sizeof(PEDIDO),1,temp);
            			fread(&p,sizeof(PEDIDO),1,arquivo);
            		}
            	}
            	fclose(arquivo);
            	fclose(temp);
            	remove("autopecas.bin");
            	rename("temp.bin","autopecas.bin");
            }
		}
	}
	fclose(arquivo);
}
int main() {
    int opc;
    do {
        opc = menu();
        switch (opc) {
            case 1: cadastrar();
				break;
            case 2: listar(); 
				break;
            case 3: consultarDescricao(); 
				break;
            case 4: listarMarca(); 
				break;
            case 5: alterar(); 
				break;
			case 6: excluir();
				break;
            case 7: printf("Encerrando o sistema...\n"); 
				break;
            default: printf("Opcao invalida!\n");
        }
    } while (opc != 7);
    return 0;
}

