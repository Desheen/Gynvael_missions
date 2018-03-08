Hello, Everyone.

if you watch Gynvael English (trying to be weekly) Stream, you can find it [here](https://www.youtube.com/channel/UCCkVMojdBWS-JtH7TliWkVg).  

In his #46 Stream, There was a mission about barcode decoding.

You can find the Archive Version of the Stream [here](https://youtu.be/PiBfI7wltM8?t=16m9s), and the mission details [here](http://goo.gl/rNHu7b).

![alt text](https://i.imgur.com/JXBYNHT.png).  
Now, Going to the link that contain details of the mission. start with start.png, this [link](http://gynvael.coldwind.pl/qrmaze/start.png). Its barcode image, lets use one of the online service to read what it say !, you can use this website onlinebarcodereader.com

and there is already a Enter url option, where you can enter a url to the barcode image, and then it will decode it for you, here is the content of the barcode image. 

`Calc value, add .png, repeat: 84905785,*577,-745,-342,*954,-672,+909,+644,-556,-524,*622`

its simple calculation equation, you can use python to solve it.  
![alt text](https://i.imgur.com/x7jkxLO.png)

We get a number as a result, lets add .png to it, as the message say, it will look like this: http://gynvael.coldwind.pl/qrmaze/29070456023771126.png

its a new image, and you will repeat the process of reading it's content and solve calculation equation, and so on, i did around ~20 iteration, until i realize, that it should be automated, so lets see, how we can do it, we going to this with python, because python rocks :P .

We going to need the following:  
	1. Download an image. 
	2. Decode the image to get string.
	3. Parse the string and solve the equation.

Im going to use these libraries, but there is always alternative:
	1. Pyzbar
	2. Pillow
	3. Requests


By using request, you can simply download an image, or any other file if's matter by:

```python
import requests
img = requests.get(url,stream=True)
img_file = open('/tmp/barcode.png','wb')
for Chunk in img.iter_content(1024):
     img_file.write(Chunk)
img_file.close()
```

Now, you have an image at  /tmp/barcode.png, lets make it a function, because we going to download a lot of images, and we can overwrite the existence one, because there is no need to keep it. 

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


This function extract_str_from_barcode, will return a string, which is another equation, The only missing function for us, is a function to parse & solve the equation, Thankfully Gynvael has made the message properly formatted, so its easy to parse it:

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

Add those function to a while loop, or for loop, and that's it. check `run.py` to see the full code.
