// 0xF0 - 0xFF reservados para controle da VM
// 0xE0 - 0xEF reservados para extensões futuras

// Intrucoes/comandos
// O que fazer com os dados
// dados 
// Comando

// if = dadosA, dadosB, if, operador, comando de jump, tamanho (na versao 1 ainda nao tem tamanho variavel so fixo = 1), numero de instrucoes puladas
// funçoes: vai ter jump antes da funcao pulando todos os seus comandos,
#include <iostream>
#include <cstdint>
#include <cstring>

#define BC_SIZE 65535 // tamanho do maximo do bytecode

const uint8_t Bytecode[BC_SIZE] = {
  0x32, 0x07, 0x10, 0xFB, 0x02, 0x56, 0x56, 0x01, 0x43,
  0x10, 0xFB, 0x02, 0x00, 0x64,
  0x10, 0xFB, 0x02, 0x00, 0x32,
  0x20,
  0x01,
  0x10, 0xFB, 0x02, 0x04, 0x56, 0x30, 0x01,
  0x10, 0xFB, 0x02, 0x04, 0x56, 0x30, 0x02,
  0x31, 0x01, 0x01, 0x31, 0x02, 0x01,
  0x31, 0x01, 0x31, 0x02,
  0x0E, 0x24, 0x06, 0x10, 0xFD, 0x02, 0x4F, 0x69, 0x01,
  0x10, 0xFB, 0x01, 0x08,
  0x10, 0xFB, 0x01, 0x08,
  0x22, 0x01, 0x42, 0x00, 0x02,
  0x10, 0xFD, 0x01, 0x67, 0x01,
  0xFF
};

struct Value {
  uint8_t type;
  uint8_t size;
  uint8_t data[8];
};

Value stack[8]; // qunatos valores podem existir ao mesmo tempo durante a execucao
Value vars[64]; // vars que podem existir (o compilador vai dizer, mas o maximo vai ser 128)
uint16_t callStack[16]; // chamadas dentro de chamadas dentro de chamadas
uint8_t cp = 0;

uint8_t sp = 0; // ponteiro que indica a distancia do topo (0)
uint16_t pc = 0; // program couter indica onde o programa esta
bool running = true;

uint8_t rd() { return Bytecode[pc++]; }

uint64_t readNum(Value &v) { // funcao que junta os bytes formando numero inteiro
  uint64_t n = 0;
  for (uint8_t i = 0; i < v.size; i++)
    n = (n << 8) | v.data[i];
  return n;
}

