#ifndef OSRM_C_H
#define OSRM_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* object declearations */
typedef struct osrm_t                   osrm_t;
typedef struct osrm_config_t            osrm_config_t;
typedef struct osrm_route_parameters_t  osrm_route_parameters_t;
typedef struct osrm_server_paths_t      osrm_server_paths_t;
typedef struct osrm_json_handler_t      osrm_json_handler_t;

/* osrm_t */
extern osrm_t* osrm_create(const osrm_config_t* config, char** errptr);
extern void osrm_destroy(osrm_t*);
extern void osrm_run_query(
    osrm_t*,
    osrm_route_parameters_t* query,
    int* code,
    osrm_json_handler_t* json_handler,
    char** errptr);

/* osrm_config_t */
extern osrm_config_t* osrm_config_create();
extern void osrm_config_destroy(osrm_config_t*);
extern void osrm_config_set_server_paths(
    osrm_config_t*, const osrm_server_paths_t* server_paths);
extern void osrm_config_set_max_locations_distance_table(osrm_config_t*, int);
extern void osrm_config_set_max_locations_map_matching(osrm_config_t*, int);
extern void osrm_config_set_use_shared_memory(osrm_config_t*, bool);

/* osrm_route_parameters_t */
extern osrm_route_parameters_t* osrm_route_parameter_create();
extern void osrm_route_parameter_destroy(osrm_route_parameters_t*);
extern void osrm_route_parameter_set_zoom_level(
    osrm_route_parameters_t*, short);
extern void osrm_route_parameter_set_number_of_results(
    osrm_route_parameters_t*, short);
extern void osrm_route_parameter_set_alternate_route_flag(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_uturn(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_all_uturns(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_classify(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_matching_beta(
    osrm_route_parameters_t*, double);
extern void osrm_route_parameter_set_gps_precision(
    osrm_route_parameters_t*, double);
extern void osrm_route_parameter_set_deprecated_api_flag(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_checksum(
    osrm_route_parameters_t*, unsigned);
extern void osrm_route_parameter_set_instruction_flag(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_service(
    osrm_route_parameters_t*, const char*);
extern void osrm_route_parameter_set_output_format(
    osrm_route_parameters_t*, const char*);
extern void osrm_route_parameter_set_jsonp_parameter(
    osrm_route_parameters_t*, const char*);
extern void osrm_route_parameter_add_hint(
    osrm_route_parameters_t*, const char*);
extern void osrm_route_parameter_add_timestamp(
    osrm_route_parameters_t*, unsigned);
extern void osrm_route_parameter_set_language(
    osrm_route_parameters_t*, const char*);
extern void osrm_route_parameter_set_geometry_flag(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_set_compression_flag(
    osrm_route_parameters_t*, bool);
extern void osrm_route_parameter_add_coordinate(
    osrm_route_parameters_t*, double lat, double lon);
extern void osrm_route_parameter_get_coordinates_from_geometry(
    osrm_route_parameters_t*, const char*);

/* osrm_server_path_t */
extern osrm_server_paths_t* osrm_server_paths_create();
extern void osrm_server_paths_destroy(osrm_server_paths_t*);
extern void osrm_server_paths_set(
    osrm_server_paths_t*, const char* key, const char* path);

/* osrm_json_handler_t */
extern osrm_json_handler_t* osrm_json_handler_create(
    void* state,
    void (*push_object)(void*),
    void (*push_array)(void*),
    void (*pop)(void*),
    void (*append_string)(void*, const char*, size_t),
    void (*append_number)(void*, double),
    void (*append_bool)(void*, bool),
    void (*append_null)(void*));

extern void osrm_json_handler_destroy(osrm_json_handler_t*);


#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif /* OSRM_C_H */