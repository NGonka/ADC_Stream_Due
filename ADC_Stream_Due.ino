/*
#### change log####
version 0.0.1beta
rewrite of ADC_Stream for Arduino Due
-ADC reading only

*/


// header definitions
boolean DEBUG=false;
String version ="0.0.1beta";
static long buad = 250000;//310000;






//Variables for ADC readings
long readA0;
long readA1;
long readA2;


int ADC_Res=10;

int Res=2^ADC_Res;

int readAnalog;


//Variables for Serial Communication
String msg = "HH";


//Variables for time reading
volatile unsigned long lastTime = 0;
volatile unsigned long currTime= 0;
//long Power =0;
volatile unsigned long currTime1 = 0;
volatile unsigned long currTime2 = 0;
volatile unsigned long meassureTime = 0;



int adcReadA0()
{
	return analogRead(A0);
}

int adcReadA1()
{
	return analogRead(A1);
}

int adcReadA2()
{
	return analogRead(A2);
}


long toVolt(int reading, long Vcc)
{
	//! Converts the ADC reading to Voltage.
	//! Consider the actual Supply voltage and the Voltage divider factor of 5
	///return returns milliVolts
	if (DEBUG)
	{
		Serial.print("V_raw: ");
		Serial.println(reading);
	}
	return long(Vcc*reading*5/Res);
	
}

long toCurrent(int reading, long Vcc)
{
	//! Converts the ADC reading to Current.
	//! Consider the actual Supply voltage
	///return returns milliAmps
	if (DEBUG)
	{
		Serial.print("I_raw: ");
		Serial.println(reading);
	}
	return long(Vcc*reading/Res);
	
}


String constTimeStringLengthOf10(unsigned long timestamp)
{
	char charBuffer[11+1];
	
	sprintf(charBuffer,"%10lu",timestamp);
	return String(charBuffer);
}

String constRPMStringLengthOf7(long value)
{
	char charBuffer[11+1];
	
	sprintf(charBuffer,"%10ld",value);
	return String(charBuffer);
}

String constVoltStringLengthOf5(long value)
{
	char charBuffer[6+1];
	
	sprintf(charBuffer,"%5ld",value);
	return String(charBuffer);
}

String constCurrStringLengthOf4(long value)
{
	char charBuffer[5+1];
	
	sprintf(charBuffer,"%4ld",value);
	return String(charBuffer);
}



void setup()
{
	//! Initialize Serial Communication
	Serial.begin(buad);
	Serial.println("Ready");
	//last_micros=micros();
	analogReadResolution(ADC_Res); //10bit for the moment; 12 bit later

}





void loop()
{

	
	//! Read the Serial Data in for the commands
	if(Serial.available()>0)
	{
		msg="";
		while(Serial.available()>0)
		{
			msg+=char(Serial.read());
			delay(10);
		}
		msg = msg.substring(0,2);
		//serial.print("msg: ");
		//serial.println(msg);
	}
	

	//! Definition of the Serial Commands and the send back Data
	if (msg.equals("DD"))
	{
		
		DEBUG=!DEBUG;
		msg= "AA";
		if (DEBUG)
		{
			Serial.println("Debug: On");
			
		}
		else
		{
			Serial.println("Debug: Off");
			
		}
		
	}
	

	else if (msg.equals("AA"))
	{
		currTime2 = micros();
		long VCC = 3300;//readVcc();
		readA0=toVolt(adcReadA0(),VCC);		//in mV
		//delay(2);
		readA1=toCurrent(adcReadA1(),VCC);	//in mA
		//delay(2);
		
		readA2=1000;//enc_rpm();					//in milli rpm (rpm*1000)
		
		
		
		
		
		
		
		if(!DEBUG)
		{
			
			//JSON Output
			Serial.println("{");
				Serial.print("\t\"vol\": ");
				Serial.print(constVoltStringLengthOf5(readA0));
				Serial.println(",");
				Serial.print("\t\"cur\": ");
				Serial.print(constCurrStringLengthOf4(readA1));
				Serial.println(",");
				Serial.print("\t\"rpm\": ");
				Serial.print(constRPMStringLengthOf7(readA2));
				Serial.println(",");
				//Serial.print("\t\"power\": ");
				//Serial.print(constTimeStringLengthOf4(Power));
				//Serial.println(",");
				Serial.print("\t\"tim\": ");
				Serial.println(constTimeStringLengthOf10(currTime2));
			Serial.println("}");
			Serial.println("EOL");
		}
		else
		{
			//Raw data output
			Serial.print("V: ");
			Serial.print(readA0);
			Serial.print("; ");
			Serial.print("I: ");
			Serial.print(readA1);
			Serial.print("; ");
			Serial.print("N: ");
			Serial.print(readA2);
			Serial.print("; ");
			Serial.print("t: ");
			Serial.println(currTime2);

		}

		
	}
	else if (msg.equals("HH"))
	{
		//Help Massage
		Serial.println("Measurement Board");
		Serial.print("Firmware Version: ");
		Serial.println(version);
		Serial.println("Commands:");
		Serial.println("HH : prints this massage");
		Serial.println("AA : print Volt, Current, RPM, Power and a Timestamp (since last restart) in actual Units ([mV],[mA],[1/min],[mW],[us])");
		Serial.println("     Output is in JSON Style!!!");
		//Serial.println("Ax : prints only Analog Pin x (bit value (x: 0-5))");
		Serial.println("DD : toggle debug mode on and off");
		msg="";
		
		
		
	}
	else
	{
		msg="";
		
	}

}


