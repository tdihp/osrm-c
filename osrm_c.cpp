#include "osrm_c.h"

#include <osrm/json_container.hpp>
#include <osrm/libosrm_config.hpp>
#include <osrm/osrm.hpp>
#include <osrm/route_parameters.hpp>

#include <string.h>
#include <boost/fusion/container/vector.hpp>

extern "C" {

struct osrm_t                   { OSRM*             rep; };
struct osrm_config_t            { libosrm_config*   rep; };
struct osrm_query_t  { RouteParameters*  rep; };
struct osrm_paths_t      { ServerPaths*      rep; };
struct osrm_json_handler_t {
    void* state;
    void (*push_object)(void*);
    void (*push_array)(void*);
    void (*pop)(void*);
    void (*append_string)(void*, const char*, size_t);
    void (*append_number)(void*, double);
    void (*append_bool)(void*, unsigned char);
    void (*append_null)(void*);
};

void WriteError(const char* errstr, char** errptr) {
  assert(errptr != NULL);
  if (*errptr) {
    free(*errptr);
  }
  *errptr = (char*)malloc(strlen(errstr) + 1);
  strcpy(*errptr, errstr);
}

struct JsonVisitor : mapbox::util::static_visitor<> {
  explicit JsonVisitor(osrm_json_handler_t* handler) : handler_(handler) {
  }

  void operator()(const osrm::json::String &string) const {
    handler_->append_string(
      handler_->state, string.value.data(), string.value.size());
  }

  void operator()(const osrm::json::Number &number) const {
    handler_->append_number(handler_->state, number.value);
  }

  void operator()(const osrm::json::Object &object) const {
    handler_->push_object(handler_->state);
    for (const auto& keyValue : object.values) {
      handler_->append_string(
        handler_->state, keyValue.first.data(), keyValue.first.size());
      mapbox::util::apply_visitor<osrm::json::Value, JsonVisitor>(*this, keyValue.second);
    }
    handler_->pop(handler_->state);
  }

  void operator()(const osrm::json::Array &array) const
  {
    handler_->push_array(handler_->state);
    for (auto i = 0u; i < array.values.size(); ++i) {
      mapbox::util::apply_visitor<osrm::json::Value, JsonVisitor>(*this, array.values[i]);
    }
    handler_->pop(handler_->state);
  }

  void operator()(const osrm::json::True &) const {
    handler_->append_bool(handler_->state, true);
  }

  void operator()(const osrm::json::False &) const {
    handler_->append_bool(handler_->state, false);
  }

  void operator()(const osrm::json::Null &) const {
    handler_->append_null(handler_->state);
  }
private:
  osrm_json_handler_t* handler_;
};

/* osrm_t */
osrm_t* osrm_create(const osrm_config_t* config, char** errptr) {
  osrm_t* engine = new osrm_t;
  try {
    engine->rep = new OSRM(*config->rep);
  } catch (std::exception const& ex) {
    WriteError(ex.what(), errptr);
    engine = NULL;
  }
  return engine;
}

void osrm_destroy(osrm_t* engine) {
  delete engine->rep;
  delete engine;
}

void osrm_run_query(
    osrm_t* engine,
    osrm_query_t* query,
    int* code,
    osrm_json_handler_t* json_handler,
    char** errptr) {
  osrm::json::Object json_object;
  int result_code = 500;
  try {
    result_code = engine->rep->RunQuery(*query->rep, json_object);
  } catch (std::exception const& ex) {
    WriteError(ex.what(), errptr);
  }
  if (code != NULL) {
    *code = result_code;
  }
  if (json_handler != NULL) {
    JsonVisitor visitor(json_handler);
    mapbox::util::apply_visitor<osrm::json::Value, JsonVisitor>(
      visitor, json_object);
  }
}

/* osrm_config_t */
osrm_config_t* osrm_config_create() {
  osrm_config_t* config = new osrm_config_t;
  config->rep = new libosrm_config;
  return config;
}

void osrm_config_destroy(osrm_config_t* config) {
  delete config->rep;
  delete config;
}

void osrm_config_set_path(
    osrm_config_t* config, const char* key, const char* path) {
  config->rep->server_paths[key] = path;
}

void osrm_config_set_max_locations_distance_table(
    osrm_config_t* config,
    int value) {
  config->rep->max_locations_distance_table = value;
}

void osrm_config_set_max_locations_map_matching(
    osrm_config_t* config,
    int value) {
  config->rep->max_locations_map_matching = value;
}

void osrm_config_set_use_shared_memory(
    osrm_config_t* config,
    unsigned char value) {
  config->rep->use_shared_memory = value;
}

/* osrm_query_t */
osrm_query_t* osrm_query_create() {
  osrm_query_t* params = new osrm_query_t;
  params->rep = new RouteParameters;
  return params;
}

void osrm_query_destroy(osrm_query_t* params) {
  delete params->rep;
  delete params;
}

void osrm_query_set_zoom_level(osrm_query_t* params, short value) {
  params->rep->setZoomLevel(value);
}

void osrm_query_set_number_of_results(osrm_query_t* params, short value) {
  params->rep->setNumberOfResults(value);
}

void osrm_query_set_alternate_route_flag(osrm_query_t* params, unsigned char value) {
  params->rep->setAlternateRouteFlag(value);
}

void osrm_query_set_uturn(osrm_query_t* params, unsigned char value) {
  params->rep->setUTurn(value);
}

void osrm_query_set_classify(osrm_query_t* params, unsigned char value) {
  params->rep->setClassify(value);
}

void osrm_query_set_matching_beta(osrm_query_t* params, double value) {
  params->rep->setMatchingBeta(value);
}

void osrm_query_set_gps_precision(osrm_query_t* params, double value) {
  params->rep->setGPSPrecision(value);
}

void osrm_query_set_checksum(osrm_query_t* params, unsigned value) {
  params->rep->setChecksum(value);
}

void osrm_query_set_instruction_flag(osrm_query_t* params, unsigned char value) {
  params->rep->setInstructionFlag(value);
}

void osrm_query_set_service(osrm_query_t* params, const char* value) {
  params->rep->setService(value);
}

void osrm_query_add_hint(osrm_query_t* params, const char* value) {
  params->rep->addHint(value);
}

void osrm_query_add_timestamp(osrm_query_t* params, unsigned value) {
  params->rep->addTimestamp(value);
}

void osrm_query_set_language(osrm_query_t* params, const char* value) {
  params->rep->setLanguage(value);
}

void osrm_query_set_geometry_flag(osrm_query_t* params, unsigned char value) {
  params->rep->setGeometryFlag(value);
}

void osrm_query_set_compression_flag(osrm_query_t* params, unsigned char value) {
  params->rep->setCompressionFlag(value);
}

void osrm_query_add_coordinate(osrm_query_t* params, double lat, double lon) {
  const boost::fusion::vector<double, double> received_coordinates(lat, lon);
  params->rep->addCoordinate(received_coordinates);
}

/* osrm_json_handler_t */
osrm_json_handler_t* osrm_json_handler_create(
    void* state,
    void (*push_object)(void*),
    void (*push_array)(void*),
    void (*pop)(void*),
    void (*append_string)(void*, const char*, size_t),
    void (*append_number)(void*, double),
    void (*append_bool)(void*, unsigned char),
    void (*append_null)(void*)) {
  osrm_json_handler_t* handler = new osrm_json_handler_t;
  handler->state = state;
  handler->push_object = push_object;
  handler->push_array = push_array;
  handler->pop = pop;
  handler->append_string = append_string;
  handler->append_number = append_number;
  handler->append_bool = append_bool;
  handler->append_null = append_null;
  return handler;
}

void osrm_json_handler_destroy(osrm_json_handler_t* handler) {
  delete handler;
}

}

