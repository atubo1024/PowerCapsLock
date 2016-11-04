# vim: syntax=python

AddOption(
    '--debug_build',
    action='store_true',
    help='debug build',
    default=False)

env = Environment(
    tools = ['mingw'],
    CC = 'gcc',
    LIBS = [
        "kernel32", "user32", "gdi32", "comdlg32", "winspool", "winmm", 
            "shell32", "shlwapi", "comctl32", "ole32", "oleaut32", "uuid", 
            "rpcrt4", "advapi32", "version", "wsock32", "wininet"
    ])

TARGET_NAME = 'power_capslock'
if GetOption('debug_build'):
    TARGET_NAME += '_debug'
    env.Append(CCFLAGS = ' -DDEBUG')
else:
    env.Append(LINKFLAGS = ' -mwindows')

env.Program(TARGET_NAME, ['main.c'])
