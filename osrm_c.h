#ifndef OSRM_C_H
#define OSRM_C_H

#ifdef __cplusplus
extern "C" {
#endif

/* DLL export macro copied from geos */
#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || \
    defined( __BCPLUSPLUS__)  || defined( __MWERKS__)

#  if defined(OSRM_DLL_EXPORT)
#    define OSRM_DLL __declspec(dllexport)
#  elif defined(OSRM_DLL_IMPORT)
#    define OSRM_DLL __declspec(dllimport)
#  else
#    define OSRM_DLL
#  endif
#else
#  define OSRM_DLL
#endif

#include <stddef.h>

/* object declearations */
typedef struct osrm_t               osrm_t;
typedef struct osrm_config_t        osrm_config_t;
typedef struct osrm_query_t         osrm_query_t;
typedef struct osrm_json_handler_t  osrm_json_handler_t;
typedef struct osrm_json_renderer_t osrm_json_renderer_t;

/* versioning interface */
extern OSRM_DLL const char* osrm_version();  /*libOSRM version*/
extern OSRM_DLL const char* osrm_c_version();  /*version of this c wrapper*/

/* osrm_t */
extern OSRM_DLL osrm_t* osrm_create(const osrm_config_t* config, char** errptr);
extern OSRM_DLL void osrm_destroy(osrm_t*);
extern OSRM_DLL void osrm_run_query(
    osrm_t*,
    osrm_query_t* query,
    int* code,
    osrm_json_handler_t* json_handler,
    char** errptr);

/* osrm_config_t */
extern OSRM_DLL osrm_config_t* osrm_config_create();
extern OSRM_DLL void osrm_config_destroy(osrm_config_t*);
extern OSRM_DLL void osrm_config_set_path(
    osrm_config_t*, const char* key, const char* path);
extern OSRM_DLL void osrm_config_set_max_locations_distance_table(
    osrm_config_t*, int);
extern OSRM_DLL void osrm_config_set_max_locations_map_matching(
    osrm_config_t*, int);
extern OSRM_DLL void osrm_config_set_use_shared_memory(
    osrm_config_t*, unsigned char);

/* osrm_query_t */
extern OSRM_DLL osrm_query_t* osrm_query_create();
extern OSRM_DLL void osrm_query_destroy(osrm_query_t*);
extern OSRM_DLL void osrm_query_set_zoom_level(osrm_query_t*, short);
extern OSRM_DLL void osrm_query_set_number_of_results(osrm_query_t*, short);
extern OSRM_DLL void osrm_query_set_alternate_route_flag(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_set_uturn(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_set_classify(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_set_matching_beta(osrm_query_t*, double);
extern OSRM_DLL void osrm_query_set_gps_precision(osrm_query_t*, double);
extern OSRM_DLL void osrm_query_set_checksum(osrm_query_t*, unsigned);
extern OSRM_DLL void osrm_query_set_instruction_flag(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_set_service(osrm_query_t*, const char*);
extern OSRM_DLL void osrm_query_add_hint(osrm_query_t*, const char*);
extern OSRM_DLL void osrm_query_add_timestamp(osrm_query_t*, unsigned);
extern OSRM_DLL void osrm_query_set_language(osrm_query_t*, const char*);
extern OSRM_DLL void osrm_query_set_geometry_flag(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_set_compression_flag(osrm_query_t*, unsigned char);
extern OSRM_DLL void osrm_query_add_coordinate(osrm_query_t*, double lat, double lon);

/* osrm_json_handler_t

A simple minimalist json streaming interface
*/
extern OSRM_DLL osrm_json_handler_t* osrm_json_handler_create(
    void* state,
    void (*push_object)(void*),
    void (*push_array)(void*),
    void (*pop)(void*),
    void (*append_string)(void*, const char*, size_t),
    void (*append_number)(void*, double),
    void (*append_bool)(void*, unsigned char),
    void (*append_null)(void*));
extern OSRM_DLL void osrm_json_handler_destroy(osrm_json_handler_t*);

/* osrm_json_renderer_t

A json renderer
*/
extern OSRM_DLL osrm_json_renderer_t* osrm_json_renderer_create();
extern OSRM_DLL void osrm_json_renderer_destroy(osrm_json_renderer_t*);
/* return value is size, the returned string is not zero-terminated */
extern OSRM_DLL size_t osrm_json_renderer_harvest(
    osrm_json_renderer_t*, char**);
extern OSRM_DLL osrm_json_handler_t* osrm_json_renderer_create_handler(
    osrm_json_renderer_t*);

/* osrm_free, to free memory chunks allocated by this library */
extern OSRM_DLL void osrm_free(void*);

#ifdef __cplusplus
}  /* end extern OSRM_DLL "C" */
#endif

#endif /* OSRM_C_H */