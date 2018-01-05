/**
* SRT Algorithm
*
* Vijay K Shah - 12506602 
* Michael Jolley - 
**/


#include<iostream>
#include<iomanip>
using namespace std;

int delay = 0;
int totalNoOfShifts = 0;
string QUOTIENT, REMAINDER;

string complement (string a);
string shiftOverZeros(string AQ);
string shiftOverOnes(string AQ);
string getA(string AQ);
string addBitStrings( string first, string second );
string subtractB(string AQ, string B);
string addB(string AQ, string B);
void printOutput(string AQ);
string convertHexToBinary(string hexInput);
int checkInvalidInputs(string AQ, string B);
string properRemainder(string AQ);
string addZerosToFront(string str, int len);

int main(){
	string Q , A , B , AQ , comB;
	int t = 0, T;

	//input number of test cases
	cin>>T;

	while(t < T){
		cin >> AQ >> B;

		cout <<" -----------------------Test Case: "<<t+1<<"----------------------------\n";
		cout <<setw(30)<<" The input is: " << AQ <<"    "<<B <<endl;

		//check and convert the hexadecimal input to binary
		AQ = convertHexToBinary(AQ);
		B = convertHexToBinary(B);

		//get rid of "." for calculations
		AQ = AQ.substr(1, AQ.length());
		B  = B.substr(1, B.length());

		//check if AQ is twice the length of B
		if(AQ.length() < 2 * B.length() ){
			AQ = addZerosToFront(AQ, 2*B.length() - AQ.length());
		}

		cout << setw(30)<<" The dividend length: " << AQ <<" "<<AQ.length()<<endl;
		cout<< setw(30)<<" The divisor length: "  << B<<" "<<B.length()<<endl<<endl;

		delay = 0 , totalNoOfShifts = 0;
		int initAdj = 0, k = 0;
		
		//Check if the inputs are valid
		int invalid = checkInvalidInputs(AQ, B) ;
		if(invalid == -1){
			cout <<" Invalid Operation: Divisor can not be Zero \n";
		}

		else{
			if(invalid == -2)
			{
				cout <<" First k bits of dividend (AQ) is higher than that of divisor \n We need to correct this: \n\n";
				cout <<" We add 1 zero to divisor after the decimal and 2 zero's to the \n";
				cout <<" dividend repeatedly unless the high k bits of dividend (AQ) \n becomes smaller than that of divisor. \n\n";

				string A = AQ.substr(0,AQ.length()/2);
				while(A.compare(B) >= 0){
					AQ = AQ.insert(0,"00");
					B = B.insert(0,"0");
					A = AQ.substr(0,AQ.length()/2);
				}
			}

			string normB = B;
			// STEP 1: normalize B
			for(int i =0 ; i < B.length(); i++){
				if(B[i] == '0'){
					//push 0 to the end
					normB.push_back('0');
					//trim 0 in the front
					normB = normB.substr(1);
					initAdj++;
				}
				else
					break;
			}
			delay = delay + initAdj * 3; // 3 Δt for each shift required for normalizing B

			cout << setw(30) << "AQ = " << "." << AQ.substr(0, AQ.length()/2) <<"  "<< AQ.substr(AQ.length()/2, AQ.length()) << endl;
			cout << setw(30) << "B = " << "." << B << endl;
			cout << setw(30) <<"Normalized B = " << "." << normB << setw(15)<<delay << "Δt"<<endl << endl;
			

			// STEP 2: 2'Complement the normalized B
			string tempNormB = normB;
			string tempComB = complement(tempNormB.insert(0,"0"));
			delay  = delay + B.length(); // length of operand times Δt
			cout<<setw(30)<<"Complement of Normalized B = " << tempComB.substr(0,1) <<"." << tempComB.substr(1, tempComB.length()) <<"     "<<delay<<"Δt"<<endl;
		

			cout<<setw(30)<<"------------------------INITIALIZATION-----------------------" <<endl;
			cout << setw(30) << "."<<AQ.substr(0, AQ.length()/2) <<"  "<<AQ.substr(AQ.length()/2  , AQ.length()) <<endl;
			
			// STEP 3: Adjust AQ

			delay = delay + initAdj * 3; // 3 Δt for each shift required for adjusting AQ
			
			string adjAQ = AQ;
			for(int i = 0; i < AQ.length() ; i++){
				if(AQ[i] == '0' && initAdj > 0 ){
					adjAQ.push_back('*');
					adjAQ = adjAQ.substr(1);
					initAdj --;
				}
			}
			AQ = adjAQ;
			
			cout << setw(30) << "Adjusted AQ: " << "." << adjAQ.substr(0, AQ.length()/2) << "  "<< AQ.substr(AQ.length()/2, AQ.length())<<
			setw(15)<<delay<<"Δt"<<endl;

			//STEP 4: Shift over 0's
			adjAQ = shiftOverZeros(AQ);
			
			AQ = adjAQ;
			string resAQ;
			char isPositive = '0'; //positive by default
			//keep performing the operation unless total # of shifts is less than the operand bit length

			while( totalNoOfShifts <= B.length()){
				
				if(isPositive == '0'){
					//STEP 5: subtract B
					resAQ = subtractB(AQ , normB);	
				}
				else{
					resAQ = addB(AQ , normB);
				}

				//Result is either positive or negative
				if(resAQ.substr(0,1) =="1"){
						cout << setw(30)<<"Negative Result : " << resAQ.substr(0,1)<<"."<<
						resAQ.substr(1, resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2+1, resAQ.length()) 
						<<setw(15)<<delay<<"Δt"<<endl;
				}
				else{
					cout << setw(30)<<"Positive Result : " << resAQ.substr(0,1)<<"."<<
					resAQ.substr(1, resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2+1, resAQ.length()) 
					<<setw(15)<<delay<<"Δt"<<endl;
				}
				
				isPositive = resAQ[0];

				resAQ = resAQ.substr(1);
				
				//STEP 6.1 : If resAQ is +ve,
				if(isPositive == '0'){

					resAQ.push_back('1');
					resAQ = resAQ.substr(1);

					delay = delay + 3; // 3 Δt delay for 1 shift
					
					totalNoOfShifts += 1;

					cout <<setw(30)<<"Shift AQ left, q0 <- 1 :  " <<"."
					<<resAQ.substr(0,resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2,resAQ.length()) 
					<<setw(15)<<delay<<"Δt"<< endl;
					
					// shift over 0's
					resAQ = shiftOverZeros(resAQ);
					if(totalNoOfShifts > resAQ.length()/2){
						AQ = resAQ;
						break;
					}
					
					AQ = resAQ;
				}
				
				//STEP 6.1 : If resAQ is -ve,
				else if(isPositive == '1'){
					resAQ.push_back('0');
					resAQ = resAQ.substr(1);

					delay = delay + 3; // 3 Δt delay for 1 shift
					
					totalNoOfShifts += 1;

					cout <<setw(30)<<"Shift AQ left, q0 <- 0 :  " << "1"<<"."
					<<resAQ.substr(0,resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2,resAQ.length())
					<<setw(15)<<delay<<"Δt" << endl;
					
					// shift over 1's
					resAQ = shiftOverOnes(resAQ);
					if(totalNoOfShifts > B.length()){
						AQ = resAQ;
						break;
					}
					
					AQ = resAQ;
				}
			}
			// correct remainder
			if( isPositive == '1'){
				
				string finalQ = AQ.substr(AQ.length()/2 );
				A = getA (AQ.insert(0, "1"));

				//shift right
				string correctA = A;
				correctA = correctA.substr(0, A.length() - 1 );
				delay += 3;

				cout << setw(30)<<"Correct remainer by  " << "1."<<correctA << setw(15)<<delay<<"Δt"<<endl;
				cout << setw(30)<<"Shifting A and adding B  " <<"."<<normB<<endl;
				cout << setw(38) << "--------"<<endl;
				correctA = addBitStrings(correctA, normB);
				cout << setw(33) << "0." <<correctA << endl;

				cout<<setw(30)<<"------------------------RESULTS-------------------------------"<<endl;
				cout <<setw(30)<<" QUOTIENT: ."<< finalQ<<endl;
				cout<<setw(30) <<" REMAINDER: ."<<properRemainder(correctA)<<endl;
				cout <<setw(30)<< "DELAY: " << delay << "Δt" <<endl;
				cout <<" \n \n \n \n";
			}
			else{
					printOutput(AQ);
			}
		}
		t++;
	}
}

