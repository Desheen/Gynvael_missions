Hello, Everyone.

if you watch Gynvael English (trying to be weekly) Stream, you can find it [here](https://www.youtube.com/channel/UCCkVMojdBWS-JtH7TliWkVg).  

In his #48 Stream, There was a mission about derive key.

You can find the Archive Version of the Stream [here](https://www.youtube.com/watch?v=zJw7CuSc8Sg), and the mission details [goo.gl/Cio3tV ](http://goo.gl/Cio3tV ).  

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

check the code in [goo.gl/nzXX7B](http://goo.gl/nzXX7B), you will find a c code that have 3 funtions:  
	1.check_password.  
	2.derive_key.  
	3.decrypt the data. 

check password function take an input and if its not 41 long, then return false.
and if you provide a 41 password long, then it will hash and compare to hash value:
57d9b1fd2552ff0b8e5aeb18754a9b03

if it doesnt match the hash then return false, or return true.

second function is derive_key, and there is a uint64_t key = 0xf8a45191c23a75be
and then it will go though that 41 password long and add each value to this key.

and the hash it with md5 again.

finally decrypt, which will xor each value of the data with the key, the data is 78 element.
but the key is 16 byte long, so that why there is % 16.

note that md5_update here take a length, first time in check_password, it took strlen(password).  
but in the dervie_key it took 8 byte only.  

so first i thought because its an md5 hash, i can make collision, so i generate the same hash with different input.  
but in the derive key function you will have the add ( sum ) the value of the key, so you need exactly the same amount of the original key.  
and knowing that hash is one way function, you can get the same hash value for different input, but having the hash and even if you know the input lenght, its impossible to get the original key.  
i spent a couple of hour trying to brute force it, was silly way to get the solution.  

but if you think about that derive key function, the length in MD5Update is 8, so it will take only 8 byte from the key.  

the max value is 0xffffffffffffffff and the original value is 0xf8a45191c23a75be.  
substracting them will get you: 530209169652156993 which is very big to compute, even with i7 :P.  

so another thing to put into consideration, that the password should in the printable asci range, which is from 32(0x20) - 128 (0x80).  
so minimum value of the password that we will add to the key is 1312 ( 41 * 32 ) but most important the max value is 5207 ( 41 * 127 ).  

so i made some changes to the original code, i removed the check_password function ( completely ).  

and this is my modification to dervie key:  
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

since we add the total value of password to the key, we dont need to loop over the string anymore.
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

i made an array for the output, so i can put the result into it :) .  

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

i made a custom print function, that will make sure to print only the array that all it's element is printable.
then you just need to loop over:
```
  uint64_t end = 5207;
  for(uint64_t i = 0;i <=end;i++)
  {
    decrypt(data, sizeof(data), i,output);
    print_data(i);
  }
```

there is some clean up can be done, but since it's working, doesnt really matter. ( yea lazy way).  

funny thing is i was about to send Gynvael an email telling him what is the solution after i spent really long time.  
but it turns out that i had a mistake in my print function, i was excluding all the printable cases :P ( yea it happen you the wrong condition :P ).  

it was fun mission after all. thank you for reading my write up and i hope you find it helpful.  

