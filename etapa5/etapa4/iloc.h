#ifndef _ILOC_H_
#define _ILOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tipos de operandos em ILOC
 */
typedef enum {
    ILOC_OP_REG,    // Registrador (r1, r2...)
    ILOC_OP_INT,    // Inteiro imediato (10, 4...)
    ILOC_OP_LABEL   // Rótulo (L1, L2...)
} iloc_operand_type_t;

/*
 * Definição de todos os opcodes suportados (conforme Apêndice C do PDF)
 */
typedef enum {
    ILOC_NOP,
    // Aritmética
    ILOC_ADD, ILOC_SUB, ILOC_MULT, ILOC_DIV,
    ILOC_ADDI, ILOC_SUBI, ILOC_RSUBI, ILOC_MULTI, ILOC_DIVI, ILOC_RDIVI,
    // Shift
    ILOC_LSHIFT, ILOC_LSHIFTI, ILOC_RSHIFT, ILOC_RSHIFTI,
    // Lógica
    ILOC_AND, ILOC_ANDI, ILOC_OR, ILOC_ORI, ILOC_XOR, ILOC_XORI,
    // Carga (Load)
    ILOC_LOADI, ILOC_LOAD, ILOC_LOADAI, ILOC_LOADA0,
    ILOC_CLOAD, ILOC_CLOADAI, ILOC_CLOADA0,
    // Armazenamento (Store)
    ILOC_STORE, ILOC_STOREAI, ILOC_STOREAO,
    ILOC_CSTORE, ILOC_CSTOREAI, ILOC_CSTOREAO,
    // Cópia/Conversão
    ILOC_I2I, ILOC_C2C, ILOC_C2I, ILOC_I2C,
    // Fluxo de Controle
    ILOC_CMP_LT, ILOC_CMP_LE, ILOC_CMP_EQ, ILOC_CMP_GE, ILOC_CMP_GT, ILOC_CMP_NE,
    ILOC_CBR, ILOC_JUMP, ILOC_JUMPI
} iloc_opcode_t;

/*
 * Estrutura de um operando
 */
typedef struct {
    iloc_operand_type_t type;
    int value; // Se REG: numero do reg (1 para r1). Se LABEL: numero do label (2 para L2). Se INT: valor literal.
} iloc_operand_t;

/*
 * Estrutura de uma instrução ILOC
 * A maioria das ops tem no máximo 3 argumentos (src1, src2 => dst) ou (src1 -> dst1, dst2)
 */
typedef struct {
    iloc_opcode_t opcode;
    iloc_operand_t op1; // Primeiro argumento (ex: r1 em 'add r1, r2 => r3')
    iloc_operand_t op2; // Segundo argumento (ex: r2)
    iloc_operand_t op3; // Terceiro argumento (ex: r3)
    
    // Flags para saber quantos operandos usar na impressão
    int num_ops; 
} iloc_op_t;

/*
 * Nó da lista encadeada de instruções
 */
typedef struct iloc_code {
    iloc_op_t *op;           // A instrução em si
    struct iloc_code *prev;  // Ponteiro para instrução anterior
    struct iloc_code *next;  // Ponteiro para próxima instrução
} iloc_code_t;


/* --- Funções de API --- */

// Geradores de Nomes
char *iloc_new_label(); // Retorna string "Ln" e incrementa contador
char *iloc_new_reg();   // Retorna string "rn" e incrementa contador
int iloc_next_label_id(); // Retorna apenas o ID inteiro
int iloc_next_reg_id();   // Retorna apenas o ID inteiro

// Criação de Instruções
// Função genérica para criar instrução. Argumentos opcionais podem receber type=ILOC_OP_INT e value=0 ignorados.
iloc_code_t *iloc_create_inst(iloc_opcode_t opcode, 
                              iloc_operand_type_t type1, int val1,
                              iloc_operand_type_t type2, int val2,
                              iloc_operand_type_t type3, int val3);

// Concatenação de código (muito usado na AST: code = code(filho1) + code(filho2))
iloc_code_t *iloc_append(iloc_code_t *code1, iloc_code_t *code2);

// Impressão
void iloc_print_code(iloc_code_t *code);

#endif