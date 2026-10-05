/* ============================================================================
    EP1 - Planilha Esparsa com Histórico de Alterações
    TEMPLATE - preencha os TODOs abaixo. Não altere assinaturas de função,
    nomes ou ordem de campos de struct.
 
    Uso: ./ep_XXXX arquivo_entrada.txt arquivo_saida.txt
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
  Estruturas de dados (não altere nomes e ordem de campos)
 ---------------------------------------------------------------------- */
 
 
unsigned int overflow = 2;

typedef struct celula {
    int linha;
    int coluna;
    int valor;
    struct celula *proxima_linha;
    struct celula *proxima_coluna;
} celula_t;

typedef struct fileira {
    int indice; //num da fileira em questão
    celula_t* primeiro; //ponteiro para a primeira célula 
    struct fileira *proximo; //próxima fielira
} fileira_t;

typedef struct {
    bool transposicao;
    int linha;
    int coluna;
    union {
        int valor_anterior;
        int tamanho;
    };
} operacao_t;

typedef struct elo_pilha {
    operacao_t op;
    struct elo_pilha *proximo;
} elo_pilha_t;

typedef struct {
    elo_pilha_t* topo;
} pilha_t;

typedef struct {
    fileira_t* primeira_linha;
    fileira_t* primeira_coluna;
    int total_celulas;
    pilha_t historico;
} planilha_t;

/* ---------------------------------------------------------------------- *
    Funções auxiliar para leitura das strings dos comandos
 ---------------------------------------------------------------------- */

int igual(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

/* ---------------------------------------------------------------------- *
    Funções obrigatórias (não altere as assinaturas)
 ---------------------------------------------------------------------- */

void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo

	p->primeira_linha = NULL;
	p->primeira_coluna = NULL;
	p->total_celulas = 0;
	p->historico.topo = NULL;
	
	
    for(int i = 0; i < (sizeof(int) * 8) - 2; i++) 
    {
        overflow *= 2;
    }
    
    overflow = overflow + (overflow - 1); //maior inteiro possivel 


}

celula_t* buscar_celula(planilha_t *p, int lin, int col,
                      celula_t** cel_ant_linha, celula_t** cel_ant_coluna,
                      fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
        por referencia, retorne NULL se a celula nao existir */


    *fil_ant_linha = NULL;
    *fil_ant_coluna = NULL;
    *cel_ant_linha = NULL;
    *cel_ant_coluna = NULL;
    
    celula_t* celulinha;
    
    if(p->primeira_linha == NULL || p->primeira_coluna == NULL)
    {
        return NULL; //pois a matriz nn tem nenhum valor
    }
    
    //achar celAntLinha e fil_ant_linha
    
    fileira_t* LinhaAlvo = p->primeira_linha; //é diferente de null pois a matriz existe
    while(LinhaAlvo != NULL && LinhaAlvo->indice < lin)
    {
        (*fil_ant_linha) = LinhaAlvo;
        LinhaAlvo = LinhaAlvo->proximo;
    }//terminou ou: com a linha alvo nn existindo(ou seja, acabou a matriz antes), ou com a linha alvo sendo igual ou maior à lin
    if(LinhaAlvo == NULL)
    {
        *cel_ant_linha = NULL;
        celulinha = NULL;
    }
    else
    {
        if(LinhaAlvo->indice == lin)
        {
            *cel_ant_linha = NULL;
            celulinha = LinhaAlvo->primeiro;
            while(celulinha != NULL && celulinha->coluna < col)
            {//Analise horizontal
                *cel_ant_linha = celulinha;
                celulinha = celulinha->proxima_linha; //horizontalmente
                
            }
        }
        else //linha alvo deu um salto, somente sendo possivel se a celulinha nn existir e nem toda a linha da celulinha alvo
        {
            *cel_ant_linha = NULL;
        }
    }
    
    
    
    //achar celAntCol e fil_ant_coluna
    
    fileira_t* ColunaAlvo = p->primeira_coluna; //é diferente de null pois a matriz existe
    while(ColunaAlvo != NULL && ColunaAlvo->indice < col)
    {
        (*fil_ant_coluna) = ColunaAlvo;
        ColunaAlvo = ColunaAlvo->proximo;
    }//terminou ou: com a linha alvo nn existindo(ou seja, acabou a matriz antes), ou com a linha alvo sendo igual ou maior à lin
    if(ColunaAlvo == NULL)
    {
        *cel_ant_coluna = NULL;
        celulinha = NULL;
    }
    else
    {
        if(ColunaAlvo->indice == col)
        {
            *cel_ant_coluna = NULL;
            celulinha = ColunaAlvo->primeiro;
            
            
            while(celulinha != NULL && celulinha->linha < lin)
            {//Analise vertical
                *cel_ant_coluna = celulinha;
                celulinha = celulinha->proxima_coluna; //vertical
                //printf("%d %d\n", celulinha->linha, celulinha->coluna);
                
            }
            
        }
        else //linha alvo deu um salto, somente sendo possivel se a celulinha nn existir e nem toda a linha da celulinha alvo
        {
            *cel_ant_coluna = NULL;
        }
    }
    
    if(celulinha != NULL)
    {
        if(celulinha->coluna != col || celulinha->linha != lin) celulinha = NULL;
    }
    
    
    return celulinha;
	
	

}

