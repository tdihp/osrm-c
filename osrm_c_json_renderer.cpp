#include "osrm_c.h"
#include <stack>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// copied from osrm-backend
std::string escape_JSON(const std::string &input)
{
  // escape and skip reallocations if possible
  std::string output;
  output.reserve(input.size() + 4); // +4 assumes two backslashes on avg
  for (const char letter : input)
  {
    switch (letter)
    {
    case '\\':
      output += "\\\\";
      break;
    case '"':
      output += "\\\"";
      break;
    case '/':
      output += "\\/";
      break;
    case '\b':
      output += "\\b";
      break;
    case '\f':
      output += "\\f";
      break;
    case '\n':
      output += "\\n";
      break;
    case '\r':
      output += "\\r";
      break;
    case '\t':
      output += "\\t";
      break;
    default:
      output.append(1, letter);
      break;
    }
  }
  return output;
}


extern "C" {
  
enum Status {
  ERROR,
  VALUE,            // waiting any value
  OBJECT,           // at clean state of object
  OBJECT_HALF,      // object with only key filled
  OBJECT_CONTINUE,  // object with some pairs
  ARRAY,            // at clean state of array
  ARRAY_CONTINUE,   // array with some elements
};

struct osrm_json_renderer_t {
  explicit osrm_json_renderer_t() {
    stack_.push(VALUE);
  }
  std::ostringstream stream_;
  std::stack<Status> stack_;
};

unsigned char ValueSwitch(osrm_json_renderer_t* json_state) {
  // used for anything that's capable to be a value
  if (json_state->stack_.empty()) {
    json_state->stack_.push(ERROR);
    return false;
  }
  switch (json_state->stack_.top()) {
    case VALUE:
      json_state->stack_.pop();
      break;
    case OBJECT_HALF:
      json_state->stream_ << ':';
      json_state->stack_.pop();
      json_state->stack_.push(OBJECT_CONTINUE);
      break;
    case ARRAY:
      json_state->stack_.pop();
      json_state->stack_.push(ARRAY_CONTINUE);
      break;
    case ARRAY_CONTINUE:
      json_state->stream_ << ',';
      break;
    case OBJECT_CONTINUE:
    case OBJECT:
      json_state->stack_.push(ERROR);
    case ERROR:
      return false;
  }
  return true;
}

void push_object(void* state) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (ValueSwitch(json_state)) {
    json_state->stream_ << '{';
    json_state->stack_.push(OBJECT);
  }
}

void push_array(void* state) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (ValueSwitch(json_state)) {
    json_state->stream_ << '[';
    json_state->stack_.push(ARRAY);
  }
}

void pop(void* state) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (json_state->stack_.empty()) {
    json_state->stack_.push(ERROR);
    return;
  }
  switch (json_state->stack_.top()) {
    case OBJECT:
    case OBJECT_CONTINUE:
      json_state->stream_ << '}';
      break;
    case ARRAY:
    case ARRAY_CONTINUE:
      json_state->stream_ << ']';
      break;
    case VALUE:
    case OBJECT_HALF:
      json_state->stack_.push(ERROR);
    case ERROR:
      return;
  }
  json_state->stack_.pop();
}

void append_string(void* state, const char* data, size_t size) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (json_state->stack_.empty()) {
    json_state->stack_.push(ERROR);
    return;
  }
  switch (json_state->stack_.top()) {
    case VALUE:
      json_state->stack_.pop();
      break;
    case OBJECT_CONTINUE:
      json_state->stream_ << ',';
    case OBJECT:
      json_state->stack_.pop();
      json_state->stack_.push(OBJECT_HALF);
      break;
    case OBJECT_HALF:
      json_state->stream_ << ':';
      json_state->stack_.pop();
      json_state->stack_.push(OBJECT_CONTINUE);
      break;
    case ARRAY:
      json_state->stack_.pop();
      json_state->stack_.push(ARRAY_CONTINUE);
      break;
    case ARRAY_CONTINUE:
      json_state->stream_ << ',';
      break;
    case ERROR:
      return;
  }
  const std::string value(data, size);
  json_state->stream_ << '\"';
  json_state->stream_ << escape_JSON(value);
  json_state->stream_ << '\"';
}

void append_number(void* state, double value) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (ValueSwitch(json_state)) {
    json_state->stream_.precision(10);
    json_state->stream_ << value;
  }
}

void append_bool(void* state, unsigned char value) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (ValueSwitch(json_state)) {
    json_state->stream_ << (value?"true":"false");
  }
}

void append_null(void* state) {
  osrm_json_renderer_t* json_state = (osrm_json_renderer_t*)state;
  if (ValueSwitch(json_state)) {
    json_state->stream_ << "null";
  }
}

osrm_json_renderer_t* osrm_json_renderer_create() {
  return new osrm_json_renderer_t;
}

void osrm_json_renderer_destroy(osrm_json_renderer_t* renderer) {
  delete renderer;
}

size_t osrm_json_renderer_harvest(
    osrm_json_renderer_t* renderer, char** dataptr) {
  assert(dataptr != NULL);
  size_t size = 0;
  if (renderer->stack_.empty()) {
    if(*dataptr != NULL) {
      free(dataptr);
    }
    std::string value = renderer->stream_.str();
    size = value.size();
    *dataptr = (char*)malloc(size);
    memcpy(*dataptr, value.data(), size);
  }
  return size;
}

osrm_json_handler_t* osrm_json_renderer_create_handler(
    osrm_json_renderer_t* renderer) {
  return osrm_json_handler_create(
    (void*)renderer,
    &push_object,
    &push_array,
    &pop,
    &append_string,
    &append_number,
    &append_bool,
    &append_null);
}

}