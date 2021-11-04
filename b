*we need to install readline or pay attention when we write Makefile 
if we can't call readline()

*Lexar
split string by delimiters ">< \t|;" and each node of linked list have split's component
delimiters are ignored if they are in "" or '' unless " or ' is unclosed.

*Parser
https://www.sigbus.info/compilerbook#%E6%96%87%E6%B3%95%E3%81%AE%E8%A8%98%E8%BF%B0%E6%96%B9%E6%B3%95%E3%81%A8%E5%86%8D%E5%B8%B0%E4%B8%8B%E9%99%8D%E6%A7%8B%E6%96%87%E8%A7%A3%E6%9E%90
was very easy to understand and implementation is based on this description

I implemented parser by adapting these rules and made AST(abstract syntax tree)

mandatory shell grammer
<commandline> ::= <list>

<list> ::= <pipeline>
		|  <list> ";" <pipeline>

<pipeline> ::=  <command>
		|  <pipeline> "|" <command>

I ignored other rules in https://www.notion.so/UNIX-6f45f62b74d54f159a986b50369c2693

After making AST. syntax error check starts. If there are some error,
error message is printed on stdout.

And I made results of parser and lexar output in result.log

*syntax error
PIPE
| [pipeline] or [pipeline] | is error
(PIPE needs right and left children)

SCOLON
; [list] is error
(SCOLON needs left child)

STR
if redirections' next node is not STR, 
it is not error

*Definition of characters in header file
# define STR 0	/* string */
# define RDIR 1     // >
# define RRDIR 2    // >>
# define LDIR 3     // <
# define LLDIR 4    // <<
//# define SPACE 6
# define SQUOTE 7   // '
# define DQUOTE 8   // "
# define PIPE 9     // |
# define DPIPE 10   // ||
# define AND 11     // &
# define DAND 12    // &&
# define SCOLON 13  // ;
or see lexar function in lexar.c

parser.c line 15
we may be betteryooooooooooo
