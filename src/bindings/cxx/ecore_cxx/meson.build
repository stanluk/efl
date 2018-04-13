public_headers = [
'Ecore.hh',
'Ecore_Manual.hh'
]

public_sub_headers= [
]

ecore_cxx = declare_dependency(
    include_directories: include_directories('.'),
    dependencies: [eina, eina_cxx, eo, eo_cxx],
)

install_headers(public_headers,
    install_dir: dir_package_include
)

install_headers(public_sub_headers,
    install_dir: dir_package_include,
    subdir: 'ecore-cxx'
)


