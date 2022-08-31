# How can we have multiple shaders for drawing various objects in the scene ?

https://gamedev.stackexchange.com/questions/22216/using-multiple-shaders 
* Draw one element using one shader
* Then unbind that shader and bind another one and draw another element
* It's a non-trivial cost but you can do it tens or hundrends of time per frame

# Cube
* https://www.khronos.org/opengl/wiki/Primitive (GL_PATCHES)
* https://www.khronos.org/opengl/wiki/Tessellation#Patches
