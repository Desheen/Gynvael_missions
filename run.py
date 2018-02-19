import requests
from pyzbar.pyzbar import decode
from PIL import Image


def download_img(number):
    url = base_url + str(number) + ext
    img = s.get(url,stream=True)
    img_file = open(tmp_path,'wb')
    for Chunk in img.iter_content(1024):
        img_file.write(Chunk)
    img_file.close()

def extract_str_from_barcode(path):
    a = decode(Image.open(path))
    a = a[0][0]
    barcode_text = str(a,'utf-8')
    print("[barcode text] " + barcode_text)
    return barcode_text


def solve_equation(equation):
    eq = equation.split(':')
    right_half = eq[1]
    right_half = right_half.strip(' ')
    operations = right_half.split(',')
    base_number = int(operations[0])
    for operation in operations[1:]:
        op = operation[0]
        right_operand = int(operation[1:])
        print("[Operation] " + str(base_number) + " " + str(op) + " " + str(right_operand))
        if op == '^':
            base_number = base_number ^ right_operand
        elif op == '*':
            base_number = base_number * right_operand
        elif op == '+':
            base_number = base_number + right_operand
        elif op == '-':
            base_number = base_number - right_operand
    return base_number


s = requests.session()
base_url = 'http://gynvael.coldwind.pl/qrmaze/'
ext = '.png'
number = 'start'
tmp_path = 'tmp_image.png'
iteration = 0
while True:
    print('------------- ' + str(iteration) + ' -------------')
    download_img(number)
    text = extract_str_from_barcode(tmp_path)
    number = solve_equation(text)
    iteration += 1