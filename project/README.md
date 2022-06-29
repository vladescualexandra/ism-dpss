Compile and run PoC.c (Proof of Concept):
```
mpicc -fopenmp -o poc PoC.c
mpirun -n 4 --allow-run-as-root ./poc
```
----------------------------------------------
Compile and run Sequential.c:
```
gcc -o seq Sequential
./seq
```

Compile and run OMP.c:
```
gcc -fopenmp -o omp OMP.c
./omp
```

Compile and run MPI.c:
```
mpicc -o mpi MPI.c 
mpirun -n 4 --allow-run-as-root ./mpi
```

Compile and run MPI_with_OMP.c:
```
mpicc -fopenmp -o mpi_omp MPI_with_OMP.c 
mpirun -n 4 --allow-run-as-root ./mpi_omp
```
