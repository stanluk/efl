eolian_cxx_src = [
    'eolian_cxx.cc'
]

eolian_cxx = executable('eolian_cxx',
    eolian_cxx_src,
    dependencies: [eina, eo, eolian, efl, eina_cxx, eo_cxx, eolian_cxx, ecore_cxx],
    install: true,
    c_args: package_c_args,
    cpp_args: '-std=c++11',
)
