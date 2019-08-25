import os

try:
    with open('hello.txt', 'r') as f:
        print('file opened')
        print(f.read())
        pass
    pass
except IOError:
    print('file not found')
    def process_me(f):
        print('process fname')
        f.write('processed\n')
        pass
    with open('hello.txt', 'w') as f:
        process_me(f)
        f.write('hello python\n')
        f.close()
