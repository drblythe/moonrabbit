## Moonrabbit file manager  

Moonrabbit is a low resource, terminal/ncurses file manager written in C. I liked Ranger, but hated that it was written in Python and how complicated the config files were, so that is why this exists! :^)  
My goal was to make it as efficient and simple as possible, and not use any outside libraries aside from ncurses.  
It should run on any Posix system.  
Keybindings are vim for the most part. They are detailed further below.  
The name Moonrabbit comes from the Touhou game series.  

### Dependencies  
    libncurses  
    
### Compile
    gcc $(ncursesw6-config --cflags --libs) *.c -o moonrabbit  
    
The ncursesw6-config... is a helper script that outputs the correct cflags and libs to compile with.  
It might also be ncursesw5-config, depending on the version you have installed
For me it just boiled down to  
    
    -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -lncursesw -ltinfo -ldl  
    
ncursesw is required over ncurses in order to render unicode wide chars correctly.  

### Keybindings  
I plan to make the keybindings configurable in the config, but currently it is basically vim:  
    
    k = up  
    j = down  
    l = forward/right/open  
    h = backward  
    space = select/hightlight file  
    gg = move to top  
    G = move to bottom  
    : = open mini shell (a bit wonky at the moment!)  
    / = search in current directory  
    yy = copy selected files to buffer  
    dd = cut selected files to buffer  
    pp = paste buffered files to current dir  
    xx = permanently delete selected files (you will get a y/N prompt)  
    r = refresh display  
    H = toggle hidden files  
    ctrl+h = toggle hidden files (I have noticed this has not worked across all distros!)  
    

### Config File Howto  
In the config file there is an explanation for how to set file associations, with some examples already given.  
You need to give the FULL path to your program path (i.e. /usr/bin/mpv).  
Then within the brackets you give a space separated list of extensions (i.e. .mp3) that will be opened with that program.  
This all goes between the section brackets that starts with File Types and ends with End.  
I parse the config this way because I wanted a simple/easy way to have unambiguous file associtions without an external mimetype library.  

Two notes:  
    - .\<none> is the extension of files like "foo" and "bar", i.e. with no extension.  
    - To have a program open IN terminal (like you might want for vim, or other text editors, prepend a '$' to the program path. This is shown for /usr/share/vim in the config. Otherwise the program is forked to the background and not waited on within the current process.

If running with no command line arguments, moonrabbit looks for the config in /home/*you*/.config/moonrabbit/config.  
You can pass the argument  

    $ moonrabbit -c /path/to/config  
    
to read from a different location

### Notes (specifically for URxvt on Debian Stable)
I have tested moonrabbit on several distros (Arch, Gentoo, Debian, ...) and also macOS.  
The only issue I have had has been on Debian Stable (Buster at this time) and only with URxvt terminal.  
If there is any issue with odd characters rendering, make sure you have:  

    URxvt.termName: rxvt-256color  
    
in your .Xresources (and of course have rxvt-unicode-256color installed!)

