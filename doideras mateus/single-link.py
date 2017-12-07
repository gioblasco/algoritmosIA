#!/usr/bin/env python3
import sys
import os
import gc
import random
import math
import operator
import imp
import numpy as np
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

global i 
i = 0



def find_parent(x):
	global elements	
	if(elements[x][1] != x): #if the parent is not me
		elements[x][1] = find_parent(elements[x][1])   # then my parent is the parent of my parent
	return elements[x][1] # e caso alguem queira usar meu parent

def union(x,y):
	global elements
	xRoot = find_parent(x)
	yRoot = find_parent(y)

	if(xRoot == yRoot): # if we do not need to merge the elements
		return False #union not maked

	#otherwise, lets merge in a not naive mode:

	if(elements[xRoot][2] < elements[yRoot][2]):
		elements[xRoot][1] = yRoot

	elif (elements[xRoot][2] > elements[yRoot][2]):
		elements[yRoot][1] = xRoot
	else:
		elements[yRoot][1] = xRoot
		elements[xRoot][2] = elements[xRoot][2] +1
	return True # unioned with sucess.
	
def imprime_arquivo(k):
	global elements
	global num_lines
	f = open(os.path.dirname(os.path.abspath(__file__))+"saida_"+str(k)+".txt" ,"w")
	for c in range(0, num_lines):
		f.write(str(elements[c][0])+" "+str(find_parent(c)))
	f.close()
	print("Salvei o Arquivo "+os.path.dirname(os.path.abspath(__file__))+"saida_"+str(k)+".txt")

# convert an upper triangular Matrix nxn cordinates to an index
def m2v(x,y,n):
	return int((( n * (n-1) ) / 2) - ( ( (n-x) * (n-x-1) ) / 2 ) + y - x -1)

# convert an index to the coordinates of a upper triangular matrix                   
def v2m(k,n):
	i = n - 2 - math.floor(math.sqrt(-8*k + 4*n*(n-1)-7)/2.0 - 0.5)
	j = k + i + 1 - n*(n-1)/2 + (n-i)*((n-i)-1)/2
	return [ i , j ]


def main():
	global elements
	global num_lines

	if( len(sys.argv)!=4): 
		print("\nChamada inválida use: $ python3 single-link.py <dataset> <corte_mínimo> <corte_máximo>")
		sys.exit(1)
	try:
		dataset = open(os.path.expanduser(sys.argv[1]), 'r')
	except (OSError, IOError) as e:
		print('Erro ao abrir arquivo de dataset')
		sys.exit(1)
	print('**Certifique-se de que o dataset tem seus atributos separados por tab.\n  Caso contrário, a partição gerada é errônea.**')

	num_lines = sum(1 for line in dataset) - 1
	kmin = int(sys.argv[2])
	kmax = int(sys.argv[3])
	dataset.seek(0)
	vetor_custos = np.zeros(shape=( math.ceil( (num_lines**2 - num_lines) /2), 3 ) ) 
	i = 0
	elements = []
	for line in dataset:
			i = i+1
			if(i==1):
					continue
			elements.append(line.replace('\n', '').split('\t') )
			elements[-1][1] =  float(elements[-1][1])
			elements[-1][2] =  float(elements[-1][2])

	dataset.close()

	print("Indo Calcular o vetor de distâncias...")
	print("Num_lines é "+ str(num_lines))
	for i in range(0, num_lines):
		for j in range(i+1,num_lines): # calculando a distância euclidiana
			temp = m2v(i,j,num_lines)
			vetor_custos[ temp ][0] = math.sqrt( (elements[i][1] - elements[j][1]) ** 2 + (elements[i][2] - elements[j][2]) ** 2 )
			vetor_custos[temp][1] = i
			vetor_custos[temp][2] = j
	#np.sort(key=lambda distancia: distancia[0])
	#vetor_custos = np.array(vetor_custos, dtype=datatype)
	print(str(vetor_custos[0][0]) + " " + str(vetor_custos[0][1]) + " " + str(vetor_custos[0][2]))
	print(str(vetor_custos[1][0]) + " " + str(vetor_custos[1][1]) + " " + str(vetor_custos[1][2]))
	print(str(vetor_custos[2][0]) + " " + str(vetor_custos[2][1]) + " " + str(vetor_custos[2][2]))
	print("O menor é esse?: "+str(vetor_custos[m2v(0,11,1000)][0]) + " " + str(vetor_custos[m2v(0,11,1000)][1]) + " " + str(vetor_custos[m2v(0,11,1000)][2]))
	vetor_custos = sorted(vetor_custos, key=operator.itemgetter(0))
	print("Foi pai?")
	print(str(vetor_custos[0][0]) + " " + str(vetor_custos[0][1]) + " " + str(vetor_custos[0][2]))
	print(str(vetor_custos[1][0]) + " " + str(vetor_custos[1][1]) + " " + str(vetor_custos[1][2]))
	print(str(vetor_custos[2][0]) + " " + str(vetor_custos[2][1]) + " " + str(vetor_custos[2][2]))
'''	print(str(vetor_custos[3]))
	print(str(vetor_custos[4]))
	print(str(vetor_custos[5]))
	print(str(vetor_custos[6]))
	print(str(vetor_custos[7]))
	print(str(vetor_custos[8]))
	print(str(vetor_custos[9]))'''
	
	
'''

	print("Definindo clusters iniciais...")
	for i in range(0, num_lines): # assumindo os clusters iniciais de cada elemento como ele mesmo
		elements[i][1] = i # Parent
		elements[i][2] = 0 # Rank

	min_distancia = [None,None,None]

	print("Iterando na matriz de custos...")
	for k in range(0, num_lines - kmin): #agrupe de X clusters até argv[2] Clusters
		print("Iteração "+str(k)+" de "+str(num_lines))
		min_distancia[0] = sys.maxsize
		union(i,j)
		if( k >= kmin and k<= kmax):
			imprime_arquivo(k)
		
	
	for v in range(0, num_lines):
		elements[min_distancia[2]][1] = elements[min_distancia[1]][1] # mesclando elemento ao cluster

	'''

if __name__ == "__main__":
		main()

