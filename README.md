<div align="center">
    <h1>CClarify</h1>
    <p>Small framework for advanced logging</p>
    <p>
        <a href="https://github.com/HyperWinX/CClarify/graphs/contributors">
            <img src="https://img.shields.io/github/contributors/HyperWinX/CCLarify" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/CClarify/commits/master">
            <img src="https://img.shields.io/github/last-commit/HyperWinX/CCLarify" alt="last commit"/>
        </a>
        <a href="https://github.com/HyperWinX/CClarify/network/members">
            <img src="https://img.shields.io/github/forks/HyperWinX/CCLarify" alt="forks"/>
        </a>
        <a href="https://github.com/HyperWinX/CClarify/stargazers">
            <img src="https://img.shields.io/github/stars/HyperWinX/CCLarify" alt="contributors"/>
        </a>
        <a href="https://github.com/HyperWinX/CClarify/issues">
            <img src="https://img.shields.io/github/issues/HyperWinX/CCLarify" alt="contributors"/>
        </a>
    </p>
</div>
<br/>

# Contents
- [About project](#about-cclarify)
- [Installation](#installation)
- [Usage](#usage)
  - [Loglevels](#loglevels)
  - [Formatting rules](#formatting-rules)
  - [Global formatter](#global-formatter-api)


# About CClarify
CClarify is a small logging library, written in pure C, that doesn't use any heap memory.

## Installation
If you use **Conan**, add the the package:
```
hyper-cclarify/<version>
```
You can choose a version in **Releases** tab.

## Usage
### Loglevels
CClarify has the following loglevels:
- **CLAR_LOG_DEBUG**
- **CLAR_LOG_INFO**
- **CLAR_LOG_WARNING**
- **CLAR_LOG_ERROR**
- **CLAR_LOG_FATAL**

**Example**:
If you will set loglevel to **CLAR_LOG_WARNING**, all log calls with priority lower than this (i. e. **CLAR_LOG_INFO** and **CLAR_LOG_DEBUG**) won't print anything at all.

### Formatting rules
| %Y | Insert current year (for example, **2025**)                                             |
|----|-----------------------------------------------------------------------------------------|
| %M | Insert abbreviated month name (for example, **Sep**)                                    |
| %d | Insert abbreviated day of week name (for example, **Mon**)                              |
| %D | Insert day of month as a decimal (for example, **09**)                                  |
| %H | Insert hour as a decimal (for example, **13**)                                          |
| %m | Insert minute as a decimal (for example, **32**)                                        |
| %s | Insert second as a decifuncleuncal (for example, **47**)                                        |
| %l | Insert formatted string from log() call                                                 |
| %x | Inserts message, specific to current loglevel (applying colors, if writing to terminal) |
| %% | Inserts a single percent                                                                |
| %  | Inserts a single percent too - if the next character is not a valid format specifier    |

### Global formatter API
`void clar_set_global_rotation(const char* filename, uint16_t max_files, uint32_t max_file_size)` - Enables log rotation when you enabled global output to file. Max file size is set in bytes.

`void clar_set_global_format(const char* fmt)` - Sets global format string. Notice, that this is NOT a format string, that is used by libc's *printf functions! See [formatting rules](#formatting-rules) for more.

`void clar_set_global_loglevel(__clar_loglevel loglevel)` - Sets global loglevel.

`void clar_log(__clar_loglevel loglevel, const char* fmt, ...)` - Uses global formatter and ged loglevel to log.

`void formattter and **CLAR_LOG_DEBUG** loglevel to log.

`void clar_info(__clar_loglevel loglevel, 
