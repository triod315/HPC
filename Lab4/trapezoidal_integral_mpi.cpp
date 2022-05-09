#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

double function(double x)
{
  return log(x);
}

int main(int argc, char *argv[])
{
    double start = atof(argv[1]);
    double end = atof(argv[2]);

    long long count = atoi(argv[3]);

    int process_id, num_processes;

    double start_time, end_time;
    double total_integral;
    double integral = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    if (process_id == 0)
    {
        start_time = MPI_Wtime();
    }
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (count != 0)
    {
        double h = (end - start) / count;
        double i1 = process_id * (count / num_processes);
        double i2 = (process_id + 1) * (count / num_processes);
        integral = (function(start + i1 * h) + function(start + i2 * h)) / 2;
        for (long long i = i1 + 1; i < i2; i++)
        {
            integral +=  function(start + i * h);
        }
    }

    MPI_Reduce(&integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (process_id == 0)
    {
        end_time = MPI_Wtime();
        cout << "Integral: " << total_integral << endl;
        cout << "Time: " << end_time - start_time << endl;
    }

    MPI_Finalize();

    return 0;
}