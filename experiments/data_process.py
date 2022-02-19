import numpy as np
import pandas as pd

data = pd.read_csv("resources/unigram_freq.csv")
print(data)

words = []
frequency = []
for i in range(len(data)):
    if len((str)(data['word'][i])) == 5:
        words.append((str)(data['word'][i]))
        frequency.append((str)(data['count'][i]))

mp = {}

with open("resources/big_list.txt", "r") as f:
    for l in f.readlines():
        mp[l.strip()] = '0'
    f.close()

for i in range(len(words)):
    mp[words[i]] = frequency[i]

with open("resources/big_list_frequency.txt", "w") as x:

    with open("resources/big_list.txt", "r") as f:
        for l in f.readlines():
            x.write(mp[l.strip()]+'\n')
            # print(len(l.strip()))
        f.close()
    x.close()
