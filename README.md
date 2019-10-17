# moonrabbit file manager
Very low resource terminal based file manager written in C.  
Posix compliant.
Keybindings are vim for the most part.
My goal is to make something like Ranger, but in C and with as little resource usage as possible (and not in Python :D)

## Dependencies  
    ncurses  
    
## Compile (Use -lncursesw rather than -lncurses for wide char/unicode compatibility
    gcc -lncursesw *.c -o moonrabbit  
   
