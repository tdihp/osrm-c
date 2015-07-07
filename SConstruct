from subprocess import check_output

osrm_version = check_output(['pkg-config', 'libosrm', '--modversion']).strip()
with open('VERSION') as f:
    osrm_c_version = f.read().strip()

SConscript('SConscript',
           variant_dir='build',
		   exports=['osrm_version', 'osrm_c_version'])
