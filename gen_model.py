import random
fname = 'verylarge.draw'
text = ''
N = 100
for i in range(N):
    for j in range(N):
        for k in range(N):
            text += f'{i*5} {j*5} {k*5} {random.random()} {random.random()} {random.random()}\n'
with open(fname, 'w') as f:
    f.write('100\n')
    f.write(text)
