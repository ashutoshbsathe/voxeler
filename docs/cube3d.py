import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits import mplot3d

fig = plt.figure()

# 3D cube
ax = fig.add_subplot(121,projection='3d')

data = np.ones([1,1,1])

colors = np.empty([1,1,1,4])

colors[:] = [0,1,0,1]
ax.text(0, 0, 0, "c",fontweight="bold", color='red')
ax.voxels(data,facecolors=colors,edgecolors='k')
#ax.axis(False)
plt.show()
