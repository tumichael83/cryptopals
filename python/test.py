import base64 as b64


with open('/Users/michaeltu/Desktop/Independent-CS-Stuff/cryptopals/cpp/c6.txt', 'rb') as f:
    s = f.read()
    
with open('test.txt', 'w') as f:
    for c in b64.b64decode(s)[:60]:
        print(c)
        f.write(f'{c}\n')