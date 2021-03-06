#include "compiler/compiler_spec_helper.h"
#include "tree_sitter/compiler.h"

using namespace rules;

START_TEST

describe("Compile", []() {
  describe("when the grammar's start symbol is a token", [&]() {
    it("does not fail", [&]() {
      Grammar grammar({
        { "rule1", str("the-value") }
      });

      auto result = compile(grammar, "test_grammar");
      const GrammarError *error = result.second;
      AssertThat(error, Equals<const GrammarError *>(nullptr));
    });
  });

  describe("when the grammar's start symbol is blank", [&]() {
    it("does not fail", [&]() {
      Grammar grammar({
        { "rule1", blank() }
      });

      auto result = compile(grammar, "test_grammar");
      const GrammarError *error = result.second;
      AssertThat(error, Equals<const GrammarError *>(nullptr));
    });
  });
});

END_TEST
