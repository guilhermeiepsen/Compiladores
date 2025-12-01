#include "iloc.h"

/*
 * Contadores globais
 */
static int _iloc_label_counter = 0;
static int _iloc_reg_counter = 0;

int iloc_next_label_id() {
    return ++_iloc_label_counter;
}

int iloc_next_reg_id() {
    return ++_iloc_reg_counter;
}

char *iloc_new_label() {
    int id = iloc_next_label_id();
    char *buffer = calloc(16, sizeof(char));
    snprintf(buffer, 16, "L%d", id);
    return buffer;
}

char *iloc_new_reg() {
    int id = iloc_next_reg_id();
    char *buffer = calloc(16, sizeof(char));
    snprintf(buffer, 16, "r%d", id);
    return buffer;
}

iloc_code_t *iloc_create_inst(iloc_opcode_t opcode, 
                              iloc_operand_type_t type1, int val1,
                              iloc_operand_type_t type2, int val2,
                              iloc_operand_type_t type3, int val3) {
    
    iloc_code_t *code = calloc(1, sizeof(iloc_code_t));
    if (!code) return NULL;

    iloc_op_t *op = calloc(1, sizeof(iloc_op_t));
    if (!op) {
        free(code);
        return NULL;
    }

    op->opcode = opcode;
    op->op1.type = type1; op->op1.value = val1;
    op->op2.type = type2; op->op2.value = val2;
    op->op3.type = type3; op->op3.value = val3;

    code->op = op;
    code->prev = NULL;
    code->next = NULL;

    return code;
}

iloc_code_t *iloc_append(iloc_code_t *code1, iloc_code_t *code2) {
    if (code1 == NULL) return code2;
    if (code2 == NULL) return code1;

    iloc_code_t *cursor = code1;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }

    cursor->next = code2;
    code2->prev = cursor;

    return code1;
}

static void print_operand(iloc_operand_t op, char *buffer) {
    switch (op.type) {
        case ILOC_OP_REG:
            if (op.value == -1) sprintf(buffer, "rfp");
            else if (op.value == -2) sprintf(buffer, "rbss");
            else if (op.value == -3) sprintf(buffer, "rsp");
            else sprintf(buffer, "r%d", op.value);
            break;
        case ILOC_OP_INT:
            sprintf(buffer, "%d", op.value);
            break;
        case ILOC_OP_LABEL:
            sprintf(buffer, "L%d", op.value);
            break;
        default:
            buffer[0] = '\0';
    }
}

static const char* get_opcode_name(iloc_opcode_t op) {
    switch (op) {
        case ILOC_NOP: return "nop";
        case ILOC_ADD: return "add";
        case ILOC_SUB: return "sub";
        case ILOC_MULT: return "mult";
        case ILOC_DIV: return "div";
        case ILOC_ADDI: return "addI";
        case ILOC_SUBI: return "subI";
        case ILOC_RSUBI: return "rsubI";
        case ILOC_MULTI: return "multI";
        case ILOC_DIVI: return "divI";
        case ILOC_RDIVI: return "rdivI";
        case ILOC_LSHIFT: return "lshift";
        case ILOC_LSHIFTI: return "lshiftI";
        case ILOC_RSHIFT: return "rshift";
        case ILOC_RSHIFTI: return "rshiftI";
        case ILOC_AND: return "and";
        case ILOC_ANDI: return "andI";
        case ILOC_OR: return "or";
        case ILOC_ORI: return "orI";
        case ILOC_XOR: return "xor";
        case ILOC_XORI: return "xorI";
        case ILOC_LOADI: return "loadI";
        case ILOC_LOAD: return "load";
        case ILOC_LOADAI: return "loadAI";
        case ILOC_LOADA0: return "loadAO";
        case ILOC_CLOAD: return "cload";
        case ILOC_CLOADAI: return "cloadAI";
        case ILOC_CLOADA0: return "cloadAO";
        case ILOC_STORE: return "store";
        case ILOC_STOREAI: return "storeAI";
        case ILOC_STOREAO: return "storeAO";
        case ILOC_CSTORE: return "cstore";
        case ILOC_CSTOREAI: return "cstoreAI";
        case ILOC_CSTOREAO: return "cstoreAO";
        case ILOC_I2I: return "i2i";
        case ILOC_C2C: return "c2c";
        case ILOC_C2I: return "c2i";
        case ILOC_I2C: return "i2c";
        case ILOC_CMP_LT: return "cmp_LT";
        case ILOC_CMP_LE: return "cmp_LE";
        case ILOC_CMP_EQ: return "cmp_EQ";
        case ILOC_CMP_GE: return "cmp_GE";
        case ILOC_CMP_GT: return "cmp_GT";
        case ILOC_CMP_NE: return "cmp_NE";
        case ILOC_CBR: return "cbr";
        case ILOC_JUMP: return "jump";
        case ILOC_JUMPI: return "jumpI";
        default: return "unknown";
    }
}

void iloc_print_code(iloc_code_t *code) {
    if (!code) return;

    iloc_code_t *cur = code;
    char buf1[32], buf2[32], buf3[32]; 

    while (cur) {
        iloc_op_t *op = cur->op;
        const char *name = get_opcode_name(op->opcode);
        
        print_operand(op->op1, buf1);
        print_operand(op->op2, buf2);
        print_operand(op->op3, buf3);

        switch (op->opcode) {
            case ILOC_NOP:
                if (op->op1.type == ILOC_OP_LABEL) {
                    printf("%s: nop\n", buf1);
                } else {
                    printf("nop\n");
                }
                break;
            case ILOC_JUMPI:
                if (op->op3.type == ILOC_OP_LABEL) printf("%s -> %s\n", name, buf3);
                else printf("%s -> %s\n", name, buf1);
                break;
            case ILOC_JUMP:
                 printf("%s -> %s\n", name, buf1);
                 break;
            case ILOC_CBR:
                printf("%s %s -> %s, %s\n", name, buf1, buf2, buf3);
                break;
            case ILOC_LOAD:
            case ILOC_CLOAD:
            case ILOC_LOADI:
            case ILOC_I2I:
            case ILOC_C2C:
            case ILOC_C2I:
            case ILOC_I2C:
                printf("%s %s => %s\n", name, buf1, buf2);
                break;
            case ILOC_STOREAI:
            case ILOC_CSTOREAI:
                printf("%s %s => %s, %s\n", name, buf1, buf2, buf3);
                break;
            case ILOC_STORE:
            case ILOC_CSTORE:
                printf("%s %s => %s\n", name, buf1, buf2);
                break;
            default:
                printf("%s %s, %s => %s\n", name, buf1, buf2, buf3);
                break;
        }
        cur = cur->next;
    }
}

// Liberação de Memória
void iloc_code_free(iloc_code_t *code) {
    if (!code) return;
    iloc_code_t *cur = code;
    while (cur) {
        iloc_code_t *next = cur->next;
        if (cur->op) free(cur->op);
        free(cur);
        cur = next;
    }
}