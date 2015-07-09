#include <osrm_c.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    char* err = NULL;
    int code = 0;
	printf("testing osrm_c version: %s\n", osrm_c_version());
	printf("testing osrm version: %s\n", osrm_version());
    osrm_config_t* config = osrm_config_create();
    osrm_config_set_path(config, "base", "data/berlin-latest.osrm");
    osrm_config_set_use_shared_memory(config, 0);
    osrm_t* engine = osrm_create(config, &err);
    // printf(err);
    assert(err == NULL);
    osrm_config_destroy(config);
    osrm_json_renderer_t* json_renderer = osrm_json_renderer_create();
    osrm_json_handler_t* json_handler = osrm_json_renderer_create_handler(json_renderer);
    osrm_query_t* query = osrm_query_create();
    osrm_query_set_service(query, "viaroute");
    osrm_query_add_coordinate(query, 52.519930, 13.438640);
    osrm_query_add_coordinate(query, 52.513191,13.415852);
	osrm_query_set_instruction_flag(query, 0);
	osrm_query_set_alternate_route_flag(query, 0);
	osrm_query_set_geometry_flag(query, 0);
    osrm_run_query(engine, query, &code, json_handler, &err);
    // printf(err);
    assert(err == NULL);
    assert(code == 200);
    char* data = NULL;
    size_t data_size = osrm_json_renderer_harvest(json_renderer, &data);
    printf("%.*s\n", (int)data_size, data);
    free(data);
    osrm_query_destroy(query);
    osrm_json_renderer_destroy(json_renderer);
    osrm_json_handler_destroy(json_handler);
    osrm_destroy(engine);
}
