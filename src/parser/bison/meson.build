cdata = configuration_data()
cdata.set('BISON_DIR', meson.current_source_dir())

skeleton_file = configure_file(input: 'skeleton.m4.in',
                               output: 'skeleton.m4',
                               configuration: cdata,
                               install: false)
