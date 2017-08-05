subdir('bison')

flex = find_program('flex')
bison = find_program('bison')

flex_args = ['-o', '@OUTPUT@', '@INPUT@']
bison_args = ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '-S', '@0@'.format(skeleton_file)]
# Ideal way
#bison_args = ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@', '-S', skeleton_file]

if line_enabled
    flex_args = ['--noline'] + flex_args
    bison_args = ['--no-lines'] + bison_args
endif

flex_gen = generator(flex,
                     output: '@BASENAME@.cpp',
                     arguments: flex_args)

lexer_files = flex_gen.process(['Scanner.l'])

bison_gen = generator(bison,
                      output: ['@BASENAME@.cpp', '@BASENAME@.h'],
                      arguments: bison_args)

parser_files = bison_gen.process(['Parser.y'])

parser_lib = library('parser', 
                     'Driver.cpp', 'CodeGenerator.cpp',
                     lexer_files, parser_files, 
                     dependencies: common_dep,
                     include_directories: incdir)


parser_dep = declare_dependency(
    link_with: parser_lib,
    dependencies: common_dep,
    sources: parser_files)
