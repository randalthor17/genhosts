project('genhosts', 'cpp')
src = ['fetch_url.cpp', 'parse_toml.cpp', 'user_utils.cpp', 'main.cpp']
curl_dep = dependency('libcurl')
executable('genhosts', src, dependencies: curl_dep)
