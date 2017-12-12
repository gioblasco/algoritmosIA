#!/usr/bin/env python3
import sys
import os
import random
import math
import imp
import ntpath
from natsort import natsorted
from operator import itemgetter

class Dataset():
	def __init__(self, arquivo, nome, qtd_atributos, qtd_inicial, qtd_final):
		self.arquivo = arquivo
		self.nome = nome
		self.qtd_atributos = qtd_atributos
		self.qtd_objetos = 0
		self.qtd_inicial = qtd_inicial
		self.qtd_final = qtd_final
		self.objetos = []
		self.combinacoes = self.inicializaDistancias()

	def inicializaDistancias(self):
		obj = []
		for line in self.arquivo:
			aux = []
			nome = []
			objeto = line.replace('\n', '').split('\t')
			aux.append(objeto[0])
			nome.append(objeto[0])
			for j in range(1, self.qtd_atributos+1):
				aux.append(objeto[j])
			obj.append(aux)
			self.objetos.append(nome)
		self.qtd_objetos = len(obj)
		if self.qtd_final > self.qtd_objetos:
			print('A quantidade de clusters deve ser menor do que a quantidade de objetos')
			self.arquivo.close()
			sys.exit(0)
		print('Quantidade de objetos:', self.qtd_objetos)
		d = []
		i = 0
		for k in obj:
			for j in range(i, self.qtd_objetos):
				if k[0] != obj[j][0]:
					aux = []
					aux.append(k[0])
					aux.append(obj[j][0])
					aux.append(self.calculaDistancia(k, obj[j]))
					d.append(aux)
			i=i+1
		return sorted(d, key = itemgetter(2))

	def calculaDistancia(self, particao1, particao2):
		dist = 0
		for n in range(1, self.qtd_atributos):
			dist = dist + ((float(particao1[n]) - float(particao2[n]))**2)
		return dist

	def agrupaObjetos(self):
		for n in range(0, len(self.combinacoes)):
			if(len(self.objetos) == self.qtd_final or len(self.objetos) == self.qtd_inicial):
				self.imprimeSaida()
				self.qtd_final = self.qtd_final - 1
				if(len(self.objetos) == self.qtd_inicial):
					print('Fim da execução')
					sys.exit(0)
			mudou = False
			for o in range(0, len(self.objetos)):
				if self.combinacoes[n][0] == self.objetos[o][0] and self.combinacoes[n][1] not in self.objetos[o] and mudou == False:
					self.objetos[o].append(self.combinacoes[n][1])
					mudou = True
				elif mudou == True:
					for p in range(o, len(self.objetos)):
						if self.combinacoes[n][1] == self.objetos[p][0]:
							del self.objetos[o]
							break

	def imprimeSaida(self):
		lista = []
		for n in range(0, len(self.objetos)):
			for item in self.objetos[n]:
	  			lista.append(item + '\t' + str(n+1) + '\n')
		objetos = natsorted(lista, key=lambda x: x)
		nome = self.nome + str(len(self.objetos)) + 'k.clu'
		saida = open(nome, 'w')
		for item in objetos:
			saida.write(item)

		saida.close()

def main():
	os.system('cls' if os.name == 'nt' else 'clear')
	if(len(sys.argv)!=4): # sys.argv[1] - arquivo, sys.argv[2] - k clusters, sys.argv[3] - qtd iterações
		print("Chamada inválida use: $ python3 teste.py <dataset> <k_inicial> <k_final>")
		sys.exit(1)

	print('**Certifique-se de que o dataset tem seus atributos separados por tab. Caso contrário, a partição gerada é errônea.**')
	try:
		dataset = open(os.path.expanduser(sys.argv[1]), 'r')
	except (OSError, IOError) as e:
		print('Erro ao abrir arquivo de dataset')
		sys.exit(1)

	nome = ntpath.split(sys.argv[1])[1].split('.')[0]

	# le primeira linha e conta quantidade de atributos
	atributos = len(dataset.readline().replace('\n','').split('\t')) - 1

	print('Resultado do Single-Link\n')
	print('Arquivo do dataset:', nome)
	print('Quantidade de clusters inicial:', sys.argv[2])
	print('Quantidade de clusters final:', sys.argv[3])

	#crio objeto e mando atributos: arquivo, qtd_atributos, qtd_inicial e qtd_final
	d = Dataset(dataset, nome, atributos, int(sys.argv[2]), int(sys.argv[3]))
	d.agrupaObjetos()


if __name__ == "__main__":
		main()
