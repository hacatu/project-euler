 pypy -m cProfile p118.py
44680
         239811170 function calls (193725410 primitive calls) in 75.780 seconds

   Ordered by: standard name

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        1    0.897    0.897   75.780   75.780 p118.py:1(<module>)
  6505864    2.630    0.000    6.124    0.000 p118.py:10(isPrime)
  4289419    1.439    0.000    1.579    0.000 p118.py:11(witness)
 46448640   16.890    0.000   16.890    0.000 p118.py:37(ttoi)
 23425642    1.523    0.000    1.834    0.000 p118.py:44(pairwise)
 10886400    5.228    0.000    9.276    0.000 p118.py:51(increasing)
 23425642    2.214    0.000    4.048    0.000 p118.py:52(<genexpr>)
 10886400   20.098    0.000   52.387    0.000 p118.py:54(primeSets)
  3138506    0.171    0.000    0.723    0.000 p118.py:6(__builtin_ctz)
57335040/11249280   17.702    0.000   18.063    0.000 p118.py:66(partitions)
 10886401    4.434    0.000   74.883    0.000 p118.py:72(<genexpr>)
 10886400    0.147    0.000    0.147    0.000 {iter}
  3138506    0.552    0.000    0.552    0.000 {math.log}
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
 10886400    0.164    0.000    0.164    0.000 {next}
  4289419    1.191    0.000    1.191    0.000 {pow}
 13382489    0.502    0.000    0.502    0.000 {range}
