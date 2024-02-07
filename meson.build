project('genhosts', 'cpp')
src = ['src/fetch_url.cpp', 'src/parse_toml.cpp', 'src/user_utils.cpp', 'src/main.cpp']
curl_dep = dependency('libcurl')
executable('genhosts', src, dependencies: curl_dep)
