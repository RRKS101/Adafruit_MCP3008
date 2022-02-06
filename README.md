MCP3008 C/C++ RP2040 Port

<b>Only Limited Testing - Please Report Any Possible Issues You May Run Into!</b>

This Is A Custom Port Of The <a href="https://github.com/adafruit/Adafruit_MCP3008">Official Library</a> And Aims To Enable The Raspberry Pi Pico's C/C++ SDK To Communicate With The <a href="https://www.adafruit.com/product/856">MCP3008 IC Sold By Adafruit</a>
<img src="https://cdn-learn.adafruit.com/assets/assets/000/030/456/large1024/sensors_raspberry_pi_mcp3008pin.gif?1455010861">

This Library Is A Single Header File Which Defines A ```I_MCP3008``` Class. 
Future Development Would Include;
    - A non-Object Oriented Implementation To Provide Support For C.
    - Multisampling To Provide Less Jitter Between Readings
    - 

This Class Can Optionally Initialize A Hardware SPI Instance Or You Can Specify A Already Initialized One. Please Not That If The Object Creates Its Own SPI Device, It Will Be Deinitialized With The Object By Default. 
This Can Be Avoided Through Calling <a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L118">```setFlag_SPIinternallyManaged(false);```</a>

<a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L91#L97">```uint16_t readADC(uint8_t channel)```</a> And <a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L102#L106">```uint16_t readADCDiff(uint8_t differential)```</a> Are Aliases To 
<a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L150#L161">```uint16_t SPIxADC(uint8_t channel, bool differential)```</a> But With Bounds Checks And A Constant Final Paramater. Should You Require A Lower Overhead, Use The SPIxADC Function, Be Warned That It Does Not Offer Any Guards Against Misuse. 

For Differentials, Please Refer To <a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L126#L144">```enum class DIFFERENTIALS : uint8_t;```</a>

The Class Also Holds The SPI Clock/Baudrate. This Is Provided Due To The Potential Of A Desired SPI Clock Failing To Be Achieved In The Constructor That Initializes A SPI Instance. Should You Change The SPI Clock Rate, Please Call <a href="https://github.com/RRKS101/Adafruit_MCP3008/blob/51c0cfd252fc4a0251769ffd1c850396ee330eb0/mcp3008.h#L113">```updateClockRate();```</a>As The Clock Rate Is Not Automatically Synced. No Internal Functions Rely On The Storage Of the Clock Rate, And Can Be Commented Out Should You Not Require It. 