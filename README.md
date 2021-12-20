# minishell

A simple shell.

![ms](https://media.giphy.com/media/f4PcFV1xiVnNUp17jM/giphy.gif)

This program was developed by [Yuta-Fujimoto](https://github.com/yuta-fujimoto) and I [AuroraSloan](https://github.com/AuroraSloan)

## Features
#### Builtin commands
- ``echo`` with option `-n`
- ``cd`` with only a relitvie or absolute path
- ``pwd`` with no options
- ``export`` with no options
- ``env`` with no options or arguments
- ``exit`` with no options
#### Expansion
- ' inhibits all interpretions of a sequence of characters
- " inhibits all interpretation of a sequence of characters except for $
- Environment values expand to their values
- $? expands to the most recently executed foreground pipeline
#### Redirection
- ``<`` redirects input
- ``>`` redirects output
- ``<<`` reads input from the current source until a linie containing only the delimiter is seen
- ``>>`` redirects output with append mode
#### Piping
- The output of each command in the pipeline is connected via a pipe to the input of the next command
#### Signals work like in BASH
- ``Ctrl-C`` prints a new prompt on a new line
- ``Ctrl-D`` exits the shell
- ``Ctrl-\`` does nothing

----
## Installation
```bash
git clone https://github.com/AuroraSloan/minishell.git
```
----
### Usage
First, create the executable with make
```bash
make - compiles the minishell executable
make clean - removes all object files
make fclean - removes all object files and the executable
make re - removes all object files and the executable before recompiling the program
```
In order to run the program, simply run the executable.
```bash
./minishell
```
To exit the program, you can use the builtin command ``exit`` or press ``Ctrl-D``.
