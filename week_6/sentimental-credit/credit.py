
num = input("Number: ")


def checksum(number:str) -> bool:
    every_other = ""
    leftover = 0
    every_other_sum = 0
    for i in range(1, len(number) +1):
        if abs(i) %2 == 0:
            product = int(number[-i])*2
            every_other += f"{product}"
        else:
            leftover += int(number[-i])
    for c in every_other:
        every_other_sum += int(c)
    result = str(every_other_sum + leftover)
    if result[-1] == "0":
        return True
    else:
        return False


def which_card(number:str) -> str:
    first_two_dig = int(number[:2])
    if len(number) == 15 and (first_two_dig == 34 or first_two_dig == 37):
        return "AMEX"
    elif len(number) == 16 and 51 <= first_two_dig and first_two_dig <= 55:
        return "MASTERCARD"
    elif (len(number) == 13 or len(number) == 16) and number[0] == "4":
        return "VISA"
    else:
        return "INVALID"


if checksum(num):
    print(which_card(num))
else:
    print("INVALID")