int obter_valor(planilha_t *p, int linha, int coluna) {
        
    celula_t* cel_ant_linha;
    celula_t* cel_ant_coluna;
    celula_t* Celular;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;
    Celular = buscar_celula(p, linha, coluna, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
    
    if(Celular != NULL)
    {
        return Celular->valor;
    }
    return 0;
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) {
    int soma = 0;
    
    for(int i = linha_ini; i <= linha_fim; i++)
    {
        for(int j = coluna_ini; j <= coluna_fim; j++)
        {
            soma += obter_valor(p, i, j);
        }
    }
    return soma;
}

int contar_nao_nulas(planilha_t* p) {
    // TODO: retorne a quantidade de celulas nao nulas
    
    return p->total_celulas;
}
bool remover_celula(planilha_t* p, int lin, int col);

bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
       Retorna true se houve alteracao, false se foi operacao nula. */
    
    celula_t* cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;
    
    celula_t* celula = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna,
                                    &fil_ant_linha, &fil_ant_coluna);
    
    if(celula != NULL)
    {
        if(valor != 0)//cell existe e vou trocar por valor diferente de zero
        {
            if(valor == celula->valor)
            {
                return false;
            }
            else
            {//to mudando de valor algo q ja existe
                elo_pilha_t* mudanca = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
                mudanca->op.transposicao = 0;
                mudanca->op.linha = lin;
                mudanca->op.coluna = col;
                mudanca->op.valor_anterior = celula->valor;
                        
                mudanca->proximo = p->historico.topo;
                p->historico.topo = mudanca;
                
                
                celula->valor = valor;
            }
        }
        else
        {
            remover_celula(p, lin, col);
            return true;
        }
    }
    else
    {
        
        if(valor != 0)
        { //era zero e to pondo algum valor
        
            elo_pilha_t* mudanca = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
            mudanca->op.transposicao = 0;
            mudanca->op.linha = lin;
            mudanca->op.coluna = col;
            mudanca->op.valor_anterior = 0;
            
            mudanca->proximo = p->historico.topo;
            p->historico.topo = mudanca;
            
            p->total_celulas++;
                
                
            celula = (celula_t*)malloc(sizeof(celula_t));
            celula->linha = lin;
            celula->coluna = col;
            celula->valor = valor;
            celula->proxima_coluna = NULL;
            celula->proxima_linha = NULL;
            
            if(cel_ant_coluna != NULL) //celula anterior da mesma coluna
            {
                
                celula->proxima_coluna = cel_ant_coluna->proxima_coluna;
                cel_ant_coluna->proxima_coluna = celula;
            }
            else
            {   
                if(fil_ant_coluna != NULL) //pois eu vou tentar acessar a coluna alvo, e eu ja tenho de bandeija o fil_ant_coluna
                {
                    
                    if(fil_ant_coluna->proximo != NULL) //q se for o q eu procuro, é o q me interessa
                    {
                        if(fil_ant_coluna->proximo->indice > col)
                        {// vou ter q criar uma fileira q fica entre fil ant, e o proximo de fil ant
                            fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                            NovaColuna->indice = col;
                            NovaColuna->primeiro = celula;
                            NovaColuna->proximo = fil_ant_coluna->proximo;
                            fil_ant_coluna->proximo = NovaColuna;
                            
                        }
                        if(fil_ant_coluna->proximo->indice == col)
                        {//é o q eu quero, e eu ja sei q nn tem anteriores na coluna
                            celula->proxima_coluna = fil_ant_coluna->proximo->primeiro; //tudo bem ser null
                            fil_ant_coluna->proximo->primeiro = celula;
                            
                        }
                        if(fil_ant_coluna->proximo->indice < col)
                        {
                            //nn vejo sentido esse caso, pois se a busca me retorna uma coluna anterior, e a proxima coluna em relação a anterior existe e é menor do que a minha coluna alvo, teoricamente essa nova coluna seria a minha anterior
                        }
                    }
                    else // a coluna anterior nn aponta para ngm
                    {
                        fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                        NovaColuna->indice = col;
                        NovaColuna->primeiro = celula;
                        NovaColuna->proximo = NULL;
                        fil_ant_coluna->proximo = NovaColuna;
                    }
                }
                else//se nn tem uma coluna anterior, ent ou a coluna q eu quero é a primeira da planilha_t, ou nn tem a coluna q eu quero
                {
                    
                    
                    if(p->primeira_coluna != NULL)
                    {
                        
                        
                        if(p->primeira_coluna->indice < col)
                        {
                            //fil_ant_coluna == NULL
                            if(p->primeira_coluna->proximo != NULL) //só ter mais uma chance de realmente nn existir a coluna q eu to procurando
                            {
                                /*fileira_t* Andadora;
                                for(Andadora = p->primeira_coluna->proximo; Andadora != NULL && Andadora->indice >= col; Andadora = Andadora->proximo);
                                if(Andadora->indice == col)
                                    {//ja q nn existe cel ant coluna, e a primeira coluna da lista é a meu alvo
                                        celula->proxima_coluna = Andadora->primeiro;
                                        Andadora->primeiro = celula;
                                    }
                                if(Andadora->indice > col)
                                    {
                                        fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                                        NovaColuna->indice = col;
                                        NovaColuna->primeiro = celula;
                                        NovaColuna->proximo = Andadora;
                                        Andadora = NovaColuna;
                                    }
                                */
                            }
                            else //realmente nn tem a coluna q eu to procurando pq é uma matriz de somente uma coluna
                            {
                                fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                                NovaColuna->indice = col;
                                NovaColuna->primeiro = celula;
                                p->primeira_coluna->proximo = NovaColuna;
                                NovaColuna->proximo = NULL;
                            }
                        }
                        if(p->primeira_coluna->indice == col)
                        {//ja q nn existe cel ant coluna, e a primeira coluna da lista é a meu alvo
                            celula->proxima_coluna = p->primeira_coluna->primeiro;
                            p->primeira_coluna->primeiro = celula;
                        }
                        if(p->primeira_coluna->indice > col)
                        {
                            fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                            NovaColuna->indice = col;
                            NovaColuna->primeiro = celula;
                            NovaColuna->proximo = p->primeira_coluna;
                            p->primeira_coluna = NovaColuna;
                        }
                    }
                    else // a matriz era vazia
                    {
                        
                        fileira_t* NovaColuna = (fileira_t*) malloc(sizeof(fileira_t));
                        NovaColuna->indice = col;
                        NovaColuna->primeiro = celula;
                        NovaColuna->proximo = NULL;
                        p->primeira_coluna = NovaColuna;
                    }
                }
                
            }
            
            ///////////AGR PARA linhas
            if(cel_ant_linha != NULL) //celula anterior da mesma coluna
            {
                celula->proxima_linha = cel_ant_linha->proxima_linha;
                cel_ant_linha->proxima_linha = celula;
            }
            else
            {   
                if(fil_ant_linha != NULL) //pois eu vou tentar acessar a coluna alvo, e eu ja tenho de bandeija o fil_ant_coluna
                {
                    if(fil_ant_linha->proximo != NULL) //q se for o q eu procuro, é o q me interessa
                    {
                        if(fil_ant_linha->proximo->indice > lin)
                        {// vou ter q criar uma fileira q fica entre fil ant, e o proximo de fil ant
                            fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                            NovaLinha->indice = lin;
                            NovaLinha->primeiro = celula;
                            NovaLinha->proximo = fil_ant_linha->proximo;
                            fil_ant_linha->proximo = NovaLinha;
                        } else if(fil_ant_linha->proximo->indice == lin)
                        {//é o q eu quero, e eu ja sei q nn tem anteriores na coluna
                            celula->proxima_linha = fil_ant_linha->proximo->primeiro; //tudo bem ser null
                            fil_ant_linha->proximo->primeiro = celula;
                            
                        } else if(fil_ant_linha->proximo->indice < lin)
                        {
                            //nn vejo sentido esse caso, pois se a busca me retorna uma coluna anterior, e a proxima coluna em relação a anterior existe e é menor do que a minha coluna alvo, teoricamente essa nova coluna seria a minha anterior
                        }
                    }
                    else // a coluna anterior nn aponta para ngm
                    {
                        fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                        NovaLinha->indice = lin;
                        NovaLinha->primeiro = celula;
                        NovaLinha->proximo = NULL;
                        fil_ant_linha->proximo = NovaLinha;
                    }
                }
                else//se nn tem uma coluna anterior, ent ou a coluna q eu quero é a primeira da planilha_t, ou nn tem a coluna q eu quero
                {
                    
                    if(p->primeira_linha != NULL)
                    {
                        if(p->primeira_linha->indice < lin)
                        {
                            //fil_ant_coluna == NULL
                            if(p->primeira_linha->proximo != NULL) //só ter mais uma chance de realmente nn existir a coluna q eu to procurando
                            {
                                /*fileira_t* Andadora;
                                for(Andadora = p->primeira_linha->proximo; Andadora != NULL && Andadora->indice >= lin; Andadora = Andadora->proximo);
                                if(Andadora->indice == lin)
                                    {//ja q nn existe cel ant coluna, e a primeira coluna da lista é a meu alvo
                                        celula->proxima_linha = Andadora->primeiro;
                                        Andadora->primeiro = celula;
                                    }
                                if(Andadora->indice > lin)
                                    {
                                        fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                                        NovaLinha->indice = lin;
                                        NovaLinha->primeiro = celula;
                                        NovaLinha->proximo = Andadora;
                                        Andadora = NovaLinha;
                                    }
                                */
                            }
                            else //realmente nn tem a coluna q eu to procurando pq é uma matriz de somente uma coluna
                            {
                                fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                                NovaLinha->indice = lin;
                                NovaLinha->primeiro = celula;
                                p->primeira_linha->proximo = NovaLinha;
                                NovaLinha->proximo = NULL;
                            }
                        }else if(p->primeira_linha->indice == lin)
                        {//ja q nn existe cel ant coluna, e a primeira coluna da lista é a meu alvo
                            celula->proxima_linha = p->primeira_linha->primeiro;
                            p->primeira_linha->primeiro = celula;
                        } else if(p->primeira_linha->indice > lin)
                        {
                            fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                            NovaLinha->indice = lin;
                            NovaLinha->primeiro = celula;
                            NovaLinha->proximo = p->primeira_linha;
                            p->primeira_linha = NovaLinha;
                        }
                    }
                    else // a matriz era vazia
                    {
                        
                        fileira_t* NovaLinha = (fileira_t*) malloc(sizeof(fileira_t));
                        NovaLinha->indice = lin;
                        NovaLinha->primeiro = celula;
                        NovaLinha->proximo = NULL;
                        p->primeira_linha = NovaLinha;
                    }
                }
                
            }
        }
        else
        {
            return false;
        }
    }
    
    return true;
}

bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)
    
    celula_t* cel_ant_linha;
    celula_t* cel_ant_coluna;
    fileira_t* fil_ant_linha;
    fileira_t* fil_ant_coluna;
    
    celula_t* celula = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna,
                                    &fil_ant_linha, &fil_ant_coluna);

    celula_t AuxCelula;

    if(celula != NULL) //ela existe
    {
        AuxCelula = *celula;
        if(cel_ant_linha != NULL) //qm aponta pra qm quero tirar, agr apontar para qm o que quero tirar apontava
        {
            
            cel_ant_linha->proxima_linha = celula->proxima_linha;
        } //logo ele era o primeiro de sua linha
        else
        {
            if(fil_ant_linha != NULL)//conseguir acessar a linha do que eu queria
            {
                fil_ant_linha->proximo->primeiro = celula->proxima_linha;
            }
            else //se nn tinha o anterior, e sabendo q a celula existia, ent a sua linha era a primeira linha
            {
                p->primeira_linha->primeiro = celula->proxima_linha;
            }
        }
        //Agr para colunas
        if(cel_ant_coluna != NULL) //qm aponta pra qm quero tirar, agr apontar para qm o que quero tirar apontava
        {
            cel_ant_coluna->proxima_coluna = celula->proxima_coluna;
        } //logo ele era o primeiro de sua linha
        else
        {
            if(fil_ant_coluna != NULL)//conseguir acessar a linha do que eu queria
            {
                fil_ant_coluna->proximo->primeiro = celula->proxima_coluna;
            }
            else //se nn tinha o anterior, e sabendo q a celula existia, ent a sua linha era a primeira linha
            {
                p->primeira_coluna->primeiro = celula->proxima_coluna;
            }
        }
        //ponteiros ja alterados
        free(celula);
        celula = NULL;
        
        
    }
    else //nn existe
    {
        return false;
    }

    //remover fileiras vazias
    if(fil_ant_coluna != NULL)
    {
        if(fil_ant_coluna->proximo->primeiro == NULL) //se a coluna q estava a celula q eu tirei nn tive nenhum element
        {
            fileira_t* AuxColuna = fil_ant_coluna->proximo;
            fil_ant_coluna->proximo = fil_ant_coluna->proximo->proximo;
            free(AuxColuna);
            AuxColuna = NULL;
        }
        else //tem algum elemento nessa coluna, ent nn faz nada
        {
            
        }
    }
    else //a coluna em questão é a primeira
    {
       if(p->primeira_coluna->primeiro == NULL)
       {
           fileira_t* AuxColuna = p->primeira_coluna;
           p->primeira_coluna = p->primeira_coluna->proximo;
           free(AuxColuna);
           AuxColuna = NULL;
       }
       else
       {
           
       }
    }
    
    //Agr Remove as colunas
    //remover fileiras vazias
    if(fil_ant_linha != NULL)
    {
        if(fil_ant_linha->proximo->primeiro == NULL) //se a coluna q estava a celula q eu tirei nn tive nenhum element
        {
            fileira_t* AuxLinha = fil_ant_linha->proximo;
            fil_ant_linha->proximo = fil_ant_linha->proximo->proximo;
            free(AuxLinha);
            AuxLinha = NULL;
        }
        else //tem algum elemento nessa coluna, ent nn faz nada
        {
            
        }
    }
    else //a coluna em questão é a primeira
    {
       if(p->primeira_linha->primeiro == NULL)
       {
           fileira_t* AuxLinha = p->primeira_linha;
           p->primeira_linha = p->primeira_linha->proximo;
           free(AuxLinha);
           AuxLinha = NULL;
       }
       else
       {
           
       }
    }
    
    elo_pilha_t* mudanca = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
    mudanca->op.transposicao = 0;
    mudanca->op.linha = lin;
    mudanca->op.coluna = col;
    mudanca->op.valor_anterior = AuxCelula.valor;
            
    mudanca->proximo = p->historico.topo;
    p->historico.topo = mudanca;
    
    p->total_celulas--;
    return true;
}


bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
       as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */
       
        if(lin >= overflow - tamanho || col >= overflow - tamanho)
        {
        return false;
        }    
       
        bool mudou = false;
        
        int Aux1;
        int Aux2;
        elo_pilha_t* temp;
        for(int j = 0; j < tamanho - 1; j++)
        {
            for(int i = 1; i < tamanho - j; i++)
            {
                Aux1 = obter_valor(p, lin+j, col+j + i);
                Aux2 = obter_valor(p, lin+j + i, col+j);
                if(Aux1 != Aux2)
                {
                    
                    mudou = true;
                    if(Aux1 != 0)
                    {
                        
                        if(Aux2 != 0)
                        {
                            definir_celula(p, lin+j, col+j + i, Aux2);
                            temp = p->historico.topo->proximo;
                            free(p->historico.topo); //nn vazio pois o definir gerou
                            p->historico.topo = temp;
                            
                            definir_celula(p, lin+j + i, col+j, Aux1);
                            temp = p->historico.topo->proximo;
                            free(p->historico.topo); //nn vazio pois o definir gerou
                            p->historico.topo = temp;
                        }
                        else
                        {
                            
                            remover_celula(p, lin+j, col+j + i);
                            temp = p->historico.topo->proximo;
                            free(p->historico.topo); //nn vazio pois o definir gerou
                            p->historico.topo = temp;
                            
                            definir_celula(p, lin+j + i, col+j, Aux1);
                            temp = p->historico.topo->proximo;
                            free(p->historico.topo); //nn vazio pois o definir gerou
                            p->historico.topo = temp;
                        }
                    }
                    else
                    {
                        definir_celula(p, lin+j, col+j + i, Aux2);
                        temp = p->historico.topo->proximo;
                        free(p->historico.topo); //nn vazio pois o definir gerou
                        p->historico.topo = temp;
                        
                        remover_celula(p, lin+j + i, col+j);
                        temp = p->historico.topo->proximo;
                        free(p->historico.topo); //nn vazio pois o definir gerou
                        p->historico.topo = temp;
                    }
                }
                else
                {
                    
                }
            }
        }   
        
            if(mudou == true)
            {
                elo_pilha_t* mudanca = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
                mudanca->op.transposicao = true;
                mudanca->op.linha = lin;
                mudanca->op.coluna = col;
                mudanca->op.tamanho = tamanho;
                mudanca->proximo = p->historico.topo;
                p->historico.topo = mudanca;
                
            }
            
            return mudou;
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
       Retorna false se o historico estiver vazio, true caso contrario. */
    elo_pilha_t* temp;
    int valorAtual = 0;
    celula_t* CelulaMexida;
       
    if(p->historico.topo != NULL)
    {
        if(p->historico.topo->op.transposicao == false)
        {
            if(p->historico.topo->op.valor_anterior == 0) //ela nn existia antes, logo tenho q removela
            {
                
                remover_celula(p, p->historico.topo->op.linha, p->historico.topo->op.coluna);
                //usar remover celula, porem evitar a criação de um historico desse remove
                temp = p->historico.topo;
                p->historico.topo = p->historico.topo->proximo;
                free(temp);
                temp = NULL;
            }
            else
            {
                celula_t* cel_ant_linha;
                celula_t* cel_ant_coluna;
                fileira_t* fil_ant_linha;
                fileira_t* fil_ant_coluna;
                CelulaMexida = buscar_celula(p, p->historico.topo->op.linha, p->historico.topo->op.coluna, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna);
    
                if(CelulaMexida != NULL) //mudou de nn 0 para nn 0, ou seja, só alterou
                {   
                    //busca otimizada
                    CelulaMexida->valor = p->historico.topo->op.valor_anterior;
                    
                    
                }
                else
                {//valor atual dela é 0, e ela nn existe, ou seja, existia e foi removida, ent tenho q criala
                    definir_celula(p, p->historico.topo->op.linha, p->historico.topo->op.coluna, p->historico.topo->op.valor_anterior);
                    
                    temp = p->historico.topo;
                    p->historico.topo = p->historico.topo->proximo;
                    free(temp);
                    temp = NULL;
                }
                
            }
        }
        else
        {
            transpor(p, p->historico.topo->op.linha, p->historico.topo->op.coluna, p->historico.topo->op.tamanho);
            
            temp = p->historico.topo;
            p->historico.topo = p->historico.topo->proximo;
            free(temp);
            temp = NULL;
        }
        
        
        //de qualquer jeito, se o historico era nn vazio, logo deu certo e possp apagar o historico
        temp = p->historico.topo;
        p->historico.topo = p->historico.topo->proximo;
        free(temp);
        temp = NULL;
        
        return true;
        
    }
    else
    {
        return false;    
    }
}

