#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<ctype.h>
typedef struct{
	int n,cep;
	char rua[50],bairro[50];
}ENDERECO;
typedef struct{
	int ped,qtd;
	char desc[50],marca[50];
	float valor;
	ENDERECO end;
}PEDIDOS;
int menu()
{
	int op;
	printf("\n1 - Cadastrar compra ");
	printf("\n2 - Listar todos pedidos ");
	printf("\n3 - Consultar descricao ");
	printf("\n4 - Listar pecas por marca ");
	printf("\n5 - Alterar Pedido ");
	printf("\n6 - Sair ");
	scanf("%d",&op);
	return op;	
}
int busca(FILE *compras, int ped)
{
	PEDIDOS info;
	long pos;
	rewind(compras);
	
	while(fread(&info.ped,sizeof(PEDIDOS),1,compras)==1){
		if(info.ped==ped){
			pos=ftell(compras)-sizeof(PEDIDOS);
			return pos;
		}
	}
	return -1;
}
void inserir()
{
	FILE *compras;
	PEDIDOS info;
	long pos;
	compras=fopen("compras.bin","ab+");
	if(compras==NULL)
		printf("Erro no arquivo");
	else
	{
		system("cls");
		printf("\nNumero do pedido: ");fflush(stdin);
		scanf("%d",&info.ped);
		pos=busca(compras,info.ped);
		if(pos==-1)
		{
			printf("\nDescricao: ");fflush(stdin);
			gets(info.desc);
			printf("\nMarca: ");fflush(stdin);
			gets(info.marca);
			printf("\nQuantidade: ");
			scanf("%d",&info.qtd);
			printf("\nValor: ");
			scanf("%d",&info.valor);
			printf("\nEndereco de entrega");
			printf("\nRua: ");fflush(stdin);
			gets(info.end.rua);
			printf("\nNumero");
			scanf("%d",&info.end.n);
			printf("\nBairro ");fflush(stdin);
			gets(info.end.bairro);
			printf("\nCEP ");
			scanf("%d",&info.end.cep);			
		}else{
			fseek(compras,pos,0);
			fread(&info,sizeof(info),1,compras);
			printf("\nO pedido %s esta cadastrado",info.ped);
			printf("\nDeseja cadastrar outro ? S/N ");
		}
	}while(toupper(getch())=='S');
	fclose(compras);
}
