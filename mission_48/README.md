Hello, Everyone.

In Gynvael English (trying to be weekly) Stream, you can find it [here](https://www.youtube.com/channel/UCCkVMojdBWS-JtH7TliWkVg).  

In his #48 Stream, There was a mission about derive key.

You can find the Archive Version of the Stream [here](https://www.youtube.com/watch?v=zJw7CuSc8Sg), and the mission details [goo.gl/Cio3tV ](http://goo.gl/Cio3tV ).  


Big shout out to Guest011011 , he stayed with me in Gynvael English IRC channel in freenode, and we discussed the mission and we tried to walk our way to solve it, and here is my solution :).  


```
MISSION 021            goo.gl/Cio3tV               DIFFICULTY: ██████░░░░ [6/10]
┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅
Dear Agent,

We've managed to snatch the source code of what was supposed to be an executable
containing a secret message. Unfortunately even within the source code the
secret data is already encrypted.

Please take a look and see if you can recover the message.

  goo.gl/nzXX7B

Good luck!
--
If you find the answer, put it in the comments under this video! If you write a
blogpost / post your solution / code online, please add a link as well!
If you tweet about it, include @gynvael to let me know :)

```

[goo.gl/nzXX7B](http://goo.gl/nzXX7B) opening this link, you will find C code that contain 3 functions:
	1.check_password.  
	2.derive_key.  
	3.decrypt the data. 

The main function simple take an input and call the other functions.  
The check_password function take an input ( password ) and if its not 41 long, then return false.
and if you provide a 41 password long, then it will md5 hash it and compare to hash value:
57d9b1fd2552ff0b8e5aeb18754a9b03

if it does not match the hash then return false, or if it's matched ( which is impoissible unless you are gynvael ) then return true.

second function is derive_key, and there is a uint64_t key = 0xf8a45191c23a75be.  
it will go though that 41 byte password ( 41 character we know that from the if statement in  the check_password function ) and add each value to this key, and it will hash the total value ( sum of the key and password ) with md5.  

Finally the decrypt function, which will xor each element of the data array with 2 hex digit from the key ( 16 byte - 32 hex digit ), the data array has 78 element, but the key is only 16 byte long, so that's probably why there is % 16.

Note that MD5Update has an argument length, first time in check_password, the length was strlen(password), but in  dervie_key the length is 8 byte only.  

So first i thought because its an md5 hash, There is possibility that the vulnerability is hash collision, so you try to generate the same hash value from different input.
But in the derive key function you will have the add ( sum ) the value of the password with the key, so you will need exactly the same value of the original key, and knowing that hash is one way function, you can get the same hash value for different input, but having the hash value and even if you know the input lenght, its impossible to get the original input.  
I spent a couple of hours trying to brute force it, was silly way to get the solution.  

but if you think about that derive_key function, the length in MD5Update is 8 byte, so it will take only 8 byte from the key.( try to play with MD5Update, declare two string with the same first 8 byte, after the 8th byte put random value, and try to hash them with length of 8 you will find out that they give the same hash ).  

The maximum value of input to decrypt that data iss 0xffffffffffffffff ( mathematically speaking ) and the minimum value is 0xf8a45191c23a75be ( not really  ).  
substracting them from each other will get you: 530209169652156993 which is very big number to iterate through, even with i7 :P.  

So another thing to put into consideration, that the password should be in the printable asci character range, which is from 32(0x20) - 128 (0x80).  ( according to this [page](https://www.juniper.net/documentation/en_US/idp5.1/topics/reference/general/intrusion-detection-prevention-custom-attack-object-extended-ascii.html) )
So minimum value of the password that we will add to the key is 1312 ( 41 \* 32 ) but most important is the max value 5207 ( 41 * 127 ).  

I made some changes to the original code, i removed the check_password function ( completely ).  

and this is my modification to dervie_key:  
```  
void derive_key(int password,unsigned char *output_key)
{
  uint64_t key = 0xf8a45191c23a75be;
  key += password;
  MD5_CTX md5;
  MD5_Init(&md5);
  MD5_Update(&md5, &key, 8);
  MD5_Final(output_key, &md5);
}
```

Since we going to add the total value of password to the key, we dont need to loop over the string ( char array if that matter ) anymore.
```
// the value in the output array does not matter, because they will be overwritten, most important is the number of element
// i just copied data array.
unsigned char output[] = {
    0xfa, 0xa0, 0x3e, 0xe8, 0xf5, 0x19, 0xde, 0x4e,
    0xac, 0x9c, 0x9c, 0xfc, 0x33, 0x6a, 0x4d, 0xae,
    0xd6, 0xe3, 0x28, 0xe4, 0xe6, 0x5c, 0xc3, 0x01,
    0xb6, 0xc9, 0x86, 0xbd, 0x28, 0x6a, 0x1e, 0xaf,
    0xd2, 0xe7, 0x28, 0xa1, 0xb0, 0x5e, 0xc8, 0x4e,
    0xbd, 0x92, 0xd4, 0xc9, 0x2d, 0x6a, 0x4d, 0xaf,
    0xd6, 0xe3, 0x3f, 0xe8, 0xe4, 0x19, 0xd7, 0x49,
    0xab, 0xdd, 0x87, 0xf8, 0x65, 0x66, 0x1e, 0xe6,
    0x93, 0xd3, 0x34, 0xfe, 0xe4, 0x5c, 0xca, 0x01,
    0x9d, 0xce, 0x81, 0xf4, 0x21, 0x0f,
};

void decrypt(unsigned char *data, size_t sz, int password,unsigned char *output) {
  unsigned char result[16];
  derive_key(password, result);

  // you need to keep the data array the same. and save the result in different array.
  for (size_t i = 0; i < sz; i++) {
    output[i] = data[i] ^ result[i % 16];
  }
}
```

Make an array for the output, so we store the result.  
Decrypt function is the same but storing the result in output instead of data.  

```
// this will only print of all the element in the output array is printable ( after decrypting )
void print_data(int index)
{
  bool printable = true;
  //printf("\ni:%d------------\n\n",index);
  for(int i = 0; i < sizeof(output);i++)
  {
    //printf("%x   -   ",output[i]);
    if( output[i] > 0x80  )
      {
        printable = false;
        break;
      }
  }
  if(printable){
    printf("i:%d - printable: %d\n\n",index,printable);
    puts((char*)output);
  }

}
```

i made a custom print function, that will make sure to print only the array that all it's element is in the printable character range, then you just need to loop over:
```
  uint64_t end = 5207;
  for(uint64_t i = 0;i <=end;i++)
  {
    decrypt(data, sizeof(data), i,output);
    print_data(i);
  }
```

There is some clean up can be done, but since it's working, doesnt really matter. ( yea lazy way).  

Its funny that i was writing email to gynvael, telling him what is the solution after i spent long time trying to solve it, while writing the email and trying to explain my effort to gynvael i found that i have a mistake in the condition of the print function ( i kinda excluded all the right cases :$, yea it happen ).  

You can compile my code with: 
```
gcc -o solve solution.cpp -lcrypto
./solve
```
( you need to install openssl library ( google it ) ).
It was fun mission after all. thank you for reading my write up and i hope you find it helpful.  

