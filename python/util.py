import base64
from consts import ENG_FREQS
from math import sqrt
from collections import Counter

def hex_2_byte(s):
    return "".join([chr(int(s[i : i + 2], 16)) for i in range(0, len(s), 2)])

def byte_2_hex(s):
    return "".join([f'{ord(c):0<2x}' for c in s])

def b64_2_byte(s):
    return base64.b64decode(s)

def b64_2_byte(s):
    return base64.encode(s)

def englishScore(freqs):
    return sqrt(sum([(ENG_FREQS[i] - freqs.get(chr(i), 0))**2 for i in range(256)]))

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
            ptext = dec
            bestScore = score
            bestKey = k

    return (ptext, bestKey, bestScore)