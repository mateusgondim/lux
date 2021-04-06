![Cornell Box scene rendered with lux](https://github.com/mateusgondim/lux/blob/master/images/cornell_box_256.png?raw=true)

# The Lux Renderer #

lux is a global Illumination renderer that uses Monte Carlo path tracing to generate physically based images.


## Motivation ##
After i decided to really learn about Ray Tracing – i’m not talking about small tutorials on the internet, but about reading some rather large books
on the subject –, i figured that the best way to solidify that knowlege was to write a full-fledged renderer using what i learn. The result was lux,
a  global Illumination renderer based on the path tracing algorithm.  

## Features ##
 - Ray-triangle/sphere intersection
 - Specular and  diffuse BRDFs
 - Thin lens camera model
 - unbiased Monte Carlo path tracing
 - Supersampling with a stratified sampler
 - Tent and box filters
 - Soft shadows from diffuse luminaire

## TODO List ##
Even though lux is a "complete" renderer, there are a few key features that are still left to be implemented:
  - Acceleration data structure for ray intersection tests
  - Support for triangle meshes
  - Support for multithreading 
