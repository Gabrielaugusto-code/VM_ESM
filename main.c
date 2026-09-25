// Vm da minha Linguagem de programação


#include <stdio.h>
#include <stdint.h> 

/*

Baguette 0.1 (Oraganizaçao dos dados na ram)

array da ram é tudo em 8 bits

[endereços das variaveis VI, dados fixos, dados dinamicos, .........................., Cache da heap]

os endereços das variaveis VI sao selecionados, tipo pull 1, seleciona o conjuto endereço 1
2 bytes, transforma em numero vai de 0 a 65556 bytes de range, le o que esse conjunto diz 
(vai falar onde começa a var), vai ler ate bater no começo da proxima var, vai saber lendo o proximo
conjunto. entedeu nao vai ter id da vm e endereço da vm, vai ser seleçao do endereço, porque
os endereços sao fixos e nao se movem, mas sao modificados, as variaveis podem se mover.

endereços das variaveis VI: são ponteiros que apontam onde as variaveis começam, o fim delas
e dada pelo começo da outra, onde a ultima vaiavel usa o peso de papel.

dados fixos: é so binario que a vm vai interpretrar, as strings vao usar ascii do proprio C, e a tipagem
vai ser "deduzido"/"imbutido" nas intruçoes/opcode, os dados da esquerda tem mais valor e  da direita menos, esses nao cresem

dados dinamicos: é so binario que a vm vai interpretrar, as strings vao usar ascii do proprio C, e a tipagem
vai ser "deduzido"/"imbutido" nas intruçoes/opcode, os dados da esquerda tem mais valor e  da direita menos, esses cresem
e vao ser o destaque da linguagem

heap: vai ser aplicada as dados dinamicos, seu funcionamento vai ser quando for espandir uma variavel A ver se tem espaço consultando o cache da heap senao tiver,
levar a variavel B para o final do array da ram antes do peso de papel, mudar o endereço VI da B para onde ela esta agora, expandir A, se sobrar espaço
entre A e C salvar o começo desse espaço no cache da heap entao assem que tiver uma variavel que cabe ali colocar ela la.






Primeiros 2 bytes diz onde termina os endereços de objetos 


*/

// Inicializa a ROM (em breva a vm vai executar arquivos externos)
#define tamanho 65536
uint8_t ROM[tamanho] = 
            {0, 56, 76, 
            1, 1, 
            2, 1};
uint64_t ROMP = 0;

// Inicializa a RAM
#define alocacao 65536
uint8_t RAM[alocacao];
uint64_t RAMP = 0;

#define Tstack 16
uint8_t STACK[Tstack];

uint8_t op() {
    uint8_t opcode = ROM[ROMP];
    ROMP += 1;
    return opcode;
}



void push() {
    for () {

    }

}

void pull(uint64_t posV) {
    //pega os bytes da ram e transforma em numero e coloca na stack


}

void VM() {
    uint8_t opcode = op();
    switch (opcode) {
        case 0:
            printf("");
        break;
        case 1: // push var (manda a variavel pra ram)
            //push();
        break;
        case 2: // pull var (traz a variavel para vm)
            //pull();
        break;
    }
}


int main()
{
    while (1==1) {
        VM();
    }

    return 0;
}