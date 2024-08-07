AddOption('--dbg', dest='dbg', action='store_true', default=False,
          help='Debug build')
env = Environment()
if GetOption('dbg'):
    env.Append(CCFLAGS='-Wall -g')
else:
    env.Append(CCFLAGS='-Wall -O3 -DNDEBUG')

app = env.Program('cx_test', ['cx_test.c', 'va_test.c', 'vec.c',
                              'vec_str.c', 'vec_int.c'])

Default(app)
appname = str(app[0])
runner = Command(target=f'run_{appname}', source=app,
                 action=f'-./{appname}')
alias = Alias('run', runner)
