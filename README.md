# Gynvael_missions
## English Stream Number 46
Link : https://www.youtube.com/watch?v=pRzjT_MpFVw

mission is barcode read and text, then do some calculation.
library need:  
	1. pyzbar   
	2. Pillow   
	3. requests  

can be installed by:  
	pip install pyzbar Pillow requests
	
	
little explaination about the code:  
the library request let you act like little browser without javascript support, you can request certain page, and get the content of it.  

however, here the content is barcode image, and we need to extract the information which is equation, Gynvael made it easy by formating it properly with { : } and {  , }  

if you going to use request, please use session, like requests.session() so you dont overload Gynvael server :).  

good luck

