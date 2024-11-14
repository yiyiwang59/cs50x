from cs50 import get_string

text = get_string("Text: ")

def grade_level(text:str):
    letters = 0
    words = 1
    sentences = 0
    for c in text:
        if c == " ":
            words += 1
        elif c in ['?', '.', '!']:
            sentences += 1
        elif c.isalpha():
            letters += 1

    answer = calculate_index(letters, words, sentences)
    return answer

def calculate_index(count_letters: int, count_words: int, count_sentences:int):
    L = count_letters/count_words * 100.0
    S = count_sentences/count_words * 100.0
    result = 0.0588 * L - 0.296 * S - 15.8
    return round(result)

grade = grade_level(text)
if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade {grade}")

