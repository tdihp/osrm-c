from subprocess import check_output

# get osrm version to build against
osrm_version = check_output(['pkg-config', 'libosrm', '--modversion']).strip()

# get version of this library
with open('VERSION') as f:
    osrm_c_version = f.read().strip()

AddOption('--prefix',
          default='/usr/local',
          dest='prefix',
          action='store',
          metavar='DIR',)

install_prefix = GetOption('prefix')

SConscript('SConscript',
           variant_dir='build',
		   exports=['osrm_version', 'osrm_c_version', 'install_prefix'])
