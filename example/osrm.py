"""it's a simple Python binding that only does viaroute queries."""

from ctypes.util import find_library
from cffi import FFI

cdef = '''\
extern void free(void*);

/* object declearations */
typedef struct osrm_t               osrm_t;
typedef struct osrm_config_t        osrm_config_t;
typedef struct osrm_query_t         osrm_query_t;
typedef struct osrm_json_handler_t  osrm_json_handler_t;
typedef struct osrm_json_renderer_t osrm_json_renderer_t;

/* versioning interface */
extern const char* osrm_version();  /*libOSRM version*/
extern const char* osrm_c_version();  /*version of this c wrapper*/

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

extern void osrm_free(void*);
'''

class OSRM(object):
    def __init__(self, dllpath, base=None):
        ffi = FFI()
        ffi.cdef(cdef)
        lib = ffi.dlopen(dllpath)
        config = ffi.gc(lib.osrm_config_create(), lib.osrm_config_destroy)
        if base:
            lib.osrm_config_set_path(config, 'base', base)
            lib.osrm_config_set_use_shared_memory(config, 0)
        else:
            lib.osrm_config_set_use_shared_memory(config, 1)
        errptr = ffi.new('char**', ffi.NULL)
        engine = ffi.gc(lib.osrm_create(config, errptr), lib.osrm_destroy)
        try:
            assert errptr[0] == ffi.NULL, ffi.string(errptr[0])
        finally:
            if errptr[0] != ffi.NULL:
                lib.osrm_free(errptr[0])
        self.engine = engine
        self.lib = lib
        self.ffi = ffi

    def query(self, locs):
        ffi = self.ffi
        lib = self.lib
        json_renderer = lib.osrm_json_renderer_create()
        json_handler = lib.osrm_json_renderer_create_handler(json_renderer)
        q = lib.osrm_query_create()
        codeptr = ffi.new('int*', 0)
        errptr = ffi.new('char**', ffi.NULL)
        dataptr = ffi.new('char**', ffi.NULL)
        try:
            lib.osrm_query_set_service(q, 'viaroute')
            lib.osrm_query_set_instruction_flag(q, 0);
            lib.osrm_query_set_alternate_route_flag(q, 0);
            lib.osrm_query_set_geometry_flag(q, 0);

            for lat, lon in locs:
                lib.osrm_query_add_coordinate(q, lat, lon);

            lib.osrm_run_query(self.engine, q, codeptr, json_handler, errptr)

            if errptr[0] != ffi.NULL:
                raise RuntimeError(ffi.string(errptr[0]))

            if codeptr[0] != 200:
                return None

            size = lib.osrm_json_renderer_harvest(json_renderer, dataptr)
            if not size:
                return None

            return str(ffi.buffer(dataptr[0], size))
        finally:
            lib.osrm_json_renderer_destroy(json_renderer)
            lib.osrm_json_handler_destroy(json_handler)
            lib.osrm_query_destroy(q)
            if errptr[0] != ffi.NULL:
                lib.osrm_free(errptr[0])
            if dataptr[0] != ffi.NULL:
                lib.osrm_free(dataptr[0])
