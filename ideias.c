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
