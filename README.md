# OpenGL-Testing

This repository keeps track of any progress towards [this project](https://medium.com/@mriadzaky/we-can-render-reality-this-is-how-6478b7ea4ce2), aiming to make more realistic realtime renders, which is mostly just learning about the internals of OpenGL for now. Keeping with that, it's currently just a modification of [this learning site's repo](https://learnopengl.com/), which is located [here](https://github.com/JoeyDeVries/LearnOpenGL). Although, eventually it will be completely different, which is why a fork was not made.

The only subsection that was modified are the files associated with example 6.1.2, the "PBR Lighting Textured" example. The project eventually needs to be able to guess a large range of PBR values against a photograph(s) (the values of which are in UV space and Screenspace respectively), and keep track of and save the most "accurate" rendering values (closest to photograph(s)) to new, better textures. So far, the following have been demonstrated in those files:

- Use of a texture in Screenspace
- Use of Uniform variables
- Use of [Texture2DArray](https://www.khronos.org/opengl/wiki/Array_Texture), which while still technically uniform, allows for per-fragment variables
- Logging of Frames-Per-Second
- Bit-packing and unpacking in [Texture2DArray](https://www.khronos.org/opengl/wiki/Array_Texture) under the uint type
- Conversion between uint and float, vector usage, etc.
- Usage of several built-in functions (texture, texelfetch, pow)
- An implementation of a 5-variable binary search in C++, which will eventually be put in the fragment shader and use all of the above, a special case of the [K-d Tree Search](https://en.wikipedia.org/wiki/K-d_tree#Nearest_neighbour_search)

This is the original example:
