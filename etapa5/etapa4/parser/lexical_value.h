#ifndef LEXICAL_VALUE_H
#define LEXICAL_VALUE_H

typedef struct {
  int line_number;
  const char *token_type;
  char *value;
} lexical_value_t;

#endif // LEXICAL_VALUE_H

