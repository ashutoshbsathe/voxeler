import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

ax = plt.axes(projection='3d')

ys = np.arange(1,11,1)
xs = np.ones_like(ys)
colors = ['k', 'b']
for i in range(len(ys)):
    if i in [0,len(ys)-1]:
        ax.scatter(xs[i],ys[i],c='k', depthshade=False)
    else:
        ax.scatter(xs[i],ys[i],c='b', depthshade=False)
ax.set_xlim((0,11))
ax.set_ylim((0,11))
ax.set_zlim((0,11))
plt.show()
