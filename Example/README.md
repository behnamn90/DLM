# Examples

There are several input files provided here, each running a different type of simulation.

You can run each input file by navigating here in terminal and then:
```
<path_to_DLM_executable> <input_file_name>
```

This will create an `Output` directory that contain trajectories, overall statistics, object (staples, domains, crossovers, etc) statistcs, transition data etc.

Each subsequent run will create a new folder `Output<n>` with increasing `n`.

## Annealing simulation
```
../build/DLM input_anneal.txt
```
This runs an annealing simulation from 80C to 40C for a single RcUa staple set binding to a scaffold. You can view the topology by loading `Input/JSONs/RcUa.json` in CadNano.

## Melting simulation
```
../build/DLM input_melt.txt
```
This runs a melting simulation from 40C to 80C for a the same RcUa staple set binding.
