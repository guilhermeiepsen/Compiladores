#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"

asd_tree_t *asd_new(const char *label)
{
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL){
    ret->label = strdup(label);
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->type = TYPE_UNDEFINED; // E4 Default type
    
    // E5 Initialization
    ret->code = NULL;
    ret->temp = NULL;
  }
  return ret;
}

void asd_free(asd_tree_t *tree)
{
  if (tree != NULL){
    int i;
    for (i = 0; i < tree->number_of_children; i++){
      asd_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->label);
    
    // E5 Cleanup
    if (tree->temp) free(tree->temp);
    // Note: tree->code is a linked list often shared/concatenated. 
    // Usually we don't free it here to avoid double-freeing if the list structure is complex,
    // but if you want strict cleanup, you'd need a function to free the ILOC list.
    
    free(tree);
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_add_child(asd_tree_t *tree, asd_tree_t *child)
{
  if (tree != NULL && child != NULL){
    tree->number_of_children++;
    tree->children = realloc(tree->children, tree->number_of_children * sizeof(asd_tree_t*));
    tree->children[tree->number_of_children-1] = child;
  }else{
    // printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, child);
  }
}

asd_tree_t *asd_add_child_to_node(asd_tree_t *node, asd_tree_t *child)
{
  if (node != NULL && child != NULL){
    asd_add_child(node, child);
  }
  return node;
}

asd_tree_t *asd_select_head_and_attach_tail(asd_tree_t *head, asd_tree_t *tail)
{
  if (head != NULL){
    if (tail != NULL) asd_add_child(head, tail);
    return head;
  }
  return tail;
}

asd_tree_t *asd_new_trinary(const char *label, asd_tree_t *left, asd_tree_t *middle, asd_tree_t *right)
{
  asd_tree_t *node = asd_new(label);
  if (node != NULL){
    if (left != NULL) asd_add_child(node, left);
    if (middle != NULL) asd_add_child(node, middle);
    if (right != NULL) asd_add_child(node, right);
  }
  return node;
}

asd_tree_t *asd_new_binary(const char *label, asd_tree_t *left, asd_tree_t *right)
{
  asd_tree_t *node = asd_new(label);
  if (node != NULL){
    if (left != NULL) asd_add_child(node, left);
    if (right != NULL) asd_add_child(node, right);
  }
  return node;
}

asd_tree_t *asd_new_node_from_value(lexical_value_t *value)
{
  if (value == NULL || value->value == NULL) return NULL;
  asd_tree_t *node = asd_new(value->value);
  // Note: we don't free value->value here anymore if it's used elsewhere (like symbol table)
  // But based on your previous code, you were freeing it. 
  // Let's keep strictly to AST logic: copy label, free source if ownership is transferred.
  free(value->value); 
  value->value = NULL;
  return node;
}

asd_tree_t *asd_new_function_call_node(lexical_value_t *id, asd_tree_t *args_root)
{
  const char *name = (id && id->value) ? id->value : "";
  char buffer[256];
  snprintf(buffer, sizeof(buffer), "call %s", name);
  asd_tree_t *node = asd_new(buffer);
  if (id && id->value) {
    free(id->value);
    id->value = NULL;
  }
  if (args_root != NULL) asd_add_child(node, args_root);
  return node;
}

asd_tree_t *asd_new_unary(const char *label, asd_tree_t *child)
{
  asd_tree_t *node = asd_new(label);
  if (node != NULL && child != NULL){
    asd_add_child(node, child);
  }
  return node;
}

static void _asd_print (FILE *foutput, asd_tree_t *tree, int profundidade)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade, profundidade*2, "", tree->label, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++){
      _asd_print(foutput, tree->children[i], profundidade+1);
    }
  }
}

void asd_print(asd_tree_t *tree)
{
  FILE *foutput = stderr;
  if (tree != NULL){
    _asd_print(foutput, tree, 0);
  }
}

static void _asd_print_graphviz (FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    for (i = 0; i < tree->number_of_children; i++){
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz(foutput, tree->children[i]);
    }
  }
}

void asd_print_graphviz(asd_tree_t *tree)
{
  FILE *foutput = stdout;
  if (tree != NULL){
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
  }
}