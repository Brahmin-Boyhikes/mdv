# 3D Model Viewer
A tool to view models that are in OBJ format.

## Setup 
**Compile** by running

	$ make
	$ ./viewer path/of/model/file

## Controls
**Keyboard shortcuts**  
**x** - Exit  
**i** - Toggle between face and vertex normal  
**I** - Toggle inertia  
**f** - Toggle drawing of face normal  
**v** - Toggle drawing of vertex normal  
**d** - Darker lighting  
**b** - Brigher lighting  

## Features
**Camera inertia**  
Some users find it more natural if the camera keeps rotating around an object after they've released the mouse, as if they had spun a globe. The rotation slow down gradually over time and eventually stops spinning.

**Lighting control**  
Users are able to edit lights on the scene, which affect shaded mode rendering. (Brighter and Darker)

##Licence
MIT