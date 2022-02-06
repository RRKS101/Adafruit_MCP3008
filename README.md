MCP3008 C/C++ RP2040 Port

<b>Only Limited Testing - Please Report Any Possible Issues You May Run Into!</b>

This Is A Custom Port Of The <a href="https://github.com/adafruit/Adafruit_MCP3008">Official Library</a> And Aims To Enable The Raspberry Pi Pico's C/C++ SDK To Communicate With The <a href="https://www.adafruit.com/product/856">MCP3008 IC Sold By Adafruit</a>
<img src="https://cdn-learn.adafruit.com/assets/assets/000/030/456/large1024/sensors_raspberry_pi_mcp3008pin.gif?1455010861">

This Library Is A Single Header File Which Defines A I_MCP3008 Class. Future Plans Include A non-Object Oriented Implementation To Provide Support For C.

This Class Can Optionally Initialize A Hardware SPI Instance Or You Can Specify A Already Initialized One. Please Not That If The Object Creates Its Own SPI Device, It Will Be Deinitialized With The Object By Default. 
This Can Be Avoided Through Calling ```setFlag_SPIinternallyManaged(false);```

```uint16_t readADC(uint8_t channel)``` And ```uint16_t readADCDiff(uint8_t differential)``` Are Aliases To ```uint16_t SPIxADC(uint8_t channel, bool differential)``` But With Bounds Checks And A Constant Final Paramater. Should You Require A Lower Overhead, Use The SPIxADC Function, Be Warned That It Does Not Offer Any Guards Against Misuse. 

For Differentials, Please Refer To ```enum class DIFFERENTIALS : uint8_t;```
The Class Also Holds The SPI Clock/Baudrate. This Is Provided Due To The Potential Of A Desired SPI Clock Failing To Be Achieved In The Constructor That Initializes A SPI Instance. Should You Change The SPI Clock Rate, Please Call ```updateClockRate();```