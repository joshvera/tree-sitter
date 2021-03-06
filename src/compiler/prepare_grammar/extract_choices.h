#ifndef COMPILER_PREPARE_GRAMMAR_EXTRACT_CHOICES_H_
#define COMPILER_PREPARE_GRAMMAR_EXTRACT_CHOICES_H_

#include <vector>
#include "tree_sitter/compiler.h"

namespace tree_sitter {
namespace prepare_grammar {

std::vector<rule_ptr> extract_choices(const rule_ptr &);

}  // namespace prepare_grammar
}  // namespace tree_sitter

#endif  // COMPILER_PREPARE_GRAMMAR_EXTRACT_CHOICES_H_
