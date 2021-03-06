#ifndef RUNTIME_PARSER_H_
#define RUNTIME_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "runtime/stack.h"

typedef struct {
  TSLexer lexer;
  Stack *stack;
  TSTree *lookahead;
  TSTree *reusable_subtree;
  size_t reusable_subtree_pos;
  const TSLanguage *language;
} TSParser;

TSParser ts_parser_make();
void ts_parser_destroy(TSParser *);
TSDebugger ts_parser_debugger(const TSParser *);
void ts_parser_set_debugger(TSParser *, TSDebugger);
TSTree *ts_parser_parse(TSParser *, TSInput, TSTree *);

#ifdef __cplusplus
}
#endif

#endif  // RUNTIME_PARSER_H_
