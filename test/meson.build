tests = []
tests_fail = []

subdir('add')  
subdir('bin')
subdir('cas')
subdir('concat')
subdir('div')
subdir('dot')
subdir('file')
subdir('func')
subdir('keyword')
subdir('list')
subdir('mod')
subdir('mul')
subdir('prec')
subdir('scope')
subdir('sig')
subdir('sub')
subdir('tuple')
subdir('when')
subdir('wild')

foreach test : tests
    test('@0@'.format(test), 
         sppl_exe,
         args: meson.source_root() + '/@0@'.format(test))
endforeach

foreach test : tests_fail
    test('@0@'.format(test), 
         sppl_exe,
         args: meson.source_root() + '/@0@'.format(test),
         should_fail: true)
endforeach
