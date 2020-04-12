## Moonrabbit file manager  

Moonrabbit is a low resource, terminal/ncurses file manager written in C. Ranger (Python) is neat, but I think C is more efficient and appropriate for this type of program. The config file(s) are also so ridiculous and large that it steams my beans to even think about it.  
My goal was to keep it efficient and tiny by using only the absolute minimum amount of memory required. I also wanted to avoid using any libraries aside from ncurses and syscalls/c libs, and to keep the config file/syntax simple.  
It should run on any Posix system.  
Keybindings are vim for the most part. They are detailed further below.  
The name Moonrabbit comes from the Touhou game series.  

<br>
<img "align="center" height="350px" width="auto" src="../assets/moonrabbit.gif?raw=true">  

### Dependencies  
You will need the ncurses development libraries from your distro  

    libncurses  
    libncursesw  
    
### Compile

	cd moonrabbit
    make

The Makefile uses "ncursesw6-config" to detect the correct cflags and libs for your version (it is a script that comes with the ncurses/ncurses development library).  
If you have any problem with that, check in your path because it might be a different version. Otherwise, the cflags and libs that my script output are commented out in the makefile, so just uncomment that instead.  


### Keybindings  
I plan to make the keybindings configurable in the config, but currently it is basically vim:  
    
    k = up  
    j = down  
    l = forward/right/open  
    h = previous directory  
    space = select/highlight file  
    gg = move cursor to top  
    G = move cursor to bottom  
    : = open mini shell (a bit wonky at the moment!)  
    / = search in current directory  
    yy = copy selected files to buffer  
    dd = cut selected files to buffer  
    pp = paste buffered files to current dir  
    xx = permanently delete selected files (you will get a y/N prompt)  
    ctrl+r = refresh display  
    ctrl+h = toggle hidden files  
    q = quit  
    

### Config File Howto  
In the config file there is an explanation for how to set file associations, with some examples already given.  
You need to give the FULL path to your program path (i.e. /usr/bin/mpv).  
Then within the brackets you give a space separated list of extensions (i.e. .mp3) that will be opened with that program.  
This all goes between the section brackets that starts with File Types and ends with End.  
I parse the config this way because I wanted a simple/easy way to have unambiguous file associations without an external mimetype library.  

Two notes:  
    - ".\<none>" is the extension of files like "foo" and "bar", i.e. with no extension.  
    - To have a program open IN terminal (like you might want for vim, or other text editors, prepend a '$' to the program path. This is shown for /usr/share/vim in the config. Otherwise the program is forked to the background and wait() does not hang within the current process.

If running with no command line arguments, moonrabbit looks for the config in /home/*you*/.config/moonrabbit/config.  
You can pass the argument  

    $ moonrabbit -c /path/to/config  
    
to read from a different location

### Notes (specifically for URxvt on Debian Stable)
I have tested moonrabbit on several distros (Arch, Gentoo, Debian, ...), FreeBSD, and also macOS.  
The only issue I have had has been on Debian Stable (Buster at this time) and only with URxvt terminal.  
If there is any issue with odd characters rendering, make sure you have:  

    URxvt.termName: rxvt-256color  
    
in your .Xresources (and of course have rxvt-unicode-256color installed!)


