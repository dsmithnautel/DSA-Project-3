To run the program:

Make sure you have the following:

A C++17 (or newer) compatible compiler (e.g., g++, clang++). A terminal or command line interface. 
All header (.h) and source (.cpp) files in the same project directory. The following dataset files:
1. u.data – contains user ratings
2. u.item – contains movie information
   - These files should be placed inside a folder called data/ at the root of the project

Once running, the program will:

1. Load movie ratings and data
2. Prompt the user to add their own movies with genres and ratings
   - The user input should follow the format of the examples shown when the program asks for input
3. Generate and display recommendations using both a graph-based and hash table-based recommender
4. Simulate re-ratings to show updated recommendations
5. Export a DOT file visualization (if applicable in your code)
