import requests
from bs4 import BeautifulSoup as BS

r = requests.get('http://www.bezengi.ru/ru/shop')
html = BS(r.content, 'html.parser')
#
# for el in html.table:
    # group = el.select('b')
    # if group:
    #     for g in group:
    #         print(g.text)

    # text = el.select('p')
    # print(text)
    # print("\n")

    # data_root = el.select('td > b')
    # data_text = el.select('td > p')
    # print(data_root)
    # print("\n")
    # print(data_text)

data = []
table = html.find('table')

rows = table.find_all('tr')
for row in rows:
    cols = row.find_all('td')
    group = row.find_all('b')
    cols = [ele.text.strip() for ele in cols]
    group = [ele.text.strip() for ele in group]
    for ele in cols:
        if ele:
            print(ele)

    # for ele in group:
    #     if ele:
    #         print(ele)
