from os import replace


with open('tri_list_1.txt') as f1, open('tri_list_2.txt') as f2:
    l1 = f1.read()
    l2 = f2.read()
point_to_name = {
    '(5, 0, 5)': 'a',
    '(5, 0, 0)': 'b',
    '(0, 0, 0)': 'c',
    '(0, 0, 5)': 'd',
    '(5, 5, 5)': 'e',
    '(5, 5, 0)': 'f',
    '(0, 5, 0)': 'g',
    '(0, 5, 5)': 'h',
}

def replace_points(x):
    for k, v in point_to_name.items():
        x = x.replace(k, v)
    return x

l1 = sorted([replace_points(x.strip().replace('\n', '->')) for x in l1.split('\n\n')])
l2 = sorted([replace_points(x.strip().replace('\n', '->')) for x in l2.split('\n\n')])


print('Incorrect:')
print('\n'.join(l1))
print(64*'-')
print('Correct:')
print('\n'.join(l2))
print(64*'-')

print('Incorrect - Correct:')
print('\n'.join(set(l1) - set(l2)))
print(64*'-')
print('Correct - Incorrect:')
print('\n'.join(set(l2) - set(l1)))
print(64*'-')
