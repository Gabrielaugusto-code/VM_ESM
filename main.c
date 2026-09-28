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
uint8_t ROM[tamanho] = {0b00000101, 56, 76, 0b00000010};
uint64_t ROMP = 0;

// Inicializa a RAM
#define alocacao 65536
uint8_t RAM[alocacao];
uint16_t RAMP = 0;

// Corpo da RAM 
uint16_t pointerVI = 0; // diz qual foi o ultimo conjunto colocado (diz onde estamos)

// a satck vai trabalhar como se fosse registradores 
#define Tstack 16
uint64_t STACK[Tstack];
uint8_t Stag[Tstack]; // diz se o dado da sack é endereço para ram ou dado e outros metadados
int8_t sp = 0; // ponteiro da stack

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

void store() {
    sp--;
    if (Stag[sp] == 1) {
        uint64_t v = STACK[sp];              // cópia, a stack não é alterada

        uint8_t n = 1;                       // passada 1: quantos bytes
        for (uint64_t t = v >> 8; t != 0; t >>= 8) n++;

        uint16_t enderecoAtual = pointerVI * 2;
        RAM[enderecoAtual]     = RAMP >> 8;
        RAM[enderecoAtual + 1] = RAMP & 0xFF;
        pointerVI++;

        for (uint8_t i = 0; i < n; i++) {    // passada 2: grava
            RAM[RAMP + i] = (v >> ((n - 1 - i) * 8)) & 0xFF;
        }
        RAMP += n;
    }
}
/*
void store() {
    uint16_t enderecoAtual = pointerVI * 2;
    RAM[enderecoAtual] = RAMP >> 8;
    enderecoAtual += 1;
    RAM[enderecoAtual] = RAMP & 0xFF;
    sp--;
    if (Stag[sp] == 1) {
        uint64_t v = STACK[sp];
        uint8_t n = 1;
        while ((v >>= 8) != 0) {
            n++;
        }
        RAMP = RAMP + n;
        v = STACK[sp];
        for (uint8_t i = 0; i < n; i++) {
            RAM[RAMP - i] = (v & 0xFF); // pega o byte menos valioso
        }
    }
}
*/
// so para strings e booleanos (booleanos clusters)
void push_D(uint8_t QntdBdTamanho) { // coloca strings na stack virtual




}

// so pra inteiros, decimais, fica na stack de dados
void push_F(uint8_t QntdBdTamanho) { // pega da rom e manda pra stack fisica
    // QntdBdTamanho é o tamanho de bytes daa variavel, pode ser 00 = 1b, 01 = 2b, 10 = 4b, 11 = 8b
    STACK[sp] = 0;
    QntdBdTamanho = 1 << QntdBdTamanho; // ajusta o modificador 
    for (uint8_t i = 0; i < QntdBdTamanho; i++) {
        STACK[sp] = (STACK[sp] << 8) | op();
    }
    Stag[sp] = 1; // dado cru
    sp++;
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
            if (modificador == 0b00000010) { //string
                printf("%c", STACK[--sp]);
            }
        break;
        case 1: // push var (coloca na satck)
            push_F(modificador); // fixo direto no registrador
        break;
        case 2: // push
            push_D(modificador); // dinamico direto na ram
        break;
        case 3: // store tira o dado da stack e manda pra ram 
            store();
        break;
        case 4: // load var (traz a variavel da ram para vm)tira ram
            //load();
        break;
        
    }
}


int main() {

    while (rodando() == 1) {
        VM();
    }

    return 0;
}