from json import dumps
from random import seed
from random import randint
from random import random
from random import choice
psList = []
seed(1)
for i in range(50):
    psList.append({
        "processo": i,
        "ciclos": randint(1,22),
        "max_quantum": randint(1,3),
        "init_type": choice(["cpu-bound","io-bound","memory-bound"]), 
        "timestamp": 0,
        "prioridade": randint(0,4)    
    })
f = open("processos.json","w")
f.write(dumps(psList))
f.close()

