#!/usr/bin/env python3
import sys
import os
import random
import math
import imp
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



class Dataset():
    def __init__(self, arquivo, qtd_atributos, qtd_clusters):
        self.arquivo = arquivo
        self.qtd_atributos = qtd_atributos
        self.qtd_clusters = qtd_clusters
        self.particao = self.inicializaDataset()
        self.centroides = self.inicializaCentroides()

    def inicializaDataset(self):
        d = {}
        for line in self.arquivo:
            objeto = line.replace('\n', '').split('\t')

            d.setdefault(objeto[0], [])
            for j in range(1, self.qtd_atributos+1):
                d[objeto[0]].append(float(objeto[j]))
            # inicializa espaço para cluster em que se encontra em ultima posição dos valores
            d[objeto[0]].append(0)
        print('Quantidade de objetos:', len(d))
        return d

    def inicializaCentroides(self):

        if self.qtd_clusters > len(self.particao):
            print('A quantidade de clusters deve ser menor do que a quantidade de objetos')
            self.arquivo.close()
            sys.exit(0)

        keys = {}
        # escolhe centroides aleatorio
        randkey = random.sample(list(self.particao.keys()), self.qtd_clusters)
        print('Clusters iniciais:', sorted(randkey))
        for i in range(0, self.qtd_clusters):
            keys.setdefault(randkey[i], [])
            for item in self.particao[randkey[i]]:
                keys[randkey[i]].append(item)
            # coloca cada centroide em um cluster
            keys[randkey[i]][len(keys[randkey[i]])-1] = i
        keys = sorted(keys.items(), key=lambda x: x[1][self.qtd_atributos])
        keys = [x[1] for x in keys]
        return keys

    def defineParticao(self, qtd_iterações):

        for i in range(0, qtd_iterações):
            for k in self.particao:
                self.particao[k][len(self.particao[k])-1] = self.menorDistancia(self.particao[k])
            self.centroides = self.novosCentroides()

    def menorDistancia(self, objeto):
        # definir qual centroide está mais proximo e retornar o mesmo
        distancias = []
        for i in range(0, len(self.centroides)):
            dist = 0
            for j in range(0, self.qtd_atributos):
                dist = dist + ((float(self.centroides[i][j]) - float(objeto[j]))**2)
            dist = math.sqrt(dist)
            distancias.append(dist)
        for i in range(0, len(distancias)):
            if distancias[i] == min(distancias):
                # retorna qual centroide está mais proximo do objeto
                return i

    def novosCentroides(self):
        # fazer media ponderada de todos os objetos em determinado cluster
        centroides = []
        for i in range(0, self.qtd_clusters):
            atributos = []
            for j in range(0, self.qtd_atributos):
                soma = 0
                conta = 0
                for k in self.particao:
                    if self.particao[k][len(self.particao[k])-1] == i:
                        soma = soma + float(self.particao[k][j])
                        conta = conta + 1
                soma = soma/float(conta)
                atributos.append(soma)
            centroides.append(atributos)
        return centroides

    def geraResultado(self, novo_arquivo):
        # ordena items
        particao = natsorted(self.particao.items(), key=lambda x: x[0])
        nome = novo_arquivo + '.clu'
        saida = open(nome, 'w')
        for item in particao:
            saida.write(str(item[0]) + '\t' + str(item[1][self.qtd_atributos]+1) + '\n')

        saida.close()

def main():

    os.system('cls' if os.name == 'nt' else 'clear')
    if( len(sys.argv)!=4): # sys.argv[1] - arquivo, sys.argv[2] - k clusters, sys.argv[3] - qtd iterações
        print("Chamada inválida use: $ python3 kmeans.py <dataset> <qtd_clusters> <qtd_iterações>")
        sys.exit(1)

    print('**Certifique-se de que o dataset tem seus atributos separados por tab. Caso contrário, a partição gerada é errônea.**')

    if(int(sys.argv[3]) < 1):
        print('O mínimo de iterações é 1')
        sys.exit(1)

    try:
        dataset = open(os.path.expanduser(sys.argv[1]), 'r')
    except (OSError, IOError) as e:
        print('Erro ao abrir arquivo de dataset')
        sys.exit(1)

    nome = ntpath.split(sys.argv[1])[1].split('.')[0]

    # le primeira linha e conta quantidade de atributos
    atributos = len(dataset.readline().replace('\n','').split('\t')) - 1

    print('Resultado do K-Médias\n')
    print('Arquivo do dataset:', nome)
    #crio objeto e mando atributos: arquivo, qtd de atributos e qtd_clusters
    d = Dataset(dataset, atributos, int(sys.argv[2]))
    print('Quantidade de iterações:', sys.argv[3])

    #define partição a partir da quantidade de iterações
    d.defineParticao(int(sys.argv[3]))

    #gera resultado no arquivo
    d.geraResultado(nome + str(sys.argv[2]) + 'k')

    arquivosaida = os.path.dirname(os.path.abspath(__file__)) + '\\' if os.name == 'nt' else nome + str(sys.argv[2]) + 'k.clu\n'
    print('Partição resultante em:', arquivosaida)


    dataset.close()


if __name__ == "__main__":
    main()
