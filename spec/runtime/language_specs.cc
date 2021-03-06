#include "runtime/runtime_spec_helper.h"
#include <functional>
#include "runtime/length.h"
#include "runtime/helpers/read_test_entries.h"
#include "runtime/helpers/spy_input.h"
#include "runtime/helpers/log_debugger.h"

extern "C" const TSLanguage *ts_language_javascript();
extern "C" const TSLanguage *ts_language_json();
extern "C" const TSLanguage *ts_language_arithmetic();
extern "C" const TSLanguage *ts_language_golang();
extern "C" const TSLanguage *ts_language_c();
extern "C" const TSLanguage *ts_language_cpp();

void expect_the_correct_tree(TSNode node, TSDocument *doc, string tree_string) {
  const char *node_string = ts_node_string(node, doc);
  AssertThat(node_string, Equals(tree_string));
  free((void *)node_string);
}

void expect_a_consistent_tree(TSNode node, TSDocument *doc) {
  TSLength start = ts_node_pos(node);
  TSLength end = ts_length_add(start, ts_node_size(node));
  size_t child_count = ts_node_child_count(node);

  bool has_changes = ts_node_has_changes(node);
  bool some_child_has_changes = false;

  for (size_t i = 0; i < child_count; i++) {
    TSNode child = ts_node_child(node, i);
    TSLength child_start = ts_node_pos(child);
    TSLength child_end = ts_length_add(child_start, ts_node_size(child));

    AssertThat(child_start.chars, IsGreaterThan(start.chars) || Equals(start.chars));
    AssertThat(child_end.chars, IsLessThan(end.chars) || Equals(end.chars));
    if (ts_node_has_changes(child))
      some_child_has_changes = true;
  }

  if (child_count > 0)
    AssertThat(has_changes, Equals(some_child_has_changes));
}

START_TEST

map<string, const TSLanguage *> languages({
  {"json", ts_language_json()},
  {"arithmetic", ts_language_arithmetic()},
  {"javascript", ts_language_javascript()},
  {"golang", ts_language_golang()},
  {"c", ts_language_c()},
  {"cpp", ts_language_cpp()},
});

describe("Languages", [&]() {
  TSDocument *doc;

  before_each([&]() {
    doc = ts_document_make();
  });

  after_each([&]() {
    ts_document_free(doc);
  });

  for (const auto &pair : languages) {
    describe(("The " + pair.first + " parser").c_str(), [&]() {
      before_each([&]() {
        ts_document_set_language(doc, pair.second);
        // ts_document_set_debugger(doc, log_debugger_make(false));
      });

      for (auto &entry : test_entries_for_language(pair.first)) {
        SpyInput *input;

        auto it_handles_edit_sequence = [&](string name, std::function<void()> edit_sequence){
          it(("parses " + entry.description + ": " + name).c_str(), [&]() {
            input = new SpyInput(entry.input, 3);
            ts_document_set_input(doc, input->input());
            edit_sequence();
            TSNode root_node = ts_document_root_node(doc);
            expect_the_correct_tree(root_node, doc, entry.tree_string);
            expect_a_consistent_tree(root_node, doc);
            delete input;
          });
        };

        it_handles_edit_sequence("initial parse", [&]() {
          ts_document_parse(doc);
        });

        it_handles_edit_sequence("repairing an inserted error", [&]() {
          ts_document_edit(doc, input->replace(entry.input.size() / 2, 0, "%^&*"));
          ts_document_parse(doc);

          ts_document_edit(doc, input->undo());
          ts_document_parse(doc);
        });

        it_handles_edit_sequence("creating and repairing an inserted error", [&]() {
          ts_document_parse(doc);

          ts_document_edit(doc, input->replace(entry.input.size() / 2, 0, "%^&*"));
          ts_document_parse(doc);

          ts_document_edit(doc, input->undo());
          ts_document_parse(doc);
        });

        it_handles_edit_sequence("repairing an errant deletion", [&]() {
          ts_document_parse(doc);

          ts_document_edit(doc, input->replace(entry.input.size() / 2, 5, ""));
          ts_document_parse(doc);

          ts_document_edit(doc, input->undo());
          ts_document_parse(doc);
        });

        it_handles_edit_sequence("creating and repairing an errant deletion", [&]() {
          ts_document_edit(doc, input->replace(entry.input.size() / 2, 5, ""));
          ts_document_parse(doc);

          ts_document_edit(doc, input->undo());
          ts_document_parse(doc);
        });
      }
    });
  }
});

END_TEST
