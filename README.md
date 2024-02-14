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
    - [Assign](#assign)
    - [Exec](#exec)
    - [Msg](#msg)
    - [Display](#display)

# About CClarify
CClarify is a small framework for creating advanced logging system. It can write to your custom descriptor, pointing to buffer, and can save everything to file, if you set it to do so.

## Installation
1. Download main header [cclarify.h]("cclarify.h")
2. Include it to your source file, where you want to add logging
3. Now you should set up everything.
```c
GLOBAL_INIT(); // Init logger variables
Clarifier clar; // Create logger object
// If you have int descriptor like stdout
init_loggerd(clar, descriptor);

// If you want it to write logs to file
init_loggerf(clar, fd);

// If you want it to write logs to both destinations
init_loggerfd(clar, descriptor, fd);
```
4. You are good to go now!
## Usage
### Assign
If you want to assign some value to variable (custom types are not supported) you should do this:
```c
ASSIGN(clar, variable, value);
```
It will output something like:
```
==> Assigning value "value" to "variable", old value: "old_var_val"
```
### Exec
If you wanna execute some function, and signal if function started or stopped, do this:
```c
EXEC(clar, function());
```
It will notify you about started execution:
```
==> Starting execution of function "function()"
```
And about end of function execution.
```
==> Execution of function "function()" finished
```

### Msg
To display some king of message, you need just to use MSG() function.
```c
// To display green info message, do this
MSG(clar, "Hello World!", INFO);
// Yellow - warning
MSG(clar, "Warning", WARNING);
// Red - error
MSG(clar, "Error", ERROR);
```

### Display
Sometimes you want just to display variable value. Do this:
```c
DISPLAY(clar, variable);
```
and you will get something like:
```
==> Variable "variable" value: "420"
```