#include "runtime/string_input.h"
#include <string.h>

typedef struct {
  const char *string;
  size_t position;
  size_t length;
} TSStringInput;

const char *ts_string_input_read(void *payload, size_t *bytes_read) {
  TSStringInput *input = (TSStringInput *)payload;
  if (input->position >= input->length) {
    *bytes_read = 0;
    return "";
  }
  size_t previous_position = input->position;
  input->position = input->length;
  *bytes_read = input->position - previous_position;
  return input->string + previous_position;
}

int ts_string_input_seek(void *payload, TSLength position) {
  TSStringInput *input = (TSStringInput *)payload;
  input->position = position.bytes;
  return (position.bytes < input->length);
}

TSInput ts_string_input_make(const char *string) {
  TSStringInput *input = malloc(sizeof(TSStringInput));
  input->string = string;
  input->position = 0;
  input->length = strlen(string);
  return (TSInput){
    .payload = input,
    .read_fn = ts_string_input_read,
    .seek_fn = ts_string_input_seek,
  };
}
