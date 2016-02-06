import os

Import('osrm_version', 'osrm_c_version', 'install_prefix')

# osrm_c_version uses Semantic Versioning, but seems '-dev' is not friendly
# to sonames
soname = osrm_c_version.replace('-', '')

env = Environment(CXXFLAGS="-std=c++11",
                  tools=['default', 'textfile'],
                  SHLIBVERSION=soname,
                  )

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
env.Append(LIBS=['boost_filesystem', 'boost_thread'], CCFLAGS='-O3')

## XXX: should add SHLIBVERSION=osrm_c_version
osrm_c = env.SharedLibrary(target='osrm_c',
                           source=sources,
                           )
env.Default(osrm_c)
env.InstallVersionedLib(os.path.join(install_prefix, 'lib'), osrm_c)
env.Install(os.path.join(install_prefix, 'include'), 'osrm_c.h')
env.Alias('install', install_prefix)

SConscript('test/SConscript')
