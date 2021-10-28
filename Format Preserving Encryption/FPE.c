#include<stdio.h>
#include<math.h>

char binaryNum[32];
char key_in_binary[48], key_in_binary_correct[48];
char expansion_10bits[10];
char creditcarddigits[16];
char display_six_digits_array[6];
int mid_six_digits_after_tweak_decimal;
char A[10], B[10], ANext[10], BNext[10], Adecrypted[10], Bdecrypted[10], APrev[10], BPrev[10];
int nround = 0;
long decrypted_six_digits;
long encrypted_six_digits;
char tweak[6], mid_six_digits[6], mid_six_digits_after_tweak[6];

//Macro to define number of rounds.
#define NUMBER_OF_ROUNDS_IN_FEISTEL 16

long long Keys[16] =
{
	12345678901234,
	23456745687908,
	16374960736522,
	56754710810976,
	76541246964222,
	36487629769628,
	82647864762176,
	92647512649870,
	71264716491708,
	87165247608701,
	14876794169172,
	81467861497619,
	81647861469978,
	61464998981996,
	87461276428916,
	71696481878473,
};

long long  creditcardnumber;

//function to convert character array digits to integer.
long convert_to_number(char* h, int arraysize, int base)
{
	long sum = 0;
	for (int i = 0; i < arraysize; i++)
	{
		sum = sum + h[i] * pow(base, i);
	}

	return sum;
}

long binaryToDecimal(long n)
{
	long num = n;
	long dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	long temp = num;
	while (temp) {
		long last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}
	return dec_value;
}

void print_input_binary_numbers()
{
	int k = 0;
	long decimalfirsthalf, decimalsecondhalf;
	for (int j = 19; j > 9; j--)
	{
		A[k] = binaryNum[j];
		k++;
	}

	k = 0;

	for (int m = 9; m >= 0; m--)
	{
		B[k] = binaryNum[m];
		k++;
	}
}

// function to convert decimal to binary
void decToBinary(int n)
{
	// counter for binary array
	int i = 0;
	while (n > 0) {

		// storing remainder in binary array
		int a = (int)n % 2;
		binaryNum[i] = a;
		n = n / 2;
		i++;
	}

	print_input_binary_numbers();
}

void decToBinaryKey(long long n)
{
	//making all the entries zero.
	for (int i = 0; i < 48; i++)
	{
		key_in_binary[i] = 0;
	}

	// counter for binary array
	int index = 0;
	while (n > 0) {

		// storing remainder in binary array
		long long a = (long long)n % 2;
		key_in_binary[index] = a;
		n = n / 2;
		index++;
	}

	//reverse to get correct binary key
	for (int i = 0; i < 48; i++)
	{
		key_in_binary_correct[i] = key_in_binary[47 - i];
	}

	printf("\n");

}


void convert_to_digits(long long cred)
{
	for (int i = 0; i < 16; i++)
	{
		creditcarddigits[i] = cred % 10;
		cred = cred / 10;
	}
}


void process_inputs()
{

	printf("Enter CreditCard Number\n");
	scanf_s("%lld", &creditcardnumber);

	convert_to_digits(creditcardnumber);

	printf("issuer identification number is ");
	for (int i = 0; i < 6; i++)
	{
		printf("%d", creditcarddigits[15 - i]);
	}

	int x = 5;
	for (int j = 0; j < 2; j++)
	{
		tweak[x] = creditcarddigits[15 - j];
		x--;
	}
	for (int k = 0; k < 4; k++)
	{
		tweak[x] = creditcarddigits[3 - k];
		x--;
	}

	for (int i = 0; i < 6; i++)
	{
		mid_six_digits[i] = creditcarddigits[4 + i];
	}

	//Adding tweak to mid six digits
	for (int i = 0; i < 6; i++)
	{
		mid_six_digits_after_tweak[i] = (mid_six_digits[i] + tweak[i]) % 10;
	}

	printf("\ntweak is ");
	for (int i = 5; i >= 0; i--)
	{
		printf("%d", tweak[i]);
	}

	printf("\nMiddle six digits are ");
	for (int i = 5; i >= 0; i--)
	{
		printf("%d", mid_six_digits[i]);
	}

	printf("\nMiddle six digits after tweak ");
	for (int i = 5; i >= 0; i--)
	{
		printf("%d", mid_six_digits_after_tweak[i]);
	}
	printf("\n");

	mid_six_digits_after_tweak_decimal = (int)convert_to_number(&mid_six_digits_after_tweak, sizeof(mid_six_digits_after_tweak), 10);
}