void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */
       
       celula_t* BaixoCelular;
       celula_t* DireitaCelular;
       celula_t* CelInicio;
       celula_t CelInicioReserva;
       CelInicioReserva.linha = 0;
       CelInicioReserva.coluna = 0;
       fileira_t* FilReserva = NULL;
       
       if(p->primeira_linha != NULL)
       {
           BaixoCelular = p->primeira_linha->primeiro;
           FilReserva = p->primeira_linha;
       }
       
       if(p->primeira_coluna != NULL)
       {
           DireitaCelular = p->primeira_coluna->primeiro; //teoricamente, DireitaCelular == BaixoCelular
       }
       else //só pra imrpimir uma vez msm, pois daria else no if anterior desse
       {
           printf("PLANILHA VAZIA\n");
           DireitaCelular = NULL;
           BaixoCelular = NULL;
       }
       
       while(BaixoCelular != NULL)
       {
           DireitaCelular = BaixoCelular;
           while(DireitaCelular != NULL)
           {
               printf("%d %d %d\n", DireitaCelular->linha, DireitaCelular->coluna, DireitaCelular->valor);
               DireitaCelular = DireitaCelular->proxima_linha;
               
           }
           if(FilReserva->proximo != NULL)
           {
               BaixoCelular = FilReserva->proximo->primeiro;
               FilReserva = FilReserva->proximo;
           }
           else
           {
               BaixoCelular = NULL;
           }
        }
       
       
}

