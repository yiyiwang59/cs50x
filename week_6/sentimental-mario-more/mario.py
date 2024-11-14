from cs50 import get_int

size = 0
while size <= 0 or size > 8:
    size = get_int("Height: ")


for i in range(1, size+1):
    row = "#"*i
    print(" " * (size-i), end="")
    print(row ,"", row)
