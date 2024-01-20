#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include <cstdlib>

#include "OPS_instance_t.hpp"
#include "OPS_input_t.hpp"
#include "OPS_output_t.hpp"

#include "solvers.hpp"

using namespace std;
using namespace EMIR;

void read(const string &file_name, OPS_instance_t &instance)
{
    ifstream input_file(file_name);

    instance.read(input_file);
    input_file.close();
}

int processor(const string &ins_file,
              const string &sta_file,
              const string &log_file,
              const int id)
{
    OPS_instance_t I; 
    read(ins_file, I);

    OPS_input_t In(I);
    const double tol = 1E-4;

    OPS_output_t Out(In);

    ofstream O_file(sta_file, std::ios_base::app);
    ofstream L_file(log_file);

    (*solver_array[id])(&In, tol, Out, L_file, O_file);

    O_file << endl;
    O_file.close();

    L_file << endl;
    L_file.close();

    return 0;
}

int main(int argc, char **argv)
{
    int exit_code = 0;

    /*if (argc == 7)
    {*/

        /*
         *  argv[1]       Target file
         *  argv[2]       Instance file
         *  argv[3]       Output log file
         *  argv[4]       Output sol file
         *  argv[5]       Output sta file
         *  argv[6]       Solver ID
         */

        /*const string sta_file(argv[1]);
        const string ins_file(argv[2]);
        const string log_file(argv[3]);
        
        const int id = atoi(argv[4]) - 1;*/

        const string sta_file("tmp.txt");
        const string ins_file("/home/riera/Dropbox/Trabajos_en_curso/OPS/OPS_CPX/test_ins.txt");
        const string log_file("test_log.txt");
        const int id = 0;

        exit_code = processor(ins_file,
                              sta_file,
                              log_file,
                              id);
    /*}*/

    return exit_code;
}
