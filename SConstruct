AddOption('--dbg', dest='dbg', action='store_true', default=False,
          help='Debug build')
env = Environment()
if GetOption('dbg'):
    env.Append(CCFLAGS='-Wall -Wextra -pedantic -g')
else:
    env.Append(CCFLAGS='-Wall -O3 -DNDEBUG')

FILES = Glob('*test.c') + [
    'set_int.c',
    #'set_str.c',
    'sx.c',
    'vec.c',
    'vec_int.c',
    'vec_str.c',
    ]
app = env.Program('cx_test', FILES)

Default(app)
appname = str(app[0])
runner = Command(target=f'run_{appname}', source=app,
                 action=f'-./{appname}')
alias = Alias('run', runner)
