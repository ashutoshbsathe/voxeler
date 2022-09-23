import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set(proj_type='persp')
xs = np.arange(1,11,1)
ys = np.ones_like(xs)
zs = np.ones_like(xs)
colors = ['r', 'g', 'k', 'y', 'b', 'c']
for j in range(5):
    for i in range(5):
        ax.plot(xs,ys+i*2, zs+j*2, c=colors[j])

#elevatoion=7, azimuth=-67, roll=0
ax.view_init(7,-67)
ax.set_xlim((0,11))
ax.set_ylim((0,11))
ax.set_zlim((0,11))
ax.grid(False)
plt.savefig('grid.pdf')
plt.show()
