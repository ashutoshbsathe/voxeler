import random
fname = 'flag.raw'
text = ''
for z in range(25,27):
    for y in range(50,91):
        for x in range(30,70):
            if y in range(50,64):
                text += f'{x*5} {y*5} {z*5} {19/255.} {136/255.} {8/255.} \n'
            elif y in range(64,77):
                text += f'{x*5} {y*5} {z*5} {1} {1} {1}\n'
            else:
                text += f'{x*5} {y*5} {z*5} {255/255.} {153/255.} {51/255.} \n'


with open(fname, 'w') as f:
    f.write('100\n')
    f.write(text)
