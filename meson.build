project('nt_string_view', 'cpp', default_options : ['warning_level=3'], meson_version : '>=0.55.0')

# インクルードディレクトリ
include_dir = include_directories('include')

# コンパイルオプション調整
cppcompiler = meson.get_compiler('cpp').get_argument_syntax()

if cppcompiler == 'msvc'
    options = ['/std:c++20', '/source-charset:utf-8', '/Zc:__cplusplus']
elif cppcompiler == 'gcc'
    options = ['-std=c++2a']
endif

# VSプロジェクトに編集しうるファイルを追加する
vs_files = ['include/nt_string_view.hpp']


if not meson.is_subproject()

# 依存プロジェクト
boostut_dep = dependency('boost.ut')

exe = executable('nt_string_view_test', 'test/nt_string_view_test.cpp', include_directories : include_dir, extra_files : vs_files, cpp_args : options, dependencies : boostut_dep)
test('nt_string_view test', exe)

else

# subprojectとして構築時は依存オブジェクトの宣言だけしとく
nt_string_view_dep = declare_dependency(include_directories : include_directories('include'), cpp_args : options)

endif