void exibir_historico(planilha_t* p) {
    /* TODO: imprima "linha coluna valor_anterior" por linha, do topo
       para a base. Se vazio, imprima "HISTORICO VAZIO" */
       elo_pilha_t* andador;
       
       if(p->historico.topo != NULL)
       {
           andador = p->historico.topo;
           while(andador != NULL)
           {
               if(andador->op.transposicao == true)
               {
                   printf("(T %d %d %d)\n", andador->op.linha, andador->op.coluna, andador->op.tamanho);
                   andador = andador->proximo;
               }
               else
               {
                   printf("%d %d %d\n", andador->op.linha, andador->op.coluna, andador->op.valor_anterior);
                   andador = andador->proximo;
               }
           }
       }
       else
       {
           printf("HISTORICO VAZIO\n");
       }
}

void liberar_tudo(planilha_t* p) {
    // TODO: libere toda a memória alocada por fileira, celula, e pilha.
    
    fileira_t* Linhas = p->primeira_linha;
    fileira_t* Colunas = p->primeira_coluna; //se existe tal, ent existe um valor nelas
    celula_t* CelAndador;
    fileira_t* filAndador;
    
    elo_pilha_t* EloAndador;
    elo_pilha_t* Elos;
    
    while(Linhas != NULL) //apagar todas as celulas
    {
        
        if(Linhas->primeiro == NULL)
        {
           Linhas = Linhas->proximo; //caso base para acabar e ir para a proxima linha
        }
        else
        {
        
        CelAndador = Linhas->primeiro->proxima_linha; //proximo da msm linha
        
        free(Linhas->primeiro); //nn tem problema em relação as outras ligacoes pq tudo vai ser apagada
        Linhas->primeiro = CelAndador; //se for nulo,
        }
        
    }
    Linhas = p->primeira_linha;
    while(Linhas != NULL)
    {
        filAndador = Linhas->proximo;
        free(Linhas);
        Linhas = filAndador;
    }
    Colunas = p->primeira_coluna;
    while(Colunas != NULL)
    {
        filAndador = Colunas->proximo;
        free(Colunas);
        Colunas = filAndador;
    }
    
    Elos = p->historico.topo;
    while(Elos != NULL)
    {
        EloAndador = Elos->proximo;
        free(Elos);
        Elos = EloAndador;
    }

    p->primeira_linha = NULL;
    p->primeira_coluna = NULL;
    p->total_celulas = 0;
    p->historico.topo = NULL;
}

