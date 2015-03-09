# 3D Model Viewer

## Logistics
---
**Name** Alan Ma  
**UTEID** am65355  
**UTCSID** ama

## Instructions 
---

**Compile** by running

	$ make clean
	$ make
	$ ./viewer path/of/model/file

## Controls
---

**Keyboard shortcuts**  
**X** - Exit  
**l** - Toggle between face and vertex normal  
**I** - Toggle inertia  
**D** - Darker lighting  
**B** - Brigher lighting  


## Features
---

**Camera inertia**  
Some users find it more natural if the camera keeps rotating around an object after they've released the mouse, as if they had spun a globe. The rotation slow down gradually over time and eventually stops spinning.

Lighting control: Users are able to edit lights on the scene, which affect shaded mode rendering. (Brighter and Darker)