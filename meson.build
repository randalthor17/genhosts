project('genhosts', 'cpp')
src = ['src/url_utils.cpp', 'src/toml_utils.cpp', 'src/user_utils.cpp', 'src/file_utils.cpp', 'src/string_utils.cpp' , 'src/main.cpp']
curl_dep = dependency('libcurl')
executable('genhosts', src, dependencies: curl_dep)
