/**
*
*
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
string substractB(string AQ, string B);
string addB(string AQ, string B);
void printOutput(string AQ);
string convertHexToBinary(string hexInput);
int checkInvalidInputs(string AQ, string B);
string properRemainder(string AQ);

int main(){
	string Q , A , B , AQ , comB;
	int t = 0, T;

	//input number of test cases
	cin>>T;

	while(t < T){
		cin >> AQ >> B;

		cout <<" -----------------------Test Case: "<<t+1<<"----------------------------";
		cout <<setw(30)<<" The input is: " << AQ <<"    "<<B <<endl;

		//check and convert the hexadecimal input to binary
		AQ = convertHexToBinary(AQ);
		B = convertHexToBinary(B);

		//get rid of "." for calculations
		AQ = AQ.substr(1, AQ.length());
		B  = B.substr(1, B.length());

		cout << setw(30)<<" The dividend length: " << AQ <<" "<<AQ.length()<<endl;
		cout<< setw(30)<<" The divisor length: "  << B<<" "<<B.length()<<endl<<endl;

		delay = 0 , totalNoOfShifts = 0;
		int initAdj = 0, k = 0, BLen = B.length();
		string normB = B;

		//Check if the inputs are valid
		int invalid = checkInvalidInputs(AQ, B) ;
		//invalid = 0;
		if(invalid == -1)
			{
				cout <<" Invalid Inputs \n";
			}
		else{
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
			

			// STEP 2: Complement B
			string tempNormB = normB;
			string tempComB = complement(tempNormB.insert(0,"0"));
			delay  = delay + B.length(); // length of operand times t
			cout<<setw(30)<<"Complement of Normalized B = " << tempComB.substr(0,1) <<"." << tempComB.substr(1, tempComB.length()) <<"     "<<delay<<"Δt"<<endl;
		

			cout<<setw(30)<<"------------------------INITIALIZATION-----------------------" <<endl;
			cout << setw(30) << "."<<AQ.substr(0, AQ.length()/2) <<"  "<<AQ.substr(AQ.length()/2  , AQ.length()) <<endl;
			
			// STEP 3: Adjust AQ

			delay = delay + initAdj * 3; // 3 t for each shift required for adjusting AQ
			
			string adjAQ = AQ;
			for(int i = 0; i < AQ.length() ; i++){
				if(AQ[i] == '0' && initAdj > 0 ){
					adjAQ.push_back('*');
					adjAQ = adjAQ.substr(1);
					initAdj --;
				}
			}
			AQ = adjAQ;
			totalNoOfShifts += initAdj;

			//print the quotient and remainder
			if(totalNoOfShifts >= AQ.length()/2){
				printOutput(AQ);
			
			}
			cout << setw(30) << "Adjusted AQ: " << "." << adjAQ.substr(0, AQ.length()/2) << "  "<< AQ.substr(AQ.length()/2, AQ.length())<<
			setw(15)<<delay<<"Δt"<<endl;

			//STEP 4: Shift over 0's
			adjAQ = shiftOverZeros(AQ);
			
			// check if the quotient and remainder are generated
			if(totalNoOfShifts >= B.length()){
				//end here;
				cout << " End at very beginning" << endl;
			}
			//cout <<" Shifted over 0's " << adjAQ << endl;

			AQ = adjAQ;
			string resAQ;
			char isPositive = '0'; //positive by default
			//keep performing the operation unless total # of shifts is less than the operand bit length
			while( totalNoOfShifts <= B.length()){
				
				if(isPositive == '0'){
					//STEP 5: Substract B
					resAQ = substractB(AQ , normB);
					cout << setw(30)<<"Negative Result : " << resAQ.substr(0,1)<<"."<<
					resAQ.substr(1, resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2+1, resAQ.length()) 
					<<setw(15)<<delay<<"Δt"<<endl;
				}
				else{
					resAQ = addB(AQ , normB);
					cout << setw(30)<<"Positive Result : " << resAQ.substr(0,1)<<"."<<
					resAQ.substr(1, resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2+1, resAQ.length())
					<<setw(15)<<delay<<"Δt"<<endl;
				}
				
				isPositive = resAQ[0];

				resAQ = resAQ.substr(1);
				
				//STEP 6.1 : If resAQ is +ve,
				if(isPositive == '0'){
					//cout << endl << "The result is positive: " << endl ;
					// shift AQ left, q0 <- 1
					resAQ.push_back('1');
					resAQ = resAQ.substr(1);

					delay = delay + 3; // 3 t delay for 1 shift
					
					totalNoOfShifts += 1;

					cout <<setw(30)<<"Shift AQ left, q0 <- 1 :  " <<"."
					<<resAQ.substr(0,resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2,resAQ.length()) 
					<<setw(15)<<delay<<"Δt"<< endl;
					
					//print the quotient and remainder
					if(totalNoOfShifts >= resAQ.length()/2){
						AQ = resAQ;
						//printOutput(resAQ);
						break;
					}
					// shift over 0's
					resAQ = shiftOverZeros(resAQ);
					if(totalNoOfShifts >= resAQ.length()/2){
						AQ = resAQ;
						break;
					}
					
					AQ = resAQ;
				}
				
				//STEP 6.1 : If resAQ is -ve,
				else if(isPositive == '1'){
					//cout << endl << "The result is negative: " << endl ;
					// shift AQ left, q0 <- 0
					resAQ.push_back('0');
					resAQ = resAQ.substr(1);

					delay = delay + 3; // 3 t delay for 1 shift
					//cout<<"Total Delay in shift AQ left and q0 <- 0" <<delay <<"\n";
					totalNoOfShifts += 1;

					cout <<setw(30)<<"Shift AQ left, q0 <- 0 :  " << "1"<<"."
					<<resAQ.substr(0,resAQ.length()/2)<<"  "<<resAQ.substr(resAQ.length()/2,resAQ.length())
					<<setw(15)<<delay<<"Δt" << endl;
					

					//print the quotient and remainder
					if(totalNoOfShifts >= resAQ.length()/2){
						AQ = resAQ;
						//printOutput(resAQ);
						//break;
					}
					// shift over 1's
					resAQ = shiftOverOnes(resAQ);
					if(totalNoOfShifts >= B.length()){
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
				delay += 3; //TODO: Verify this

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

string shiftOverZeros(string AQ){
	string adjAQ = AQ;
	int noOfShiftsHere = 0;
	for( int i = 0 ; i < AQ.length() ; i ++){
		if(totalNoOfShifts >= adjAQ.length()/2){
				//cout << "Shifted over 0's : " << adjAQ << endl;
				QUOTIENT = adjAQ.substr(0, adjAQ.length()/2);
				REMAINDER = adjAQ.substr(adjAQ.length()/2+1 , adjAQ.length());
				//cout << "we are done here";
				//printOutput(adjAQ);
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

string shiftOverOnes(string AQ){
	string adjAQ = AQ;
	int noOfShiftsHere = 0;
	for( int i = 0 ; i < AQ.length() ; i ++){
		if(totalNoOfShifts >= adjAQ.length()/2){
				QUOTIENT = adjAQ.substr(0, adjAQ.length()/2);
				REMAINDER = adjAQ.substr(adjAQ.length()/2+1 , adjAQ.length());
				
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
		delay = delay + 3 * noOfShiftsHere; // each shift takes 3 * t
		cout<< setw(30)<<"Shift Over 1's: " <<"."<< adjAQ.substr(0, adjAQ.length()/2) <<"  "<<
		adjAQ.substr(adjAQ.length()/2, adjAQ.length())<<setw(15)<<delay<<"Δt"<<endl;
	}

	return adjAQ;
}

string getA(string AQ){
	return AQ.substr(0, AQ.length()/2 + 1);
}

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
	delay = delay + ((first.length()/2 - 1) * 2 + 6); // 6 t for 2 bit ripple adder 
	//cout<<"Total Delay in addition: "<<delay<<"\n";
    return result;
}
string substractB(string AQ, string B){
	string A = getA(AQ.insert(0,"0"));
	//cout << " A is : " << A << endl;
	
	string comB = complement(B.insert(0, "0"));
	cout<<setw(30)<<"Substract B: "<<comB.substr(0,1)<<"."<<comB.substr(1, comB.length())<<endl;
	cout << setw(40) <<" ---------"<<endl;
	//cout << "Complement of B: " << comB << endl;
	A = addBitStrings( A, comB);
	//cout << " A after substraction: " << A << endl;
	A = A.append(AQ.substr(AQ.length()/2 + 1   , AQ.length()));
	return A;
}

string addB(string AQ, string B){
	string A = getA(AQ.insert(0, "1"));
	//cout << " A is : " << A << endl;
	A = addBitStrings(A , B.insert(0,"0"));
	cout<<setw(30)<<"Add B: "<<"."<<B.substr(1, B.length())<<endl;
	cout << setw(40) <<" ---------"<<endl;
	//cout<<" A after addition: " << A << endl;
	A = A.append(AQ.substr(AQ.length()/2 + 1   , AQ.length()));
	return A;
}

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
void printOutput(string AQ){
	REMAINDER = properRemainder(AQ.substr(0, AQ.length()/2));
	QUOTIENT = AQ.substr(AQ.length()/2 , AQ.length());

	cout<<setw(30)<<"------------------------RESULTS-------------------------------"<<endl;
	cout <<setw(30)<<" QUOTIENT: "<<"." << QUOTIENT<<endl;
	cout<<setw(30) <<" REMAINDER: "<<"."<<REMAINDER<<endl;
	cout <<setw(30)<< "DELAY: " << delay << "Δt" <<endl;
	cout <<" \n \n \n \n";


}

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
		return -1;
	}
	return 0;
}
string convertHexToBinary(string hexaDecimal){
	//cout <<" The number is : " << hexaDecimal.at(hexaDecimal.length() -2 )<<endl;
	//already a binary number
	if(hexaDecimal.at(hexaDecimal.length() -2 ) == 'y'){
		return hexaDecimal.substr(0, hexaDecimal.length()-8);
	}

	string binaryNum = ".";
	for(int i = 1; i < hexaDecimal.length(); i++){
		//cout <<" THe digit is: " << hexaDecimal[i];
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
