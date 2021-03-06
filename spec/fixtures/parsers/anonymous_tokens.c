#include "tree_sitter/parser.h"

#define STATE_COUNT 3
#define SYMBOL_COUNT 7

enum {
    sym_program = ts_builtin_sym_start,
    anon_sym_LF,
    anon_sym_CR,
    aux_sym_SLASH_BSLASHd_SLASH,
    anon_sym_DQUOTEhello_DQUOTE,
};

static const char *ts_symbol_names[] = {
    [sym_program] = "program",
    [ts_builtin_sym_error] = "ERROR",
    [ts_builtin_sym_end] = "END",
    [anon_sym_LF] = "\n",
    [anon_sym_CR] = "\r",
    [aux_sym_SLASH_BSLASHd_SLASH] = "/\\d/",
    [anon_sym_DQUOTEhello_DQUOTE] = "\"hello\"",
};

static const TSNodeType ts_node_types[SYMBOL_COUNT] = {
    [sym_program] = TSNodeTypeNamed,
    [ts_builtin_sym_error] = TSNodeTypeNamed,
    [ts_builtin_sym_end] = TSNodeTypeHidden,
    [anon_sym_LF] = TSNodeTypeAnonymous,
    [anon_sym_CR] = TSNodeTypeAnonymous,
    [aux_sym_SLASH_BSLASHd_SLASH] = TSNodeTypeHidden,
    [anon_sym_DQUOTEhello_DQUOTE] = TSNodeTypeAnonymous,
};

static TSTree *ts_lex(TSLexer *lexer, TSStateId lex_state) {
    START_LEXER();
    switch (lex_state) {
        case 1:
            START_TOKEN();
            if ((lookahead == '\t') ||
                (lookahead == ' '))
                ADVANCE(1);
            if (lookahead == '\n')
                ADVANCE(2);
            if (lookahead == '\r')
                ADVANCE(3);
            if (lookahead == '\"')
                ADVANCE(4);
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(11);
            LEX_ERROR();
        case 2:
            START_TOKEN();
            ACCEPT_TOKEN(anon_sym_LF);
        case 3:
            START_TOKEN();
            ACCEPT_TOKEN(anon_sym_CR);
        case 4:
            if (lookahead == 'h')
                ADVANCE(5);
            LEX_ERROR();
        case 5:
            if (lookahead == 'e')
                ADVANCE(6);
            LEX_ERROR();
        case 6:
            if (lookahead == 'l')
                ADVANCE(7);
            LEX_ERROR();
        case 7:
            if (lookahead == 'l')
                ADVANCE(8);
            LEX_ERROR();
        case 8:
            if (lookahead == 'o')
                ADVANCE(9);
            LEX_ERROR();
        case 9:
            if (lookahead == '\"')
                ADVANCE(10);
            LEX_ERROR();
        case 10:
            ACCEPT_TOKEN(anon_sym_DQUOTEhello_DQUOTE);
        case 11:
            ACCEPT_TOKEN(aux_sym_SLASH_BSLASHd_SLASH);
        case 12:
            START_TOKEN();
            if (lookahead == 0)
                ADVANCE(13);
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                ADVANCE(12);
            LEX_ERROR();
        case 13:
            ACCEPT_TOKEN(ts_builtin_sym_end);
        case 14:
            START_TOKEN();
            if (lookahead == 0)
                ADVANCE(13);
            if ((lookahead == '\t') ||
                (lookahead == ' '))
                ADVANCE(14);
            if (lookahead == '\n')
                ADVANCE(15);
            if (lookahead == '\r')
                ADVANCE(16);
            if (lookahead == '\"')
                ADVANCE(4);
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(11);
            LEX_ERROR();
        case 15:
            START_TOKEN();
            ACCEPT_TOKEN(anon_sym_LF);
        case 16:
            START_TOKEN();
            ACCEPT_TOKEN(anon_sym_CR);
        case ts_lex_state_error:
            START_TOKEN();
            if (lookahead == 0)
                ADVANCE(13);
            if ((lookahead == '\t') ||
                (lookahead == ' '))
                ADVANCE(14);
            if (lookahead == '\n')
                ADVANCE(15);
            if (lookahead == '\r')
                ADVANCE(16);
            if (lookahead == '\"')
                ADVANCE(4);
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(11);
            LEX_ERROR();
        default:
            LEX_ERROR();
    }
}

static TSStateId ts_lex_states[STATE_COUNT] = {
    [0] = 1,
    [1] = 12,
    [2] = 12,
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

static const TSParseAction *ts_parse_actions[STATE_COUNT][SYMBOL_COUNT] = {
    [0] = {
        [sym_program] = ACTIONS(SHIFT(1)),
        [anon_sym_LF] = ACTIONS(SHIFT(2)),
        [anon_sym_CR] = ACTIONS(SHIFT(2)),
        [aux_sym_SLASH_BSLASHd_SLASH] = ACTIONS(SHIFT(2)),
        [anon_sym_DQUOTEhello_DQUOTE] = ACTIONS(SHIFT(2)),
    },
    [1] = {
        [ts_builtin_sym_end] = ACTIONS(ACCEPT_INPUT()),
    },
    [2] = {
        [ts_builtin_sym_end] = ACTIONS(REDUCE(sym_program, 1)),
    },
};

#pragma GCC diagnostic pop

EXPORT_LANGUAGE(ts_language_anonymous_tokens);
