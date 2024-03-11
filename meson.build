project('genhosts', 'cpp', default_options : ['cpp_std=c++20'] )
src = ['src/url_utils.cpp', 'src/toml_utils.cpp', 'src/user_utils.cpp', 'src/file_utils.cpp',
      'src/string_utils.cpp', 'src/argparse.cpp', 'src/restore_hosts.cpp', 'src/main.cpp']
curl_dep = dependency('libcurl')
cxxopts_dep = dependency('cxxopts', required: false)
if not cxxopts_dep.found()
  cxxopts_proj = subproject('cxxopts')
  cxxopts_dep = cxxopts_proj.get_variable('cxxopts_dep')
endif
executable('genhosts', src, dependencies: [curl_dep, cxxopts_dep], install: true)
