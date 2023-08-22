import os
from math import sqrt
import base64 as b64
from collections import Counter

letterFreqs = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0.022801660401168957, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0.23706244495666062, 0.0016203146862179265, 
8.610898943039636e-05, 1.8321061580935398e-07, 0, 
1.8321061580935398e-07, 3.8474229319964335e-06, 
0.005692170622580818, 0.00011505626672827429, 
0.00011523947734408364, 1.15422687959893e-05, 
0, 0.015238359759327207, 0.0014792425120447239, 
0.014295008298524843, 9.160530790467698e-07, 
5.477997412699684e-05, 0.00017001945147108048, 
6.705508538622355e-05, 6.045950321708681e-05, 
1.703858727026992e-05, 1.5023270496367025e-05, 
1.15422687959893e-05, 7.511635248183512e-06, 
7.328424632374159e-06, 0.00017368366378726755, 
0.0003347257950836897, 0.0031510393813050787, 
8.574256819877765e-05, 1.8321061580935398e-07, 
8.07958815719251e-05, 0.0019193144112187922, 
1.4656849264748318e-06, 0.008150307454894921, 
0.0028238252214695726, 0.0039384786080536825, 
0.0028732920877380984, 0.00780165765300972, 
0.002145945942974963, 0.0020453633148956275, 
0.0033824343890722927, 0.010224251625856808, 
0.00037869634287793463, 0.0011351729755547572, 
0.0043710388719795665, 0.002907918894126066, 
0.005008611814996119, 0.006084241340412836, 
0.002187351542147877, 0.00021582210542341897, 
0.005307611539996984, 0.006231176254291938, 
0.007291782509212288, 0.0025885827907703622, 
0.0006558940045974872, 0.003022242318391103, 
0.00011102563318046851, 0.0016670333932493116, 
9.746804761057631e-05, 0.000381994133962503, 
0, 0.0003805284490360282, 0, 
1.3007953722464131e-05, 1.8321061580935398e-07, 
0.044825042106379775, 0.008527171691614762, 
0.012217949547094197, 0.024509732972359564, 
0.0741308625793966, 0.01260543999953098, 
0.010449417472686504, 0.04001429775645776, 
0.036309412683561006, 0.0004968671900749679, 
0.005351948509022848, 0.026778246817310985, 
0.017511270659058054, 0.03955956900801894, 
0.051553818393209924, 0.008523873900530193, 
0.0004404383204056869, 0.03827159837887919, 
0.039386251765463294, 0.053126498319317414, 
0.021035876485998403, 0.006227145620744132, 
0.013354954628807049, 0.0008588913669142513, 
0.015622552420679421, 0.00020134846677448, 
0, 6.045950321708681e-06, 3.6642123161870795e-07, 
1.8321061580935398e-07, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0]

letterFreqs = {chr(i) : letterFreqs[i] for i in range(256)}

def hex_2_byte(s):
    return "".join([chr(int(s[i : i + 2], 16)) for i in range(0, len(s), 2)])


def englishScore(freqs):
    return sqrt(sum([(v - freqs.get(k, 0)) ** 2 for k, v in letterFreqs.items()]))


def freqAnalysis(ctext):
    ptext = ctext
    bestScore = 100000
    bestKey = 0
    for k in range(256):
        dec = "".join([chr(c ^ k) for c in ctext])
        counts = Counter(dec)
        freqs = {
            k: v / total for total in (sum(counts.values()),) for k, v in counts.items()
        }

        score = englishScore(freqs)

        if score < bestScore:
            bestScore = score
            ptext = dec
            bestKey = k

    return (ptext, bestKey, bestScore)


def solve(ctext):

    bestKey = ""
    bestScore = 100000000
    for numKeys in range(1,64):
        ptext = []
        keyList = []
        score = 0
        for i in range(numKeys):
            stream = ctext[i::numKeys]
            p, k, s = freqAnalysis(stream)
            ptext.append(p)
            keyList.append(k)
            score += s

        score /= numKeys
        print(score)
        if score < bestScore:
            bestKey = keyList
            bestScore = score
            bestPtext = ptext

    for i in bestKey:
        print(i)
    print(bestScore)

    knum = len(bestKey)
    out = ""
    for i in range(len(ctext)):
        out += chr(ctext[i] ^ bestKey[i % knum])

    print(out)

    # number of keys is 29
    return(bestKey, out)

