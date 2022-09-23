# How can we have multiple shaders for drawing various objects in the scene ?

https://gamedev.stackexchange.com/questions/22216/using-multiple-shaders 
* Draw one element using one shader
* Then unbind that shader and bind another one and draw another element
* It's a non-trivial cost but you can do it tens or hundrends of time per frame

# Drawing grid 

Use [`glDrawArrays(Elements)Instanced`](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArraysInstanced.xhtml) 

Collect all points so that they can be used to draw (N+1) lines parallel to X-axis in the X-Y plane at say Z=0 

If N = 2, normal `glDrawArrays` will draw something like this : 

```
P0 -- P1
P2 -- P3
P4 -- P5
```

Now use instanced draw call to : 

1. For each instance, calculate the Z value the same way we calculated coords of intermediate points but with instanceID as the index and then set the Z value 
2. Once instanceID = N+1, interprete the input pair as (Y,Z) instead of (X,Y) and control X instead of Z. Similarly, when instanceID = 2(N+1), change the interpretation again

More ref: https://learnopengl.com/Advanced-OpenGL/Instancing

# Cube
* https://www.khronos.org/opengl/wiki/Primitive (GL_PATCHES)
* https://www.khronos.org/opengl/wiki/Tessellation#Patches

# Figuring out triangles to draw for the whole model 

* https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/

# Timing various approaches (optional)

* https://www.khronos.org/opengl/wiki/Query_Object#Timer_queries
