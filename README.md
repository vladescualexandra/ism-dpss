
### Settings:
1. Open Solution Explorer
2. Right-Click on "omp-directives" (2nd level)
3. Configuration Properties -> C/C++ -> All Options
4. Search for "Open MP Support"
5. Click on the arrow and select "Yes (/openmp)"
6. Apply
After that, those two functions from #pragma omp parallel will be called 8 times each (for all the processors).
noCores will be the default number of threads.
