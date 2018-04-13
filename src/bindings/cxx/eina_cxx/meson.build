public_headers = [
'Eina.hh'
]

public_sub_headers= [
'eina_value.hh',
'eina_logical.hh',
'eina_type_traits.hh',
'eina_tuple_c.hh',
'eina_copy_traits.hh',
'eina_list.hh',
'eina_array.hh',
'eina_log.hh',
'eina_stringshare.hh',
'eina_eo_concrete_fwd.hh',
'eina_string_view.hh',
'eina_tuple.hh',
'eina_aligned_union.hh',
'eina_ref.hh',
'eina_pp.hh',
'eina_range_types.hh',
'eina_ptrlist.hh',
'eina_error.hh',
'eina_workarounds.hh',
'eina_integer_sequence.hh',
'eina_throw.hh',
'eina_ptrarray.hh',
'eina_thread.hh',
'eina_lists_auxiliary.hh',
'eina_accessor.hh',
'eina_deleter.hh',
'eina_future.hh',
'eina_clone_allocators.hh',
'eina_variant.hh',
'eina_strbuf.hh',
'eina_iterator.hh',
'eina_tuple_unwrap.hh',
'eina_inarray.hh',
'eina_optional.hh',
'eina_fold.hh',
'eina_inlist.hh',
'eina_function.hh'
]

eina_cxx = declare_dependency(
    include_directories: include_directories('.'),
    dependencies: [eina, eo],
)

install_headers(public_headers,
    install_dir: dir_package_include
)

install_headers(public_sub_headers,
    install_dir: dir_package_include,
    subdir: 'eina-cxx'
)