void encryprion_function(char* second_half)
{
	int i = 0;
	char expansion_b[48];
	int j = 0;
	for (int i = 0; i < 48; i++)
	{
		if (i % 10 == 0 && i != 0)
		{
			j = 0;
		}
		expansion_b[i] = second_half[j];
		j++;
	}

	decToBinaryKey(Keys[nround]);


	char expansion_40bits[40];
	for (int i = 0; i < 40; i++)
	{
		expansion_40bits[i] = expansion_b[i] ^ key_in_binary_correct[i];
	}

	char expansion_20bits[20];
	//xoring 20 bits 
	for (int i = 0; i < 20; i++)
	{
		expansion_20bits[19 - i] = expansion_40bits[39 - i] ^ expansion_40bits[19 - i];
	}
	//xoring to get 10 bits
	for (int i = 0; i < 10; i++)
	{
		expansion_10bits[9 - i] = expansion_20bits[19 - i] ^ expansion_20bits[9 - i];
	}

}

void display_six_digits(long six_digits_display)
{
	for (int i = 0; i < 6; i++)
	{
		int a = six_digits_display % 10;
		display_six_digits_array[i] = a;
		six_digits_display = six_digits_display / 10;
	}
	//print it
	for (int i = 0; i < 6; i++)
	{
		printf("%d", display_six_digits_array[5 - i]);
	}
}

void display_credit_card_number(short IsEncryption)
{
	if (IsEncryption == 1)
	{
		printf("\nEncrypted Credit card number is  ");
		for (int i = 0; i < 6; i++)
		{
			creditcarddigits[9 - i] = display_six_digits_array[5 - i];
		}
	}

	else if (IsEncryption == 0)
	{
		printf("\nDecrypted credit card number is  ");
		{
			for (int i = 0; i < 6; i++)
			{
				display_six_digits_array[5 - i] = (display_six_digits_array[5 - i] - tweak[5 - i]) % 10;
				if (display_six_digits_array[5 - i] < 0)
				{
					display_six_digits_array[5 - i] = display_six_digits_array[5 - i] + 10;
				}
				creditcarddigits[9 - i] = display_six_digits_array[5 - i];
			}
		}
	}

	for (int i = 0; i < 16; i++)
	{
		printf("%d", creditcarddigits[15 - i]);
	}
}


void feistal()
{
	char encrypted_binary[20], decrypted_binary[20], decrypted_reverse_for_int_conversion[20];
	int nTotalnumberofsteps = 0;

	while (1)
	{
		nround = 0;
		while (nround < NUMBER_OF_ROUNDS_IN_FEISTEL)
		{
			printf("This is Encryption Round........ %d\n", nround);
			printf("Input A and B are\n");
			printf("A = ");
			for (int i = 0; i < 10; i++)
			{
				printf("%d", A[i]);
			}
			printf("       B = ");
			for (int i = 0; i < 10; i++)
			{
				printf("%d", B[i]);
			}
			printf("\n");

			encryprion_function(&B);

			for (int i = 0; i < 10; i++)
			{
				BNext[i] = A[i] ^ expansion_10bits[i];
				ANext[i] = B[i];
			}
			for (int i = 0; i < 10; i++)
			{
				A[i] = ANext[i];
				B[i] = BNext[i];
			}
			nround++;
		}
		nround = 0;
		for (int i = 0; i < 10; i++)
		{
			encrypted_binary[9 - i] = B[i];
			encrypted_binary[19 - i] = A[i];
		}

		encrypted_six_digits = (long)convert_to_number(&encrypted_binary, 20, 2);

		printf("Encrypted six digits are    ");
		display_six_digits(encrypted_six_digits);

		display_credit_card_number(1);

		nTotalnumberofsteps++;
		if (encrypted_six_digits <= 999999)
		{
			break;
		}
	}


	printf("\n\n......................................................\n\n");

	//decryption starts here

	nround = NUMBER_OF_ROUNDS_IN_FEISTEL - 1;
	while (nTotalnumberofsteps > 0)
	{
		while (nround >= 0)
		{
			printf("This is Decryption round........ %d\n", nround);
			printf("Input A and B are\n");
			printf("A = ");
			for (int i = 0; i < 10; i++)
			{
				printf("%d", A[i]);
			}
			printf("       B = ");
			for (int i = 0; i < 10; i++)
			{
				printf("%d", B[i]);
			}
			printf("\n");

			encryprion_function(&A);

			for (int i = 0; i < 10; i++)
			{
				APrev[i] = B[i] ^ expansion_10bits[i];
				BPrev[i] = A[i];
			}

			for (int i = 0; i < 10; i++)
			{
				A[i] = APrev[i];
				B[i] = BPrev[i];
			}

			nround--;
		}
		nround = NUMBER_OF_ROUNDS_IN_FEISTEL - 1;
		nTotalnumberofsteps--;
	}


	for (int i = 0; i < 10; i++)
	{
		decrypted_binary[19 - i] = A[i];
		decrypted_binary[9 - i] = B[i];
	}

	decrypted_six_digits = (long)convert_to_number(&decrypted_binary, 20, 2);

	printf("Decrypted six digits are    ");
	display_six_digits(decrypted_six_digits);

	display_credit_card_number(0);

}

//main function
int main()
{

	process_inputs();

	//converts decimal to binary number.
	decToBinary(mid_six_digits_after_tweak_decimal);

	//feistal encryption.
	feistal();

	getch();

	return 0;
}