def main():
    with open(
        os.path.expanduser(
            "/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c6.txt"
        ),
        "rb",
    ) as f:
        ctext = f.read().replace(b'\n',b'')
        print(len(ctext))
        ctext = b64.b64decode(ctext)[110:116]
        print(len(ctext))

    # key, t = solve(ctext)
    # key = "".join([chr(k) for k in key])
    # print(key)
    with open('solution.txt', 'w') as f:
        # f.write(f'key: {key}\n\n')
        # f.write(t)
        for c in ctext:
            f.write(f'{c}\n')

if __name__ == "__main__":
    main()


# I'm back and I'm ringin' the bell 
# A rockin' on the mike while the fly girls yell 
# In ecstasy in the back of me 
# Well that's my DJ Deshay cuttin' all them Z's 
# Hittin' hard and the girlies goin' crazy 
# Vanilla's on the mike, man I'm not lazy. 

# I'm lettin' my drug kick in 
# It controls my mouth and I begin 
# To just let it flow, let my concepts go 
# My posse's to the side yellin', Go Vanilla Go! 

# Smooth 'cause that's the way I will be 
# And if you don't give a damn, then 
# Why you starin' at me 
# So get off 'cause I control the stage 
# There's no dissin' allowed 
# I'm in my own phase 
# The girlies sa y they love me and that is ok 
# And I can dance better than any kid n' play 

# Stage 2 -- Yea the one ya' wanna listen to 
# It's off my head so let the beat play through 
# So I can funk it up and make it sound good 
# 1-2-3 Yo -- Knock on some wood 
# For good luck, I like my rhymes atrocious 
# Supercalafragilisticexpialidocious 
# I'm an effect and that you can bet 
# I can take a fly girl and make her wet. 

# I'm like Samson -- Samson to Delilah 
# There's no denyin', You can try to hang 
# But you'll keep tryin' to get my style 
# Over and over, practice makes perfect 
# But not if you're a loafer. 

# You'll get nowhere, no place, no time, no girls 
# Soon -- Oh my God, homebody, you probably eat 
# Spaghetti with a spoon! Come on and say it! 

# VIP. Vanilla Ice yep, yep, I'm comin' hard like a rhino 
# Intoxicating so you stagger like a wino 
# So punks stop trying and girl stop cryin' 
# Vanilla Ice is sellin' and you people are buyin' 
# 'Cause why the freaks are jockin' like Crazy Glue 
# Movin' and groovin' trying to sing along 
# All through the ghetto groovin' this here song 
# Now you're amazed by the VIP posse. 

# Steppin' so hard like a German Nazi 
# Startled by the bases hittin' ground 
# There's no trippin' on mine, I'm just gettin' down 
# Sparkamatic, I'm hangin' tight like a fanatic 
# You trapped me once and I thought that 
# You might have it 
# So step down and lend me your ear 
# '89 in my time! You, '90 is my year. 

# You're weakenin' fast, YO! and I can tell it 
# Your body's gettin' hot, so, so I can smell it 
# So don't be mad and don't be sad 
# 'Cause the lyrics belong to ICE, You can call me Dad 
# You're pitchin' a fit, so step back and endure 
# Let the witch doctor, Ice, do the dance to cure 
# So come up close and don't be square 
# You wanna battle me -- Anytime, anywhere 

# You thought that I was weak, Boy, you're dead wrong 
# So come on, everybody and sing this song 

# Say -- Play that funky music Say, go white boy, go white boy go 
# play that funky music Go white boy, go white boy, go 
# Lay down and boogie and play that funky music till you die. 

# Play that funky music Come on, Come on, let me hear 
# Play that funky music white boy you say it, say it 
# Play that funky music A little louder now 
# Play that funky music, white boy Come on, Come on, Come on 
# Play that funky music 
