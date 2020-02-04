import sqlite3
import requests
import random

from bs4 import BeautifulSoup as BS


def createWordList(line):
    wordList2 =[]
    wordList1 = line.split(',')
    for word in wordList1:
        leng = word.find('–')
        if leng >= 0:
            word = word[leng:]
        cleanWord = ""
        for char in word:
            if char in '!,–.?":;0123456789':
                char = ""
            cleanWord += char
        wordList2.append(cleanWord[1:])
    return wordList2


def getTitle(line):
    leng = line.find('–')
    return line[:leng]


conn = sqlite3.connect('data.db')

r = requests.get('https://www.listplanit.com/list-of-categories-for-an-organized-grocery-list/')
html = BS(r.content, 'html.parser')

body = html.find("div", {"class": "blog_content"})

uls = body.find_all('ul')[:-1]

for ul in uls:

    li = ul.find_all('li')
    lines = [ele.text.strip() for ele in li]

    for line in lines:
        if line:
            print(getTitle(line))
            words = createWordList(line)
            for word in words:
                if word:
                    print(word)
                    cursor = conn.execute("SELECT COUNT(name) from product")
                    result = cursor.fetchone()
                    number_of_rows = result[0] + 1
                    print(number_of_rows)
                    rand = random.randint(10, 100)
                    conn.execute("insert into product (ID,NAME,PRICE,GROUP_NAME) values (?, ?, ?, ?)",
                                 (number_of_rows, word, rand, getTitle(line)))

            conn.commit()

conn.close()
