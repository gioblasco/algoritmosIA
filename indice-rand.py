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

#http://scikit-learn.org/stable/modules/generated/sklearn.metrics.adjusted_rand_score.html usar essa bib
