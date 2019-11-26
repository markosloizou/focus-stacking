# Focus Stacker

Focus stacking is a technique used in photography and microscopy to combine images with shallow depth of field, each with 
a different part of the image in focus into a single image with enhaned depth of field. 

There are two ways to create images with different areas in focus. One is by adjuting the focus of the lens, shifting the focal 
plane, and the other is moving the camera so that the lens is at different distances from the object. 

So far a number of sharpness measures have been implemented. Each assigning a sharpness value at each pixel. 

## Current Work
* Refactor the GUI to an MVC pattern and investigate wheter or not to move from wWidgets to QT 

## Future Work
* Implement a DMAP focus stack
* Implement a pyramid focus stack 
* Implement an average focus stack based on sharpness measure
* Implement one of the many wavelet methods found in the literature
