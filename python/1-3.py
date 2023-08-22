import os
from math import sqrt
import requests
from collections import Counter

text = requests.get(
    "http://ocw.mit.edu/ans7870/6/6.006/s08/lecturenotes/files/t8.shakespeare.txt"
).text

counts = Counter(text)
letterFreqs = {k: v / total for total in (sum(counts.values()),) for k, v in counts.items()}

def hex_2_byte(s):
    return "".join([chr(int(s[i : i + 2], 16)) for i in range(0, len(s), 2)])

def englishScore(freqs):
    return sqrt(sum([(v - freqs.get(k, 0))**2 for k,v in letterFreqs.items()]))

def freqAnalysis(ctext):
    ptext = ctext
    bestScore = 100000
    bestKey = 0
    for k in range(256):
        dec = ''.join([chr(ord(c) ^ k) for c in ctext])
        counts = Counter(dec)
        freqs = {k:v/total for total in (sum(counts.values()),) for k,v in counts.items()}

        score = englishScore(freqs)

        if score < bestScore:
            bestScore = score
            ptext = dec
            bestKey = k

    return (ptext, bestKey, bestScore)


        

def main():
    ctext = hex_2_byte('1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736')
    ptext, _, _ = freqAnalysis(ctext)
    print(ctext.encode())
    print(ptext)

if __name__ == "__main__":
    main()
