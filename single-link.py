#!/usr/bin/env python3
import sys
import os
import random
import math
import imp
import numpy
try:
    imp.find_module('natsort')
except ImportError:
    print("ERRO: É necessário ter o módulo natsort instalado em sua máquina!",file=sys.stderr);
    sys.exit(1)

try:
    imp.find_module('ntpath')
except ImportError:
    print("ERRO: É necessário ter o módulo ntpath instalado em sua máquina!",file=sys.stderr);
    sys.exit(1)
import ntpath
from natsort import natsorted

def main():
    os.system('cls' if os.name == 'nt' else 'clear')
    if( len(sys.argv)!=4): 
        print("Chamada inválida use: $ python3 single-link.py <dataset> <corte_mínimo> <corte_máximo>")
        sys.exit(1)
    try:
        dataset = open(os.path.expanduser(sys.argv[1]), 'r')
    except (OSError, IOError) as e:
        print('Erro ao abrir arquivo de dataset')
        sys.exit(1)
    print('**Certifique-se de que o dataset tem seus atributos separados por tab.\n  Caso contrário, a partição gerada é errônea.**')

    num_lines = sum(1 for line in dataset) - 1
    dataset.seek(0)
    matriz_custos = numpy.zeros(shape=(num_lines,num_lines))
    i = 0
    elements = []
    for line in dataset:
        i = i+1
        if(i==1):
            continue
        elements.append(line.replace('\n', '').split('\t') )

    dataset.close()

    print("Indo Calcular a matriz dissociativa...")

    for i, elem1 in enumerate(elements):
        for j in range(i,num_lines):
            dx =  float(elem1[1]) - float(elements[j][1])
            dy =  float(elem1[2]) - float(elements[j][2])
            matriz_custos[i][j] = math.sqrt( dx*dx + dy*dy)
            
    print(matriz_custos)
if __name__ == "__main__":
    main()

    
