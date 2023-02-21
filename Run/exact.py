import os
import numpy as np
import pandas as pd
import math
import matplotlib.pyplot as plt

gas_constant = 0.00198720358509

def get_weights_from_probs(DF,op):
    df = DF.groupby(op)[['P']].sum()
    df['w'] = df['P'].apply(lambda p: np.sqrt(1/p))
    df['w'] = df['w']/df['w'].min()
    return df

def get_squared_weights_from_probs(DF,op):
    df = DF.groupby(op)[['P']].sum()
    df['w'] = df['P'].apply(lambda p: 1/p)
    df['w'] = df['w']/df['w'].min()
    return df

def plot_test(x,y,name=''):
    fig,ax = plt.subplots(1,1)
    ax.plot(x,y)
    fig.tight_layout()
    fig.savefig(name)
    plt.close(fig)

def get_probs(filename,celcius):
    df = pd.read_csv(filename)
    T = 273.15 + celcius
    beta = gas_constant * T
    concentration = math.pow(10, -7)
    print(df.columns)
    df['G_total'] = df[['G_duplex','G_stack','G_shape']].sum(axis=1)
    df['PxZ'] = df['G_total'].apply(lambda x: np.exp(-x / beta))
    df['conc'] = df['numStapleCopies'].apply(lambda x: math.pow(concentration, x))
    df['PZC'] = df['PxZ'] * df['conc']
    Z = df['PZC'].sum()
    df['P'] = df['PZC'] / Z
    return df

def get_freeE(probs, op='numBoundDomains'):
    df = probs.set_index(op)[['P']]
    df = df.groupby(op)['P'].sum()
    logP0 = np.log(df[0])
    df = df.apply(lambda p: logP0 - np.log(p))
    return df

def get_temp(inputfilename):
    with open(inputfilename,'r') as myfile:
        lines = myfile.read().split('\n')
    for line in lines:
        if line.split('=')[0].strip(' ') == 'temp':
            celcius = float(line.split('=')[1].strip(' '))
    return celcius

def get_top(inputfilename):
    with open(inputfilename,'r') as myfile:
        lines = myfile.read().split('\n')
    for line in lines:
        if line.split('=')[0].strip(' ') == 'topology_file':
            val = line.split('=')[1].strip(' ')
            top = (val.split('/')[-1]).split('.')[0]
    return top

def write_weights(probs, outfile, top, temp):
    dfw0 = get_weights_from_probs(probs,'numBoundDomains')
    dfw0 = dfw0.reset_index()[['numBoundDomains','w']]
    dfw0 = dfw0.rename(columns={'numBoundDomains':'OP'})
    dfw0['Topology'] = top
    dfw0['Scaffold'] = 'average'
    dfw0['Temp'] = temp
    dfw0['Window'] = 'D_W1'
    dfw0['Weight'] = 'w0'
    dfw0 = dfw0[['Topology','Scaffold','Temp','Window','Weight','OP','w']]
    #dfw0.to_csv(outfile,index=False)
    dfw1 = get_squared_weights_from_probs(probs,'numBoundDomains')
    dfw1 = dfw1.reset_index()[['numBoundDomains','w']]
    dfw1 = dfw1.rename(columns={'numBoundDomains':'OP'})
    dfw1['Topology'] = top
    dfw1['Scaffold'] = 'average'
    dfw1['Temp'] = temp
    dfw1['Window'] = 'D_W1'
    dfw1['Weight'] = 'w1'
    dfw1 = dfw1[['Topology','Scaffold','Temp','Window','Weight','OP','w']]
    #dfw1.to_csv(outfile,index=False)
    dfw = pd.concat([dfw0,dfw1],axis=0)
    dfw.to_csv(outfile,index=False)

directory = 'Output'
exactfilename = directory+'/Exact.csv'
inputfilename = directory+'/input.txt'
top = get_top(inputfilename)
celcius = get_temp(inputfilename)
probs = get_probs(exactfilename,celcius)
df = get_freeE(probs,op='numBoundDomains')
plot_test(df.index.values, df.tolist(), name=directory+'/FreeEnergy.pdf')
df = get_freeE(probs,op='numStack')
plot_test(df.index.values, df.tolist(), name=directory+'/FreeEnergyStack.pdf')
df = get_freeE(probs,op='numStapleCopies')
plot_test(df.index.values, df.tolist(), name=directory+'/FreeEnergyCopies.pdf')
write_weights(probs,directory+'/weights.csv',top,celcius)