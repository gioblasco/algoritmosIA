#!/usr/bin/env python3
import sys
import os
import random
import math
import imp
try:
    imp.find_module('natsort')
except ImportError:
    print("ERRO: É necessário ter o módulo natsort instalado em sua máquina!",file=sys.stderr)
    sys.exit(1)
try:
    imp.find_module('ntpath')
except ImportError:
    print("ERRO: É necessário ter o módulo ntpath instalado em sua máquina!",file=sys.stderr)
    sys.exit(1)
try:
    imp.find_module('sklearn')
except ImportError:
    print("ERRO: É necessário ter o módulo sklearn instalado em sua máquina!",file=sys.stderr)
    sys.exit()
import ntpath
from natsort import natsorted
from sklearn.metrics.cluster import adjusted_rand_score

#http://scikit-learn.org/stable/modules/generated/sklearn.metrics.adjusted_rand_score.html usar essa bib

def ler_arquivo(arquivo):
    lista = []
    for line in arquivo:
        lista.append(line.replace('\n', '').split('\t')[1])

    return lista

def main():

    os.system('cls' if os.name == 'nt' else 'clear')
    if( len(sys.argv)!=3): # sys.argv[1] - arquivo, sys.argv[2] - k clusters, sys.argv[3] - qtd iterações
        print("Chamada inválida use: $ python3 indice-rand.py <partição_real> <partição_resultante>")
        sys.exit(1)

    try:
        real = open(os.path.expanduser(sys.argv[1]), 'r')
    except (OSError, IOError) as e:
        print('Erro ao abrir a partição real')
        sys.exit(1)

    clusters1 = ler_arquivo(real)
    real.close()

    try:
        result = open(os.path.expanduser(sys.argv[2]), 'r')
    except (OSError, IOError) as e:
        print('Erro ao abrir a partição resultante')
        sys.exit(1)

    clusters2 = ler_arquivo(result)
    result.close()

    print("Indice rand ajustado: ", adjusted_rand_score(clusters1, clusters2))


if __name__ == "__main__":
    main()
