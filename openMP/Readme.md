To execute code in openMP


1. Run 'python HPC_helper.py'
   This will create an 'output.csv' file which consists of initial data

2. Run 'g++ -o a -fopenmp HPC_project.cpp' in Linux terminal wherever HPC_project.cpp is present
   This will compile the cpp file

3. './a' in linux terminal to run the compiled file
    This will perform smote in parallel and append to 'output.csv' and print the time taken along with total no. of samples


4. 'python HPC_post.py' to get the generated data plotted.
    You can also run this after 1st step to get the initial data plotted.

Note - PLs make sure to delete 'output.csv' before starting 1st step.

