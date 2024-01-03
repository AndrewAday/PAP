#include <stdio.h>


const char* reg8[] = {
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
};

const char* reg16[] = {
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
};

void print_bin(int byte)
{
    printf("%d%d%d%d%d%d%d%d ",
        (byte & 0x80) >> 7,
        (byte & 0x40) >> 6,
        (byte & 0x20) >> 5,
        (byte & 0x10) >> 4,
        (byte & 0x08) >> 3,
        (byte & 0x04) >> 2,
        (byte & 0x02) >> 1,
        (byte & 0x01) >> 0
    );
}

int main(int argc, char *argv[])
{
    FILE *binary_fp, *decode_fp;
    int byte1, byte2;
    printf("argv[0] = %s\n", argv[0]);
    printf("argv[1] = %s\n", argv[1]);

    // open file
    binary_fp = fopen(argv[1], "rb");

    // create output file by concatenating ".decode" to input file name
    char decode_file_name[100];
    // sprintf(decode_file_name, "%s.decode", argv[1]);
    sprintf(decode_file_name, "decode.asm", argv[1]);
    decode_fp = fopen(decode_file_name, "w");

    fputs("bits 16\n", decode_fp);

    while ((byte1 = fgetc(binary_fp)) != EOF) {
        int byte2 = fgetc(binary_fp);

        // print as binary
        print_bin(byte1);
        print_bin(byte2);
        printf("\n");
        
        char opcode = (byte1 & 0b11111100) >> 2;
        char D = (byte1 & 0b10) >> 1;
        char W = (byte1 & 0b01) >> 0;

        char MOD = (byte2 & 0b11000000) >> 6;
        char REG = (byte2 & 0b00111000) >> 3;
        char RM  = (byte2 & 0b00000111) >> 0;

        char instr_src = (D == 0) ? REG : RM;
        char instr_dst = (D == 0) ? RM : REG;

        const char* reg_src_str = (W == 1) ? reg16[instr_src] : reg8[instr_src];
        const char* reg_dst_str = (W == 1) ? reg16[instr_dst] : reg8[instr_dst];

        fprintf(decode_fp, "mov %s, %s\n", reg_dst_str, reg_src_str);
    }

    // close files
    fclose(binary_fp);
    fclose(decode_fp);

    return 0;
}