void loop() {
  if (!running) return;

  uint8_t op = rd();
  uint8_t id1 = 0;
  uint8_t id2 = 0;

  if (sp >= 8) { 
    running = false; // stack overflow
    return;
}

  switch (op) {
    case 0x10: { // PUSH Colocar um valor literal no rascunho/stack
      stack[sp].type = rd(); // le o tipo do dado que vem do bytecode
      stack[sp].size = rd(); // le o tamanho do dado que vem do bytecode
      for (uint8_t i = 0; i < stack[sp].size; i++) // le os numeros que compoem o dado e armazena no rascunho/stack
        stack[sp].data[i] = rd();
      sp++; // Avança a stack
      break;
    }

    case 0x20: { // ADD Consome os tres ultimos valores da stack/rascunho
        if (sp < 2) {
            running = false;
            break;
        }
      // vai ser consumido por que depois o programa vai escrever por cima
      Value b = stack[--sp]; // cria uma instacia "b" passa o primeiro valor da pilha para "b" pos 3
      Value a = stack[--sp]; // cria uma instacia "a" passa o primeiro valor da pilha para "a" pos 2
      uint64_t r = readNum(a) + readNum(b); // junta o valo a com o valor b
      stack[sp].type = 0xFB; // pos 1
      stack[sp].size = 2; // pos 1
      stack[sp].data[0] = r >> 8; // pos 1
      stack[sp].data[1] = r & 0xFF; // pos 1
      sp++;
      break;
    }

    case 0x01: { // PRINT
      Value v = stack[--sp]; // salva o valor e consome o valor na stack/rascunho
        if (v.type == 0xFB) {
            std::cout << readNum(v) << std::endl; // 
        }
        if (v.type == 0xFD) { // texto
            for (uint8_t i = 0; i < v.size; i += 1) {
                std::cout << (char)v.data[i]; //
            }
            std::cout << "\n";
        }
      break;
    }

    case 0x30: { // STORE salva o valor em um lugar na ram ou seja dentro de uma caixinha var
      uint8_t id = rd(); // vai de 0 a 255 vars diz aonde armazenar na ram
      Value v = stack[--sp];
      vars[id] = v;
      break;
    }

    case 0x31: { // LOAD
      uint8_t id = rd(); // vai de 0 a 255 vars diz qual valor pegar na ram
      stack[sp] = vars[id]; // copia o valor da ram para stack
      sp++;
      break;
    }

    case 0x32: { // JUMP pula opcodes
      uint8_t pulo = rd(); // pula no maximo 255 bytes por vez caso, podemos usar varios para pular lacos condicionais gigantes
      pc = pc + pulo;
      break;
    }

    case 0x33: { // BACK volta opcodes
      uint8_t volta = rd(); // vai de 0 a 255 vars diz qual valor pegar na ram
      pc = pc - volta;
      break;
    }

    case 0x42: { // CALL salva o pc atual e pula para a funcao
      // 2 bytes de id para idicar a posicao da funcao
      // tem que ser em big edian
      id1 = rd();
      id2 = rd();
      uint16_t callpc = (id1 << 8) | id2;
      callStack[cp] = pc; // salva o pc atual dentro da stack
      cp++;
      pc = callpc;
      break;
    }

    case 0x43: { // RET volta para o ultimo pc salvo pelo call
      pc = callStack[--cp];
      break;
    }

    // SUB MUL DIV

    case 0x21: { // SUB
      if (sp < 2) {
        running = false;
        break;
      }
      Value b = stack[--sp]; // diminui para ajustar com o formato do indice (0, 1, 2, 3)
      Value a = stack[--sp]; // "b" e "a" tirados do stack
      uint64_t r = readNum(a) - readNum(b); // subtrai o valor de a com o valor de b
      stack[sp].type = 0xFB; // modifica a pos da stack para inteiro
      stack[sp].size = 2; // tamanho 
      stack[sp].data[0] = r >> 8; // quebra o numero em bytes
      stack[sp].data[1] = r & 0xFF; // 
      sp++; // avança a stack
      break;
    }
    case 0x22: { // MUL
      if (sp < 2) {
        running = false;
        break;
      }
      Value b = stack[--sp]; // diminui para ajustar com o formato do indice (0, 1, 2, 3)
      Value a = stack[--sp]; // "b" e "a" tirados do stack
      uint64_t r = readNum(a) * readNum(b); // multiplica o valor de a com o valor de b
      stack[sp].type = 0xFB; // modifica a pos da stack para inteiro
      stack[sp].size = 2; // tamanho 
      stack[sp].data[0] = r >> 8; // quebra o numero em bytes
      stack[sp].data[1] = r & 0xFF; //
      sp++; // avança a stack
      break;
    }
    case 0x23: { // DIV
      if (sp < 2) {
        running = false;
        break;
      }
      Value b = stack[--sp]; // diminui para ajustar com o formato do indice (0, 1, 2, 3)
      Value a = stack[--sp]; // "b" e "a" tirados do stack
      uint64_t r = readNum(a) / readNum(b); // divide o valor de a com o valor de b
      stack[sp].type = 0xFB; // modifica a pos da stack para inteiro
      stack[sp].size = 2; // tamanho 
      stack[sp].data[0] = r >> 8; // quebra o numero em bytes
      stack[sp].data[1] = r & 0xFF; // 
      sp++; // avança a stack
      break;
    }

    case 0x0E: { // SE dado1 dado 2 0x0E operador (0x24 = "==", 0x25 = ">=", 0x26 = "<=", 0x27 = ">", 0x28 = "<")
      if (sp < 2) {
        running = false;
        break;
      }
      Value b = stack[--sp]; //  pega o valor de B
      Value a = stack[--sp]; // pega o valor de A
      op = rd(); // pega o operador
      bool result = false;
      if (a.type != b.type) {
        result = false;
      }
      uint8_t TextoCiclo = 0;
      if ((a.type == 0xFB && b.type == 0xFB) || (a.type == 0xFC && b.type == 0xFC)) { // caso seja decimal ou inteiro
        uint64_t av = readNum(a);
        uint64_t bv = readNum(b);
        // onde parei
        switch (op) {
        case 0x24: result = (av == bv); break; // ==
        case 0x25: result = (av >= bv); break; // >=
        case 0x26: result = (av <= bv); break; // <=
        case 0x27: result = (av >  bv); break; // >
        case 0x28: result = (av <  bv); break; // <
        }
      } else if (a.type == 0xFA) { // caso seja booleano so suporta ==
        if (a.data[0] == b.data[0]) result = true;
      } else if (a.type == 0xFD && b.type == 0xFD) { // Caso string so vai suportar ==
        if (a.size == b.size) {
          for (uint8_t i = 0; i < a.size; i += 1) {
            if (a.data[i] == b.data[i]) TextoCiclo = TextoCiclo + 1;
          }
          if (TextoCiclo == a.size) result = true;
        }
      }
      op = rd(); // pega o ofset do jump
      if (result == true) {
        op = 0;
      } else {
        pc = pc + op;
      }
      break;
    }

    case 0xFF:
      running = false;
      break;
  }
}


int main() {
    while (running == true) {
        loop();
    }
    
}