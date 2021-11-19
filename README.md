# OpenGL-Testing

This repository keeps track of any progress towards [this project](https://medium.com/@mriadzaky/we-can-render-reality-this-is-how-6478b7ea4ce2), aiming to make more realistic realtime renders, which is mostly just learning about the internals of OpenGL for now. Keeping with that, it's currently just a modification of files from [this learning site's repo](https://learnopengl.com/), which is located [here](https://github.com/JoeyDeVries/LearnOpenGL). Although, eventually it will be completely different, which is why a fork was not made.

The only files that were modified are the files associated with example 6.1.2 (1.2.pbr.fs, lighting_textured.cpp), the "PBR Lighting Textured" example. The project eventually needs to be able to guess a large range of PBR values against a photograph(s) (the values of which are in UV space and Screenspace respectively), and keep track of and save the most "accurate" rendering values (closest to photograph(s)) to new, better textures. So far, the following have been demonstrated in these files:

- Use of a texture in Screenspace
- Use of Uniform variables
- Use of [Texture2DArray](https://www.khronos.org/opengl/wiki/Array_Texture), which while still technically uniform, allows for per-fragment variables
- Logging of Frames-Per-Second
- Bit-packing and unpacking in [Texture2DArray](https://www.khronos.org/opengl/wiki/Array_Texture) under the uint type
- Conversion between uint and float, vector usage, etc.
- Usage of several built-in functions (texture, texelfetch, pow)
- An implementation of a 5-variable binary search in C++ (5varBinSearch.cpp), which will eventually be put in the fragment shader and use all of the above, a special case of the [K-d Tree Search](https://en.wikipedia.org/wiki/K-d_tree#Nearest_neighbour_search)

This is the original example:
![alt text](https://raw.githubusercontent.com/MichaelRZ/OpenGL-Testing/main/Original.PNG "Original Example")

And this is the modified example, alternating every second:
![alt text](https://raw.githubusercontent.com/MichaelRZ/OpenGL-Testing/main/One.PNG "Modified Example First Frame")

![alt text](https://raw.githubusercontent.com/MichaelRZ/OpenGL-Testing/main/Two.PNG "Modified Example Second Frame")

The first frame was used for logging to make sure that uint / float / other variables were being passed correctly, by writing to the screen, as other forms of logging are difficult. The second frame demonstrates screenspace usage (texturing across screen coordinates), as opposed to the original UV (per-object texturing), which is extremely important for eventually comparing / writing / reading values in both coordinate spaces.

The C++ implementation of the searching algorithm is also worth mentioning, as it's a surprisingly difficult algorithm to make work efficiently, the detail of which I won't include here although is visible in the file itself (5varBinSearch.cpp). Here is an example output, finding the "correct" input (this could have been set to anything) of all values of 5 in 0 - 255:

![alt text](https://raw.githubusercontent.com/MichaelRZ/OpenGL-Testing/main/CPPOUT.PNG "CPP code Output")

Here, to describe what is happening, in a normal binary search the search space is split into two further spaces with each iteration. With 5 variables, it is split into 32 spaces each iteration. The [Physically Based Rendering](https://en.wikipedia.org/wiki/Physically_based_rendering) input space is 5 numbers with values of 0 - 255, so there are a little more than a trillion possible inputs. With this script, each iteration divides this into 32 sections, of which some may appear closer to the target than others according to the render output. The number of sections to consider at each depth can be designated as it is here to increase the number of renders tested, but still in a directed way, with [1, 1, 5, 5, 5, 5, 5, 5] designating that only the closest section should be considered at the first two depths, however the closest five sections should be considered at the other six depths. This results in 19,532 iterations as stated, or 32 times that, a little more than half a million renders / guesses. Here, it finds the target input of [5, 5, 5, 5, 5] immediately because the function is entirely linear, but this searching scheme should function under any circumstances (whether it works well remains to be seen). This is simulating what the algorithm would look like for a single pixel / fragment, but the goal is to make this function in the fragment shader on a per-fragment basis, which should absolutely be possible.

As for speed, that script runs almost instantaneously without printing the output progress and putting the -O2 optimization flag. Let's assume the rate of ~2000 frames per second is accurate for this machine (which is using an AMD HD7870, almost a decade old now), and that the speed penalty of running the search logic is negligible. Let's assume the same logic is used as in the script, i.e. 19,532 iterations * 32 renders = 625,024 renders. Let's also assume that I've misjudged somewhere and we're off by two orders of magnitude - we are running at 20 frames per second, for whatever reason. That still only results in about 31,251 seconds of processing, or about 8 and a half hours to generate textures apparently accurate to a photograph, in theory. Accurate to two photographs, which is likely what needs to happen, would be around 17 hours in theory, which is still entirely reasonable.

For storage usage, in theory, it should be around 200 mb of VRAM for a 2048 x 2048 texture on an object. This is already allocated in the script as a 2D Array Texture. It would have been more than 2GB originally, however, due to the bit-packing and unpacking and the depth-first-search nature of the algorithm, as well as how the rendering values can be generated, it should be able to be reduced significantly like that. That's what most of the commented-out parts of the fragment shader are for (in theory).
