common_lib = library('common', 
                     'Visitor.cpp', 'Node.cpp', 'Scope.cpp', 
                     'Error.cpp', 'Location.cpp', 'Type.cpp', 
                     'BinaryOp.cpp', 'UnaryOp.cpp', 'Pattern.cpp', 
                     'Expression.cpp')
common_dep = declare_dependency(
                link_with: common_lib,
                include_directories: incdir)
