public_headers = [
'Eo.hh'
]

public_sub_headers= [
'efl_object_impl.hh',
'eo_concrete.hh',
'eo_future.hh',
'eo_cxx_interop.hh',
'eo_init.hh',
'eo_ops.hh',
'eo_promise.hh',
'eo_wref.hh',
'eo_event.hh',
'eo_private.hh',
'eo_promise_meta.hh',
]

eo_cxx = declare_dependency(
    include_directories: include_directories('.'),
    dependencies: [eina, eina_cxx, eo],
)

install_headers(public_headers,
    install_dir: dir_package_include
)

install_headers(public_sub_headers,
    install_dir: dir_package_include,
    subdir: 'eo-cxx'
)

