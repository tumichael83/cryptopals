import os

with open(
    os.path.expanduser(
        "/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c8.txt"
    ),
    "r",
) as f:
    count = 0
    for line in f.readlines():
        count += 1
        blocks = set([line[i:i+32] for i in range(0,len(line),32)])
        if len(blocks) < 10:
            print(blocks)
            print(count)
        
