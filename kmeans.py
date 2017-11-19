import sys
import os
import csv
import json


class Dataset():
    def __init__(self, arquivo, qtd_atributos, qtd_clusters, qtd_iteracoes):
        self.arquivo = arquivo
        self.qtd_atributos = qtd_atributos
        self.qtd_clusters = qtd_clusters
        self.qtd_iteracoes = qtd_iteracoes
        self.particao_inicial = inicializaDataset()
        self.centroides = inicializaCentroides()

        def inicializaDataset(self):

        def inicializaCentroides(self):


def main():
    os.system('cls' if os.name == 'nt' else 'clear')
    if( len(sys.argv)!=4):
        print("Chamada inválida use: $ python kmeans.py <dataset> <qtd_clusters> <qtd_iterações>")
        sys.exit(1)

    # sys.argv[1] - arquivo, sys.argv[2] - k clusters, sys.argv[3] - qtd iterações
    dataset = open(os.path.expanduser(sys.argv[1]), 'r')

    # le primeira linha e conta quantidade de atributos
    atributos = len(dataset.readline().replace('\n','').split('\t')) - 1

    d = Dataset(dataset, atributos, int(sys.argv[2]), int(sys.argv[3]))

    dataset.close()


if __name__ == "__main__":
    main()
