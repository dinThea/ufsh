with open("meta/ALUNOS.meta", "rb") as f:
    byte = f.read(1)
    count = 0
    while byte != b"":
        # Do stuff with byte.
        print (byte, end=" ")
        count+=1;
        if count == 26:
            print ('')
            count = 0
