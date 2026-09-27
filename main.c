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

/*

Marquise 0.1 (Organização do bytecode)

000001 00 → comando 1, inteiro
000001 01 → comando 1, decimal
000001 10 → comando 1, string
000001 11 → comando 1, boolean


*/

// Inicializa a ROM (em breva a vm vai executar arquivos externos)
#define tamanho 5
uint8_t ROM[tamanho] = {0b00000100, 2, 56, 76, 0b00000010};
uint64_t ROMP = 0;

// Inicializa a RAM
#define alocacao 65536
uint8_t RAM[alocacao];
uint64_t RAMP = 0;

#define Tstack 16
uint64_t STACK[Tstack];
uint8_t sp = 0;

uint8_t rodando() {
    if (ROMP == tamanho) {
        return 0;
    }
    return 1;
}

uint8_t op() {
    uint8_t opcode = ROM[ROMP];
    ROMP += 1;
    return opcode;
}
/*
void push(uint8_t QntdBdTamanho) {
    // Essa variavel QntdBdTamanho diz quantos bytes o tamanho da variavel ocupa, tipo o tamanho de uma variavel pode ter 4Gigas de ram, por esses ( É O TAMANHO DO TAMANHO)
    STACK[sp] = 0;
    QntdBdTamanho += 1;
    uint8_t b = 0;
    for (uint8_t a = 0; a < QntdBdTamanho; a++) {
        b++;
    }
    for (uint8_t i = 0; i < b; i++) {
        STACK[sp] = (STACK[sp] << 8) | op();
    }
    sp++;
}
*/
void push(uint8_t QntdBdTamanho) {

    QntdBdTamanho += 1;

    uint32_t tamanhof = 0;

    // Lê o tamanho da variável
    for (uint8_t i = 0; i < QntdBdTamanho; i++) {
        tamanhof = (tamanhof << 8) | op();
    }

    // Lê os dados
    STACK[sp] = 0;

    for (uint32_t i = 0; i < tamanhof; i++) {
        STACK[sp] = op();
        sp++;
    }
}

/*



void pull(uint64_t posV) {
    //pega os bytes da ram e transforma em numero e coloca na stack


}
void load() {

}

*/

void VM() {
    uint8_t opcode = op(); // instruçao de 8 bits
    uint8_t comando = opcode >> 2; // pega os primeiros 6 bits (coloca 2 bits zerados no começo)
    uint8_t modificador = opcode & 0b00000011; // pega os ultimos 2 bits

    switch (comando) {
        case 0:
            if (modificador == 2) { //string
                printf("%c", STACK[--sp]);
            }
        break;
        case 1: // push var (coloca na satck)
            push(modificador);
        break;
        case 2: // load var (traz a variavel da ram para vm)tira ram
            //load();
        break;
        
    }
}


int main()
{
    while (rodando() == 1) {
        VM();
    }

    return 0;
}