# https://stackoverflow.com/questions/29459664/undefined-references-compiling-opengl-glfw-glew-on-ubuntug
# https://stackoverflow.com/questions/4158900/embedding-resources-in-executable-using-gcc
# use xxd -i, this is likely best
gcc -std=c99 *.c -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl -o main.out
