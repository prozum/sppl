subdir('bison')

flex = find_program('flex')
bison = find_program('bison')

flex_args = ['-o', '@OUTPUT@', '@INPUT@']
bison_args = ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '-S', '@0@'.format(skeleton_file)]
# Ideal way
#bison_args = ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '-S', skeleton_file]

if line_enabled
    flex_args = ['--noline'] + flex_args
    bison_args = ['--no-lines'] +  bison_args  
endif

flex_cmd = [flex] + flex_args
bison_cmd = [bison] + bison_args

scanner_files = custom_target('flex',
                              command: flex_cmd,
                              input: 'Scanner.l',
                              output: 'Scanner.cpp')

parser_files = custom_target('bison',
                             command: bison_cmd,
                             input: 'Parser.y',
                             output: ['Parser.cpp', 'Parser.h'])

parser_lib = library('parser', 
                     'Driver.cpp', 'CodeGenerator.cpp',
                     scanner_files, parser_files, 
                     link_with: common_lib,
                     include_directories: incdir)
