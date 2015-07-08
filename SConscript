Import('osrm_version', 'osrm_c_version')

env = Environment(CXXFLAGS="-std=c++11", tools=['default', 'textfile'])

env['SUBST_DICT'] = {'@OSRM_VERSION@': osrm_version,
                     '@OSRM_C_VERSION@': osrm_c_version}
env.Substfile('version.c.in')

sources = [
    'osrm_c.cpp',
    'osrm_c_json_renderer.cpp',
	'version.c',
]

# env.ParseConfig('pkg-config libosrm --cflags --libs --static')
env.ParseConfig('pkg-config libosrm --cflags --libs')
env.Append(LIBS=['boost_filesystem', 'boost_thread'])

## XXX: should add SHLIBVERSION=osrm_c_version
env.SharedLibrary(target='osrm_c', source=sources)


SConscript('test/SConscript')