/*
* Get complement of any number a
*/
string complement (string a)
{

 	int l, i, c=0;
 	char b[24];
 	l=a.length();
 	for (i=l-1; i>=0; i--)
 	{
  		if (a[i]=='0')
  			b[i]='1';
  		else
  			b[i]='0';
 	}
 	for(i=l-1; i>=0; i--)
 	{
 		if(i==l-1)
 		{
  			if (b[i]=='0')
  				b[i]='1';
  			else
  			{
   				b[i]='0';
   				c=1;
  			}
 		}
 		else
 		{
  			if(c==1 && b[i]=='0')
  			{
   				b[i]='1';
   				c=0;
  			}
 			else if (c==1 && b[i]=='1')
 			{
  				b[i]='0';
  				c=1;
 			}
		}
	}
	b[l]='\0';

	return b;
}

/*
* Shift AQ over 0's to the left and push 0 to q0
*/
string shiftOverZeros(string AQ){
	string adjAQ = AQ;
	int noOfShiftsHere = 0;
	for( int i = 0 ; i < AQ.length() ; i ++){
		if(totalNoOfShifts > adjAQ.length()/2){
				break;
		}
		else if(AQ[i] == '0'){
			adjAQ.push_back('0');
			adjAQ = adjAQ.substr(1);
			noOfShiftsHere ++;
			totalNoOfShifts ++;

		}
		else
			break;
	}
	if(noOfShiftsHere >0){
		delay = delay + 3 * noOfShiftsHere; // each shift takes 3 * t
		cout<< setw(30)<<"Shift Over 0's: " <<"."<< adjAQ.substr(0, adjAQ.length()/2) <<"  "<<
		adjAQ.substr(adjAQ.length()/2, adjAQ.length())<<setw(15)<<delay<<"Δt"<<endl;
	}

	return adjAQ;
}

