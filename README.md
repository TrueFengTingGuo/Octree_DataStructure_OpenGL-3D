# Octree Data Structure 3D (C++ , OpenGL)


# Project Description
  This game uses C++ and OpenGL language in VS 2019. In this project, all display is direactly handled by vertex shdaers, geometry shaders and fragment shaders without using any third party editor. It features sprite rendering ,basic game physcis and particle system. This game is intended for showing octree data structure. Game objects are stored inside the octree data structures to minimize the cost of upadting and rendering. There are other features such as construct a smooth tunnel only using a few game vertices, using quaternion on transformation.
  
## In-game screen shot
<img src="https://github.com/TrueFengTingGuo/Octree_DataStructure_OpenGL-3D/blob/main/Reference_Photos_For_README/In_Game.png" width="420" height="250" border="10" /> 

# Run It Locally
  * using VS 2019
  
  * Inside the CMakeLists.txt, find the 
    ```txt 
    set(LIBRARY_PATH ---------set your library path here---------)
    ```
     add the path of Libraries folder into the statement.
  
  * Inside the path_config.h, change the
  
  ```c++
    # define MATERIAL_DIRECTORY "------set material dirctory here ------- "
  ```
  to the path of Octree_demonstration_game folder.
  
  * set main.cpp as Startup item
  
  * run it in x86
