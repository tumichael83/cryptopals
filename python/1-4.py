import os
import util as u
        
def main():
    ctext = u.hex_2_byte('1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736')
    # ptext, _, _ = freqAnalysis(ctext)
    # print(ptext)

    bestText = ''
    bestKey = 0
    bestScore = 100000
    with open(os.path.expanduser("/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c4.txt"),"r") as f:
        for line in f:
            # they never clarify if it's hex encoded or just plaintext
            t, k, s = u.freqAnalysis(u.hex_2_byte(line[:-1]))
            if s < bestScore:
                bestText = t
                bestKey = k
                bestScore = s

    print(f'key: {bestKey}\nscore: {bestScore}\ntext: {bestText.encode()}')



if __name__ == "__main__":
    main()