/*
* Shift AQ over 1's to the left and push 1 to q0
*/
string shiftOverOnes(string AQ){
	string adjAQ = AQ;
	int noOfShiftsHere = 0;

	for( int i = 0 ; i < AQ.length() ; i ++){
		if(totalNoOfShifts > adjAQ.length()/2){
				break;
		}
		else if(AQ[i] == '1'){
			adjAQ.push_back('1');
			adjAQ = adjAQ.substr(1);
			noOfShiftsHere ++;
			totalNoOfShifts ++;

		}
		else
			break;
	}
	if(noOfShiftsHere >0){
		delay = delay + 3 * noOfShiftsHere; // each shift takes 3 * Δt
		cout<< setw(30)<<"Shift Over 1's: " <<"1."<< adjAQ.substr(0, adjAQ.length()/2) <<"  "<<
		adjAQ.substr(adjAQ.length()/2, adjAQ.length())<<setw(15)<<delay<<"Δt"<<endl;
	}

	return adjAQ;
}

/*
* Get A as the high k bits of dividend AQ
*/
string getA(string AQ){
	return AQ.substr(0, AQ.length()/2 + 1);
}

/*
* Add any two numbers of equal length
* Any number added to symbol "*" outputs "*"
*/
string addBitStrings( string first, string second )
{
	//cout<< " the A and B are : " << first << "  "<< second << endl;
    string result;  // To store the sum bits
 
    int carry = 0;  // Initialize carry
 
    // Add all bits one by one
    for (int i = first.length()-1 ; i >= 0 ; i--)
    {
    	if(first.at(i) == '*' || second.at(i) == '*')
    	{
    		result = result + '*';
    		carry = 0; // CORRECT THIS
    	}
    	else{
    		int firstBit = first.at(i) - '0';
       		int secondBit = second.at(i) - '0';
 
        	// boolean expression for sum of 3 bits
        	int sum = (firstBit ^ secondBit ^ carry)+'0';
 
        	result = (char)sum + result;
 
        	// boolean expression for 3-bit addition
        	carry = (firstBit & secondBit) | (secondBit & carry) | (firstBit & carry);
    	}
    }
 
    // if overflow, then add a leading 1
    //if (carry)
      //  result = '1' + result;
 	

	/**
	* For carry select adder with 2 bit adder block
	* Tdelay = (n/m -1) * Ts + mTc
	* where
	* n = number of total operand bits
	* m = number of bits in full bit adder(we consider 2 bit adder)
	* Ts = delay for multiplexer ( 2t)
	* Tc = delay for m full bit ripple adder 
	**/
	delay = delay + ((first.length()/2 - 1) * 2 + 6); // 6 Δt for 2 bit ripple adder 
	
    return result;
}

/*
* Subtract normalized B from A part of AQ i.e. high k bits of AQ
*/
string subtractB(string AQ, string B){
	string A = getA(AQ.insert(0,"0"));
	
	string comB = complement(B.insert(0, "0"));
	cout<<setw(30)<<"Subtract B: "<<comB.substr(0,1)<<"."<<comB.substr(1, comB.length())<<endl;
	cout << setw(40) <<" ---------"<<endl;
	A = addBitStrings( A, comB);
	A = A.append(AQ.substr(AQ.length()/2 + 1   , AQ.length()));
	return A;
}

/*
* Add normalized B to A part of AQ i.e. high k bits of AQ
*/
string addB(string AQ, string B){
	string A = getA(AQ.insert(0, "1"));
	A = addBitStrings(A , B.insert(0,"0"));
	cout<<setw(30)<<"Add B: "<<"."<<B.substr(1, B.length())<<endl;
	cout << setw(40) <<" ---------"<<endl;
	A = A.append(AQ.substr(AQ.length()/2 + 1   , AQ.length()));
	return A;
}

