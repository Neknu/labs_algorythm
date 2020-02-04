import requests
from bs4 import BeautifulSoup as BS

r = requests.get('http://www.bezengi.ru/ru/shop')
html = BS(r.content, 'html.parser')

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
