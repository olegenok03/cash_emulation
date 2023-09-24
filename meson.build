project('CacheModel', 'cpp')

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')

src = files([
    'BTreeTools.cpp', 
    'FInOut.cpp', 
    'HashTableTools.cpp', 
    'Messages.cpp', 
    'Messages.cpp', 
    'Output.cpp', 
    'Person.cpp'])

executable('CacheModel', src, files(['CacheModel.cpp']))

src_for_testing = files([
    'BTreeTools.cpp', 
    'FInOut.cpp', 
    'HashTableTools.cpp', 
    'Messages.cpp', 
    'Messages.cpp', 
    'Output.cpp', 
    'Person.cpp',
    'Tests\TestingTools.cpp'])

subdir('Tests')
executable('Tests1', src_for_testing, tests1, dependencies:[gtest_dep])
executable('Tests3', src_for_testing, tests3, dependencies:[gtest_dep])