# genhosts

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue)]()
<!-- [![GitHub Issues](https://img.shields.io/github/issues/randalthor17/genhosts.svg)](https://github.com/randalthor17/genhosts/issues) -->
<!-- [![GitHub Pull Requests](https://img.shields.io/github/issues-pr/randalthor17/genhosts.svg)](https://github.com/randalthor17/genhosts/pulls) -->

A simple program to generate hosts file from specified blocklist URLs.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

## Features

- Downloads specified blocklist URLs in `/etc/genhosts/config.toml`
- Concatenates each one, ordered by priority
- Removes duplicate entries
- Hosts download directory, output location 
- Dumps default config to `/etc/genhosts/config.toml` if specified
- Restores default `/etc/hosts` if specified
- Loads config from a custom file if specified

## Prerequisites

- A C++ compiler with C++20 support
- Meson build system
- libcurl library

## Installation

1. Clone the repository:

```sh
git clone https://github.com/randalthor17/genhosts.git
```
2. Navigate to the project directory:
```sh
cd genhosts
```
3. Generate meson build directory:
```sh
meson setup -Dbuildtype=release -Db_strip=true --prefix=/usr build
```
  **Note:** You can change the prefix to `~/.local` to do a local installation.

4. Build the project:
```sh
meson compile -C build
```
5. Install the project:
```sh
meson install -C build
```

## Usage

```sh  
genhosts [OPTION...]

  -c, --config arg              Configuration file location (default: 
                                /etc/genhosts/config.toml)
      --hosts-save-dir arg      Hosts file download directory (default: 
                                /etc/hosts.d)
  -o, --output arg              Hosts output path (default: /etc/hosts)
  -s, --dump-config [=arg(=/etc/genhosts/config.toml)]
                                Dump config to a specific path
      --restore [=arg(=/etc/hosts)]
                                Restore hosts file
  -h, --help                    Print help

```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
