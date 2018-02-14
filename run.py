from pyzbar.pyzbar import decode
from PIL import Image
import requests
import time

def download_img(number):
    url = base_url +  str(number) + ext
    tmp_path = 'tmp_image.png'
    img = s.get(url,stream=True)
    img_file = open(tmp_path,'wb')
    for Chunk in img.iter_content(1024):
        img_file.write(Chunk)
    img_file.close()
    a = decode(Image.open(tmp_path))
    a = a[0][0]
    return str(a,'utf-8')


def extract_equation(data):
    print(data)
    a = data.split(':')
    equation = a[1].strip(' ')
    number = 0
    parameters = equation.split(',')
    number = int(parameters[0])
    for i in parameters[1:]:

        operation = i[0]
        right_side = int(i[1:])
        print("Number: " + str(number) + " operation: " + str(operation) + " right_side: " + str(right_side))
        if operation == '^':
            number = int(number) ^ right_side
        elif operation == '*':
            number = int(number) * right_side
        elif operation == '+':
            number = int(number) + right_side
        elif operation == '-':
            number = int(number) - right_side
    return number

s = requests.session()
base_url = 'http://gynvael.coldwind.pl/qrmaze/'
ext = '.png'
number = 'start'

for i in range(10000):
    img = download_img(number)
    number = extract_equation(img)
    print("i:" + str(i) + " Number: " + str(number))