/* ---------------------------------------------------------------------- *
    Main para leitura de arquivos (já pronta no caso, pode ser que na versão final deixemos sem)
 ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso do comando eh: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
    
    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = freopen(argv[2], "w", stdout);

    if (!entrada || !saida) {
        fprintf(stderr, "Erro ao tentar abrir os arquivos.\n");
        return 1;
    }

    planilha_t p;
    inicializar_planilha(&p);

    int n;
    fscanf(entrada, "%d", &n);

    char cmd[20];

    while (fscanf(entrada, "%s", cmd) != EOF) {
 
        if (igual(cmd, "DEF")) {
            int lin, col, valor;
            fscanf(entrada, "%d %d %d", &lin, &col, &valor);
            definir_celula(&p, lin, col, valor);
        } else if (igual(cmd, "REM")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            remover_celula(&p, lin, col);
        } else if (igual(cmd, "GET")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            fprintf(saida, "GET %d %d %d\n", lin, col, obter_valor(&p, lin, col));
        } else if (igual(cmd, "SOMA")) {
            int li, lf, ci, cf;
            fscanf(entrada, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(saida, "SOMA %d %d %d %d %d\n", li, lf, ci, cf, somar_intervalo(&p, li, lf, ci, cf));
        } else if (igual(cmd, "CONT")) {
            fprintf(saida, "CONT %d\n", contar_nao_nulas(&p));
        } else if (igual(cmd, "DESFAZER")) {
            if (!desfazer(&p)) {
                fprintf(saida, "HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "EXIBIR")) {
            if (contar_nao_nulas(&p)) {
                printf("PLANILHA\n");
                exibir_planilha(&p);
            } else {
                printf("PLANILHA VAZIA\n");
            }
        } else if (igual(cmd, "HIST")) {
            if (p.historico.topo) {
                printf("HISTORICO\n");
                exibir_historico(&p);
            }
            else {
                printf("HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "TRANS")) {
            int lin, col, tam;
            fscanf(entrada, "%d %d %d", &lin, &col, &tam);
            transpor(&p, lin, col, tam);
        }
    }

    fclose(entrada);
    fclose(saida);

    liberar_tudo(&p);
    return 0;
}
