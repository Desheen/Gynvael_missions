# Gynvael_missions #46
Hello, Everyone.

if you watch Gynvael English (trying to be weekly) Stream, you can find it [here](https://www.youtube.com/channel/UCCkVMojdBWS-JtH7TliWkVg).  

In his #46 Stream, There was a mission about barcode decoding.

You can find the Archive Version of the Stream [here](https://youtu.be/PiBfI7wltM8?t=16m9s), and the mission details [here](http://goo.gl/rNHu7b).

![alt text](https://i.imgur.com/JXBYNHT.png)
Now, Going to the link in the details. it will tell you start with start.png, this link. Its barcode image, lets use one of the online service to read what it say !, you can use this website onlinebarcodereader.com

and there is already a Enter url, where you can enter a url to the barcode image, and then it will decode it for you, here is what is the content.

`Calc value, add .png, repeat: 84905785,*577,-745,-342,*954,-672,+909,+644,-556,-524,*622`

its simple calculation equation, you can use python to do this.  
![alt text](https://i.imgur.com/x7jkxLO.png)

now the result we got the result, lets add .png as its say, http://gynvael.coldwind.pl/qrmaze/29070456023771126.png

now if you repeat the process, of reading the barcode, you will find a new equation, and so on, i did around ~20 equation, until i realize, that this should be automated, so lets see, how we can do this, first we going to this with python, cuz python make it easy :P.

we need a way to download image from the internet, then decode the barcode, then solve the equation.

im going to use these libraries, but there is always alternative.

1. pyzbar
2. Pillow
3. requests

using request, you can simple download an image, or any other file if's matter by:

```python
import requests
img = requests.get(url,stream=True)
img_file = open('/tmp/barcode.png','wb')
for Chunk in img.iter_content(1024):
     img_file.write(Chunk)
img_file.close()
```

now, you have an image at  /tmp/barcode.png, lets make a function, because we going to download a lot of images, and we can overwrite the existence one, as its not useful to keep it.

```python
import requests
def download_img(url):
 tmp_path = 'tmp_image.png'
 img = requests.get(url,stream=True)
 img_file = open(tmp_path,'wb')
 for Chunk in img.iter_content(1024):
 	img_file.write(Chunk)
 img_file.close()

download_img('http://gynvael.coldwind.pl/qrmaze/29070456023771126.png')
```

Now that we have a function that will download an image, we need another function decode barcode images.  

```python
def extract_str_from_barcode(path):
	a = decode(Image.open(path))
	a = a[0][0]
	return str(a,'utf-8')

print(extract_str_from_barcode('tmp_image.png'))
```


The function extract_str_from_barcode, will return a string, which is another equation, The only missing function for now is a function to solve the equation, Thankful Gynvael formatted it, so its easy to parse it:

`Calc value, add .png, repeat: 84905785,*577,-745,-342,*954,-672,+909,+644,-556,-524,*622`

notice the { : } in the middle of the equation, we can split by it, then each operation splited by { , }

```python
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
```

This function will take Gynvael formatted equation, and solve it and return the next image number.

and that's it actually check `run.py` to see the full code.
