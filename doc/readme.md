# FreePaintingTools v3.0

## Functions：
1. Draw different shapes include **line**, **rectangle** and **ellipse**.
2. Draw shapes in different colors include  **red**, **green** and **blue**.
3. Draw shapes with different size of pen include **thin**, **medium** and **thick**..

## Compile and Run
- You can
1. Click the project 

> DawingProject.vcxproj

2. Run it with debug version in Win32 platform

> Debug + x86

- Or
1. Click the .exe file in folder

> freepainting.exe

## Operation
- Move mouse and push left button to draw. if you finish, release it.
- Click right button to erase the board.
- Have a good time!

## Version Update Record
 ### Version 1
 ####  1.1   
 - Fix the problem of ***coordinate deviation***;
 - Fix the problem which will cause painting erased when zoom in or out the program window.
 #### 1.2
 - Change the way of showing text box;
 - Only draw ***contour*** instead of fill the rectangle or ellipse.

### version 2
#### 2.0
- Use ***multi-thread*** way to read and parse xml files, you can change the file content while run this program!
- Add ***message-boxs*** to popup error;
- Add several kind of error code.

#### 2.1
- Add ***Drawing board*** class cater to C++;
- Add multi-thread synchronization ways of atomic.
#### 2.2
- Introduce database and cache tech;
- Use ***Cache*** class to simulate database&cache.

#### 3.0
- Add thread pool mechanism, which will allocate the task more covienient;
- Modify the class encapsulation.
