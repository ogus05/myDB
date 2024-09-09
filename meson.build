project('myDB', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

libnvme = dependency('libnvme', required: true)

executable('testCode',
           'myDB.cpp',
           'test.cpp',
           'mnvme_kvs.cpp',
           install : true,
           dependencies: libnvme,
           )

