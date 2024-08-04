# Domain Level Model

This model is designed to simulate the folding pathway of DNA origami as described in my thesis: https://ora.ox.ac.uk/objects/uuid:23ca884b-f547-4458-9e43-494fd6c2f6a3

# Installation

## CMake

The recommended method is to use CMake. From the root directory, do:
```
mkdir build
cd build
cmake ..
```

This create a `Makefile`. Then run make:
```
make
```

This creates an excecutable `/build/exe/DLM`.

You can use a different compiler by recpling the last line with:

```cmake -DCMAKE_CXX_COMPILER=/path/to/compiler ..```

## Makefile

There is a preprepared `Makefile` if the above doesn't work. Create an empty `bin` directory, adjust the paths in the `Makefile` and then:

```
mkdir bin
make
```

This creates an executable `/bin/DLM`.

## Dependencies

The `boost::graph` and `boost::random` libraries are used heavily. The plan is remove these dependencies in the future. The best option is installing `boost` using homebrew or a similar package manager so that CMake can find the library.


## Compatibility

I have tested the package on using g++-8, g++-10 and Apple CLang.
I have tested the following versions of Boost:
boost@1.61, boost@1.66 and boost@1.74 (recommended with cmake).

# Running

The model is a MC package and batch runs are needed for most results. Here. The `Example` directory contains an example with a sample input file. The subdirectory `Input` contains files that can be used to run the model with different origami topologies.
- `JSONs` contains CadNano-readable `json` format.
- `Topologies` contains the converted versions used by the model.
- `OPs` contains their associated order parameter files. These are used to track folding and to apply biases in umbrella sampling simulations.
- `Scaffolds` contains a few scaffold sequences used in my thesis.

## Input File Options
Options are set with the `key` = `value` syntax. Comments are preceded by `#`.
- `rate_model = global|local`: whether the thermodynamically consistent (global) model or the shortest path (local) is used.
- `rate_method = custom|mixed|metropolis|kawasaki`: use custom for now. Others are not implemented in this version.
- `sim_type = isothermal|anneal|melt|exact`: the type of temperature ramp to be applied. See options below.
- `domain_energies = average, seq, nucleate, custom`:
  - `average`: use {ATCG}-averaged base-pair hybridisation energies.
  - `seq`: use sequence dependent hybridisation energies. Must specify a `scaffold_file`.
  - `nucleate`: used for some nucleation experiments. Redundant.
  - `custom`: use custom energy per domain. Must specify path to an `energy_file`.
- `energy_file = <str>`: path to energy file used if `domain_energies = custom`. Each line must specify a domain, dH, dS, separated by tabs.
- `k_bi = <float>`: bimolecular binding rate constant [s-1 mol-1 m3].
- `k_uni = <float>`: unimolecular rate constant [s-1]. Should be `1000*k_bi` to correct for `1M` concentration.
- `k_f = <float>`: rate constant used for strand displacement reactions. This should be equal to `k_bi`, except when testing the model.
- `gamma = 1.5|2.5|3.5`: controls the loop cost.
- `n = <int>`: controls the stacking strength in the model. Can be a multiple of average hybridisation energy.
- `seed <int>`: random number generator seed.
- `custom_conc = <bool>`: whether to use different concentrations for each domain.
- `conc_file = <str?`: each line must have a staple and a concentration (separated by tabs).
- `concentration = <float>`: if `custom_conc=false`, this specifies the concentration of all staples.
- `miss_staples = <bool>`: whether some staples are missing from the system. Equivalent to setting concentration to zero.
- `missing_staple_file = <str>`: path to the missing staple data. Each line specifies the staple pool id and the staple id in that pool. The staple pool id is needed when two sets of staples are competing.
- `rotated_pool = -1|0|1`: which staple pool to rotate with respect to the scaffold. `-1` specifies no rotation. This is useful for competitive simulations.
- `rotate_by = <int>`: how many bases to rate the specied pool w.r.t. the scaffold.
- `scaff_rot = <int>`: how many bases to rotate the scaffold. This is useful for sequence-dependent simulations.
- `track_clusters = <bool>`: deprecated.
- `make_movie <bool>`: deprecated. It's easier to create a complete trajectory file and use Python for movies.
- `initialise_as = empty|coated|conf_file|random`:
  - `empty`: no domain is bound to the scaffold initially. Use this option in annealing simulations.
  - `coated`: all domains in the staple pool specified by `coat_with` are bound to the scaffold.
  - `conf_file`: initialise using a `configuration_file`.
  - `random`: useful for umbrella sampling simulations.
- `configuration_file = <str>`: path to configuration file if `initialise_as=conf_file`.
- `coat_with = 0|1`: the staple pool that is initially bound to the scaffold if `initialise_as=coated`.
- `scaffold_file = <str>`: path to the scaffold sequence file.
- `topology_file = <str>`: path to the topology file.
- `target_pool = 0|1`: this is only used when `sim_type=isothermal`. It specifies which pool must be bound by the `target_bound_fraction` for the simulation to stop.
- `target_bound_fraction`: this is only used when `sim_type=isothermal`. It specifies the fraction to which the `target_pool` must be bound to stop the simulation.
- `temp = <float>`: this is only used when `sim_type=isothermal`. Specifies the temperature in [$\degree C$].
- `max_time = <float>`: this is only used when `sim_type=isothermal`. Upper limit on time.
- `max_steps = <int>`: this is only used when `sim_type=isothermal`. Upper limit on number of MC steps.
- `burn_steps = <int>`: this is only used when `sim_type=isothermal`. MC steps before statistics are gathered.
- `extra_ops = <bool>`: whether to use extra order parameters. Number of domains, crossovers and staples are hardcoded but groups of different order parameters can be specified by an `op_file`.
- `op_file = <int>`: path to the order parameter file; only used when `extra_ops=true`.
- `umbrella_sampling = <bool>`: whether to do umbrella sampling.
- `weight_file = <int>`: path to the weights for umbrella sampling; only used when `umbrella_sampling=true`.
- `max_temp = <float>`: maximum temperature if `sim_type=anneal|melt`.
- `min_temp = <float>`: minimum temperature if `sim_type=anneal|melt`.
- `cool_rate = <float>`: heating/cooling rate in [K/minute] if `sim_type=anneal|melt`.
- `const_T_interval = <float>`: the temperature window in [s] for which the temperature is kept constant as we cool/heat. Note that the change in temperature in each window is dT [K] = const_T_interval [s] / coolrate [s/K].
- `n_cycles = <int>`: not implemented.

- There are several other inputs that specify the output of the model. See sample input file in `Example` directory.

# Analysis

There is an accompanying repository (under development) that contains analysis tools written in Python: https://github.com/behnamn90/DLMAnalysis.
