public_headers = [
'Eolian_Cxx.hh',
]

# FIXME add remaining files once install is sorted out
public_sub_headers= [
join_paths('grammar', 'address_of.hpp'),
]

eolian_cxx = declare_dependency(
    include_directories: [include_directories('.'),
                          include_directories('./grammar')
                         ],
    dependencies: [eina, eolian, eina_cxx]
)

#eolian_cxx_include_dir = join_paths(dir_data, 'eolian-cxx', 'include')

install_headers(public_headers,
    install_dir: dir_package_include
)

install_headers(public_sub_headers,
    install_dir: dir_package_include,
    #subdir: 'grammar'
)
