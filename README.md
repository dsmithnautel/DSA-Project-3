To run the program:

Make sure you have the following:

A C++17 (or newer) compatible compiler (e.g., g++, clang++). A terminal or command line interface. 
All header (.h) and source (.cpp) files in the same project directory. The following dataset files:
1. u.data – contains user ratings
2. u.item – contains movie information
   - These files should be placed inside a folder called data/ at the root of the project
  
Make sure ImGui is properly installed:
1. Go to the ImGui github repo: https://github.com/ocornut/imgui
2. Follow their install instructions
3. Once properly placed within the file according to the ImGui instructions the program should work and output ImGui (does not work on Mac and worked for us first try so hopefully these instructions work)

(If using Mac and ImGui does not work out program returns DOT files with the graphs as well)

Once running, the program will:

1. Load movie ratings and data
2. Prompt the user to add their own movies with genres and ratings
   - using simulated user input that randomly changes input and rating each time so the recommendation outputs are not the same everytime
3. Once finished the user should enter 'done' to break out of the user input section
3. Generate and display recommendations using both a graph-based and hash table-based recommender
4. Export a DOT file visualization
5. Viewable using ImGui is properly setup as described in the beginning
