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


Ah o codigo inteiro é pra estar em big edian os da esquerda valiossos e da direita fracos

*/

// Inicializa a ROM (em breva a vm vai executar arquivos externos)
#define tamanho 5
uint8_t ROM[tamanho] = {0b00001000, 2, 56, 76, 0b00000010};
uint64_t ROMP = 0;

// Inicializa a RAM
#define alocacao 65536
uint8_t RAM[alocacao];
uint16_t RAMP = 0;

// Corpo da RAM 
uint16_t pointerVI = 0; // diz qual foi o ultimo conjunto colocado (diz onde estamos)
#define UnitsRAM 2 // 2 bytes

// a satck vai trabalhar como se fosse registradores 
#define Tstack 16
uint8_t sp = 0;
struct Registrador {
    uint8_t tamanhoR;
    uint8_t Stag; // diz se o dado da sack é endereço para ram ou dado e outros metadados
    uint8_t dado[8];
};

struct Registrador stack[Tstack];

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

void move(uint64_t pos) { // vai ir ate chegar em outra variavel


}

void append(uint64_t enderecoVI, uint8_t dado) { // acresento  o byte x a variavel



}

uint16_t getID(uint16_t VI) { // pega o endereço real da variavel
    VI = VI - 1;
    uint16_t bytesVI = 0;
    for (uint8_t i = 0; i < UnitsRAM; i++) {
        bytesVI = bytesVI <<= 8;
        bytesVI |= RAM[VI];
        VI++;
    }
    return bytesVI;
}

uint16_t varID(uint16_t posVar) { // qual endereço é desse dado?
    for (uint8_t i = 0; i < (pointerVI * 2); i++) {
        if (getID(i) == posVar) {
            return posVar;
        }
    }
    return 0;
}


void new() { // cria a variavel
    uint16_t enderecoAtual = pointerVI * 2;
    if (varID(enderecoAtual) == 0) { // se o proximo byte esta livre


    }

    uint16_t enderecoVIvar = 

    RAM[enderecoAtual]     = RAMP >> 8;
    RAM[enderecoAtual + 1] = RAMP & 0xFF;
    pointerVI++;
}

void store() {
    sp--;
    if (stack[sp].Stag == 1) { // se o dado estiver na stack
        new();
        for (uint8_t i = 0; i < stack[sp].tamanhoR; i++) {
            RAM[RAMP] = stack[sp].dado[i];
            RAMP += 1;
        }
    }
}

// para strings e booleanos (booleanos clusters)  principalmente
void put(uint8_t QntdBdTamanho) {
    // aqui no put o modificado que geralmente é o tamanho do dado, indica o tamanho do tamanho do dado ou seja, quantos bytes o tamanho dessa variavel ocupa

    // Primeiro passo achar o tamnho de verdade
    // no caso so juntar bytes que o compilador ja calculou
    uint64_t tamanhoDeVerdade = 0;
    for (uint8_t i = 0; i < QntdBdTamanho; i++) {
        tamanhoDeVerdade = tamanhoDeVerdade << 8;
        tamanhoDeVerdade |= ROM[ROMP];
        ROMP++;
    }
    new();
    for (uint16_t o = 0; o < tamanhoDeVerdade; o++) {
        RAM[RAMP] = ROM[ROMP];
        RAMP += 1;
        ROMP += 1;
    }
}

// so pra inteiros, decimais, fica na stack de dados
void push(uint8_t QntdBdTamanho) { // pega da rom e manda pra stack fisica
    // QntdBdTamanho é o tamanho de bytes daa variavel, pode ser 00 = 1b, 01 = 2b, 10 = 4b, 11 = 8b
    QntdBdTamanho = 1 << QntdBdTamanho; // ajusta o modificador 
    stack[sp].tamanhoR = QntdBdTamanho;
    for (uint8_t i = 0; i < QntdBdTamanho; i++) {
        stack[sp].dado[i] = op();
    }
    stack[sp].Stag = 1; // dado cru
    sp++;
}

void load() {

}


void VM() {
    uint8_t opcode = op(); // instruçao de 8 bits
    uint8_t comando = opcode >> 2; // pega os primeiros 6 bits (coloca 2 bits zerados no começo)
    uint8_t modificador = opcode & 0b00000011; // pega os ultimos 2 bits

    switch (comando) {
        case 0: // print
            sp--;
            if (stack[sp].Stag == 1) {
                for (uint8_t i = 0; i < stack[sp].tamanhoR; i++) {
                    printf("%c", stack[sp].dado[i]);
                }
            } else if (stack[sp].Stag == 2) {
                for (uint8_t i = 0; i < stack[sp].tamanhoR; i++) {
                    printf("%c", stack[sp].dado[i]);
                }
            }
        break;
        case 1: // push 
            push(modificador); // da rom para stack (registradores)
        break;
        case 2: // push
            put(modificador); // da rom para ram
        break;
        case 3: // store
            store(); // da stack para ram
        break;
        case 4: // load 
            load(); // da ram para stack
        break;
        
    }
}


int main() {

    while (rodando() == 1) {
        VM();
    }

    return 0;
}