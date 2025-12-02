#ifndef _ASD_H_
#define _ASD_H_

#include "lexical_value.h"
#include "scope.h"
#include "iloc.h" // E5: Necessário para iloc_code_t

typedef struct asd_tree {
  char *label;
  int number_of_children;
  struct asd_tree **children;
  
  // E4: Tipo de dado (Inferência de Tipos)
  data_type_t type; 

  // E5: Geração de Código
  iloc_code_t *code; // Lista de instruções ILOC geradas por este nó e seus filhos
  char *temp;        // Nome do registrador (ou literal) que guarda o resultado deste nó (ex: "r1", "10")
} asd_tree_t;

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_add_child_to_node, adiciona child como filho de node.
 */
asd_tree_t *asd_add_child_to_node(asd_tree_t *node, asd_tree_t *child);

/*
 * Seleciona o primeiro nó (head) se existir, anexando tail a ele; caso contrário retorna tail
 */
asd_tree_t *asd_select_head_and_attach_tail(asd_tree_t *head, asd_tree_t *tail);

/*
 * Função asd_new_trinary, cria um nó com três filhos
 */
asd_tree_t *asd_new_trinary(const char *label, asd_tree_t *left, asd_tree_t *middle, asd_tree_t *right);

/*
 * Função asd_new_binary, cria um nó com dois filhos
 */
asd_tree_t *asd_new_binary(const char *label, asd_tree_t *left, asd_tree_t *right);

/*
 * Função asd_new_unary, cria um nó pai com apenas um filho
 */
asd_tree_t *asd_new_unary(const char *label, asd_tree_t *child);

/*
 * Função asd_new_node_from_value, cria um nó para a arvore a partir de um valor literal
 */
asd_tree_t *asd_new_node_from_value(lexical_value_t *value);

/*
 * Função asd_new_function_call_node, cria um nó de chamada de função
 */
asd_tree_t *asd_new_function_call_node(lexical_value_t *id, asd_tree_t *args_root);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);

#endif //_ASD_H_