/*
* Create proper remainder by
* Take all the bits after the decimal in A until the first *
* Then, add zeros to the front of the number obtained from above equal to (2k - total bits in previous step)
*
*/
string properRemainder(string remainder){
	int count = 0;
	int len = 2 * remainder.length();
	string properRem = "";
	for(int i = 0 ; i < len/2; i++){
		if(remainder[i] == '*')
			break;
		else
			properRem = properRem + remainder[i];
		count++;
	}
	
	string addZeros ="";
	for(int i = count; i < len; i++){
		addZeros.append("0");
	}

	properRem = addZeros.append(properRem);
	return properRem;
}

/*
*
* Output the Quotient and Remainder
*
*/
void printOutput(string AQ){
	REMAINDER = properRemainder(AQ.substr(0, AQ.length()/2));
	QUOTIENT = AQ.substr(AQ.length()/2 , AQ.length());

	cout<<setw(30)<<"------------------------RESULTS-------------------------------"<<endl;
	cout <<setw(30)<<" QUOTIENT: "<<"." << QUOTIENT<<endl;
	cout<<setw(30) <<" REMAINDER: "<<"."<<REMAINDER<<endl;
	cout <<setw(30)<< "DELAY: " << delay << "Δt" <<endl;
	cout <<" \n \n \n \n";


}

/*
* Check invalid inputs:
* Case 1: If the divisor is zero, return -1
* Case 2: If the high k bits of dividend (AQ) is greater or equal to total bits in B
*
*/
int checkInvalidInputs(string AQ, string B){
	bool allZeros = true;

	//case: if the divisor is Zero
	for(int i = 0; i < B.length(); i++){
			if(B[i] == '1'){
				allZeros = false;
				break;
			}
	}
	if(allZeros == true)
		return -1;	

	//case : If high K bits of AQ is greater than K bits of B, then 
	string A = AQ.substr(0, AQ.length()/2);
	if(A.compare(B) >= 0){
		return -2;
	}
	return 0;
}
/*
* Check if the given number is hexadecimal or binary
* if the number is binary, it would always have a string - (binary)
* at the end.
* If not binary, convert the hexadecimal to binary
*
*/
string convertHexToBinary(string hexaDecimal){
	//already a binary number
	if(hexaDecimal.at(hexaDecimal.length() -2 ) == 'y'){
		return hexaDecimal.substr(0, hexaDecimal.length()-8);
	}

	string binaryNum = ".";
	for(int i = 1; i < hexaDecimal.length(); i++){
         switch(hexaDecimal[i]){
             case '0': binaryNum = binaryNum.append("0000"); break;
             case '1': binaryNum = binaryNum.append("0001"); break;
             case '2': binaryNum = binaryNum.append("0010"); break;
             case '3': binaryNum = binaryNum.append("0011"); break;
             case '4': binaryNum = binaryNum.append("0100"); break;
             case '5': binaryNum = binaryNum.append("0101"); break;
             case '6': binaryNum = binaryNum.append("0110"); break;
             case '7': binaryNum = binaryNum.append("0111"); break;
             case '8': binaryNum = binaryNum.append("1000"); break;
             case '9': binaryNum = binaryNum.append("1001"); break;
             case 'A': binaryNum = binaryNum.append("1010"); break;
             case 'B': binaryNum = binaryNum.append("1011"); break;
             case 'C': binaryNum = binaryNum.append("1100"); break;
             case 'D': binaryNum = binaryNum.append("1101"); break;
             case 'E': binaryNum = binaryNum.append("1110"); break;
             case 'F': binaryNum = binaryNum.append("1111"); break;
             case 'a': binaryNum = binaryNum.append("1010"); break;
             case 'b': binaryNum = binaryNum.append("1011"); break;
             case 'c': binaryNum = binaryNum.append("1100"); break;
             case 'd': binaryNum = binaryNum.append("1101"); break;
             case 'e': binaryNum = binaryNum.append("1110"); break;
             case 'f': binaryNum = binaryNum.append("1111"); break;
             default:  cout <<"\nInvalid hexadecimal digit  " << hexaDecimal[i]; 
						return 0;
         }
    }
    return binaryNum;
}

/*
* We introduce zeros after decimal in the dividend AQ if the 
* the total bits in dividend is not TWICE the total bits in
* divisor
* 
*/
string addZerosToFront(string str, int len){
	string updatedStr = "";
	for(int i =0 ; i < len; i++){
		updatedStr = updatedStr.insert(0, "0");
	}
	updatedStr  = updatedStr.append(str);
	return updatedStr;
}
