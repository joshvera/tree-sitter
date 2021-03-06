#include "compiler/parse_table.h"
#include <string>
#include "compiler/precedence_range.h"

namespace tree_sitter {

using std::string;
using std::ostream;
using std::to_string;
using std::set;
using std::vector;
using rules::Symbol;

ParseAction::ParseAction(ParseActionType type, ParseStateId state_index,
                         Symbol symbol, size_t consumed_symbol_count,
                         PrecedenceRange precedence_range,
                         rules::Associativity associativity,
                         const Production *production)
    : type(type),
      symbol(symbol),
      state_index(state_index),
      consumed_symbol_count(consumed_symbol_count),
      precedence_range(precedence_range),
      associativity(associativity),
      production(production) {}

ParseAction::ParseAction()
    : type(ParseActionTypeError),
      symbol(Symbol(-1)),
      state_index(-1),
      consumed_symbol_count(0),
      associativity(rules::AssociativityNone) {}

ParseAction ParseAction::Error() {
  return ParseAction();
}

ParseAction ParseAction::Accept() {
  ParseAction action;
  action.type = ParseActionTypeAccept;
  return action;
}

ParseAction ParseAction::Shift(ParseStateId state_index,
                               PrecedenceRange precedence_range) {
  return ParseAction(ParseActionTypeShift, state_index, Symbol(-1), 0,
                     precedence_range, rules::AssociativityNone, nullptr);
}

ParseAction ParseAction::ShiftExtra() {
  ParseAction action;
  action.type = ParseActionTypeShiftExtra;
  return action;
}

ParseAction ParseAction::ReduceExtra(Symbol symbol) {
  ParseAction action;
  action.type = ParseActionTypeReduceExtra;
  action.symbol = symbol;
  return action;
}

ParseAction ParseAction::Reduce(Symbol symbol, size_t consumed_symbol_count,
                                int precedence,
                                rules::Associativity associativity,
                                const Production &production) {
  return ParseAction(ParseActionTypeReduce, 0, symbol, consumed_symbol_count,
                     { precedence, precedence }, associativity, &production);
}

bool ParseAction::operator==(const ParseAction &other) const {
  bool types_eq = type == other.type;
  bool symbols_eq = symbol == other.symbol;
  bool state_indices_eq = state_index == other.state_index;
  bool consumed_symbol_counts_eq =
    consumed_symbol_count == other.consumed_symbol_count;
  bool precedences_eq = precedence_range == other.precedence_range;
  return types_eq && symbols_eq && state_indices_eq &&
         consumed_symbol_counts_eq && precedences_eq;
}

bool ParseAction::operator<(const ParseAction &other) const {
  if (type < other.type)
    return true;
  if (other.type < type)
    return false;
  if (symbol < other.symbol)
    return true;
  if (other.symbol < symbol)
    return false;
  if (state_index < other.state_index)
    return true;
  if (other.state_index < state_index)
    return false;
  return consumed_symbol_count < other.consumed_symbol_count;
}

ParseState::ParseState() : lex_state_id(-1) {}

set<Symbol> ParseState::expected_inputs() const {
  set<Symbol> result;
  for (auto &pair : actions)
    result.insert(pair.first);
  return result;
}

ParseStateId ParseTable::add_state() {
  states.push_back(ParseState());
  return states.size() - 1;
}

ParseAction &ParseTable::set_action(ParseStateId id, Symbol symbol,
                                    ParseAction action) {
  symbols.insert(symbol);
  states[id].actions[symbol] = vector<ParseAction>({ action });
  return *states[id].actions[symbol].begin();
}

ParseAction &ParseTable::add_action(ParseStateId id, Symbol symbol,
                                    ParseAction action) {
  symbols.insert(symbol);
  states[id].actions[symbol].push_back(action);
  return *states[id].actions[symbol].rbegin();
}

}  // namespace tree_sitter
