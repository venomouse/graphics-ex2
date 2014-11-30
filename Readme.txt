322128083 smdyshel

301117800 moshemandel


Implementation description 
--------------------------
We used the code we received for exercise 0 as base, and updated it according to the requirements of the exercise.

Do display the openMesh object, we loop over the vertices on each face, as was explained in class. 

We compute the transformation of the original shape using the principles explained in class:
Transformation = Projection * View * (Translation*Rotation*Scale) * Model

As each of the matrices is changed independently using the mouse moves, we update them using a single function updateMatrices. The effect of the function depends on the mode (translationMode, rotationMode or zoomMode), and each mode is toggled by the appropriate click of the mouse.

For coloring similar to the school solution, we use random point as a reference and then color each point according to its direction relative to the reference point. To keep the coloring stable after the transformations, the color is calculated in the vertex shader prior to applying the transformations. 


Files necessary for compilation 
-------------------------------
We used the files provided in the base code without adding any files, the files necessary for compilation are:
ex2.h
ex2.cpp - takes care of the user interface
Model.h
Model.cpp - responsible for the drawing
ShaderIO.h
ShaderIO.cpp     - used for shading
SimpleShader.vert
SimpleShader.frag


