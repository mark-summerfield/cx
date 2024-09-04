AddOption('--dbg', dest='dbg', action='store_true', default=False,
          help='Debug build')
env = Environment()
if GetOption('dbg'):
    env.Append(CCFLAGS='-Wall -Wextra -pedantic -g')
else:
    env.Append(CCFLAGS='-Wall -O3 -DNDEBUG')

FILES = Glob('*test.c') + [
    'fx.c',
    'deq_int.c',
    'set_int.c',
    'set_str.c',
    'str.c',
    'vec.c',
    'vec_int.c',
    'vec_str.c',
    ]
app = env.Program('cx_test', FILES, LIBS=['m'])

Default(app)
appname = str(app[0])
runner = Command(target=f'run_{appname}', source=app,
                 action=f'-./{appname}')
alias = Alias('run', runner)
