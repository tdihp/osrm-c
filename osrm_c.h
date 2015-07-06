#ifndef OSRM_C_H
#define OSRM_C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* object declearations */
typedef struct osrm_t               osrm_t;
typedef struct osrm_config_t        osrm_config_t;
typedef struct osrm_query_t         osrm_query_t;
typedef struct osrm_json_handler_t  osrm_json_handler_t;
typedef struct osrm_json_renderer_t osrm_json_renderer_t;

/* osrm_t */
extern osrm_t* osrm_create(const osrm_config_t* config, char** errptr);
extern void osrm_destroy(osrm_t*);
extern void osrm_run_query(
    osrm_t*,
    osrm_query_t* query,
    int* code,
    osrm_json_handler_t* json_handler,
    char** errptr);

/* osrm_config_t */
extern osrm_config_t* osrm_config_create();
extern void osrm_config_destroy(osrm_config_t*);
extern void osrm_config_set_path(osrm_config_t*, const char* key, const char* path);
extern void osrm_config_set_max_locations_distance_table(osrm_config_t*, int);
extern void osrm_config_set_max_locations_map_matching(osrm_config_t*, int);
extern void osrm_config_set_use_shared_memory(osrm_config_t*, unsigned char);

/* osrm_query_t */
extern osrm_query_t* osrm_query_create();
extern void osrm_query_destroy(osrm_query_t*);
extern void osrm_query_set_zoom_level(osrm_query_t*, short);
extern void osrm_query_set_number_of_results(osrm_query_t*, short);
extern void osrm_query_set_alternate_route_flag(osrm_query_t*, unsigned char);
extern void osrm_query_set_uturn(osrm_query_t*, unsigned char);
extern void osrm_query_set_classify(osrm_query_t*, unsigned char);
extern void osrm_query_set_matching_beta(osrm_query_t*, double);
extern void osrm_query_set_gps_precision(osrm_query_t*, double);
extern void osrm_query_set_checksum(osrm_query_t*, unsigned);
extern void osrm_query_set_instruction_flag(osrm_query_t*, unsigned char);
extern void osrm_query_set_service(osrm_query_t*, const char*);
extern void osrm_query_add_hint(osrm_query_t*, const char*);
extern void osrm_query_add_timestamp(osrm_query_t*, unsigned);
extern void osrm_query_set_language(osrm_query_t*, const char*);
extern void osrm_query_set_geometry_flag(osrm_query_t*, unsigned char);
extern void osrm_query_set_compression_flag(osrm_query_t*, unsigned char);
extern void osrm_query_add_coordinate(osrm_query_t*, double lat, double lon);

/* osrm_json_handler_t

A simple minimalist json streaming interface
*/
extern osrm_json_handler_t* osrm_json_handler_create(
    void* state,
    void (*push_object)(void*),
    void (*push_array)(void*),
    void (*pop)(void*),
    void (*append_string)(void*, const char*, size_t),
    void (*append_number)(void*, double),
    void (*append_bool)(void*, unsigned char),
    void (*append_null)(void*));
extern void osrm_json_handler_destroy(osrm_json_handler_t*);

/* osrm_json_renderer_t

A json renderer
*/
extern osrm_json_renderer_t* osrm_json_renderer_create();
extern void osrm_json_renderer_destroy(osrm_json_renderer_t*);
/* return value is size, the returned string is not zero-terminated */
extern size_t osrm_json_renderer_harvest(osrm_json_renderer_t*, char**);
extern osrm_json_handler_t* osrm_json_renderer_create_handler(
    osrm_json_renderer_t*);


#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif /* OSRM_C_H */